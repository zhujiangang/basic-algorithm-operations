package com.bao.examples.thread;

import java.util.Iterator;
import java.util.LinkedList;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class QueueTokenPool implements TokenPool
{
	private static Log log = LogFactory.getLog(QueueTokenPool.class);
	
	private static final int QUE_CONTENT_NONE = 0;
	private static final int QUE_CONTENT_HEAD = 1;
	private static final int QUE_CONTENT_FULL = 2;

	private int count = -1;
	private long[] tokenHolders = null;

	private LinkedList<Thread> requestQueue = null;
	private int capacity = -1;

	public QueueTokenPool()
	{
		this(-1);
	}

	public QueueTokenPool(int queueSize)
	{
		count = MAX_TOKEN_COUNT;
		if(count <= 0)
		{
			throw new IllegalArgumentException("Invalid total token count = "
				+ count);
		}

		tokenHolders = new long[count];
		for(int i = 0; i < count; i++)
		{
			tokenHolders[i] = -1;
		}

		requestQueue = new LinkedList<Thread>();
		this.capacity = (queueSize <= 0) ? -1 : queueSize;
		
		log.info("TokenPool TotalCount = " + count + ", capacity = " + capacity);
	}

	public int getToken()
	{
		synchronized(this)
		{
			if(log.isDebugEnabled())
			{
				log.debug("[Pre  Enqueue]- " + getCurrentState(QUE_CONTENT_FULL));
			}
			
			//If there's NO wait seats and NO available token count, throw exception
			if(capacity > 0 && (requestQueue.size() - count) >= capacity)
			{
				log.info("The waiting queue is already full. " + getCurrentState(QUE_CONTENT_NONE));
				return -1;
			}
			
			Thread t = Thread.currentThread();
			requestQueue.add(t);
			
			if(log.isDebugEnabled())
			{
				log.debug("[Post Enqueue]- " + getCurrentState(QUE_CONTENT_FULL));
			}

			//1. No available tokens
			//2. The current thread is not at the first of the waiting queue.
			while(count <= 0 || t != requestQueue.peek())
			{
				try
				{
					log.info("getToken [WAIT]- " + getCurrentState(QUE_CONTENT_HEAD));
					// Wait until the condition changed
					wait();
				}
				catch(InterruptedException e)
				{
					log.info("The thread is interrupted.", e);
					Thread.currentThread().interrupt(); // very important
				}
			}

			//1. The current thread is the first one in the queue
			//2. There are available tokens.
			for(int i = tokenHolders.length - 1; i >= 0; i--)
			{
				if(tokenHolders[i] == -1)
				{
					log.info("getToken [SUCC]- " + getCurrentState(QUE_CONTENT_HEAD));
					if(log.isDebugEnabled())
					{
						log.debug("[Pre  Dequeue]- " + getCurrentState(QUE_CONTENT_FULL));
					}
					
					tokenHolders[i] = Thread.currentThread().getId();
					count--;
					
					// Remove the current thread from queue
					requestQueue.removeFirst();
					
					if(log.isDebugEnabled())
					{
						log.debug("[Post Dequeue]- " + getCurrentState(QUE_CONTENT_FULL));
					}
					
					//if there are still available tokens and the queue is not empty
					//notify the threads in the queue
					if(count > 0 && !requestQueue.isEmpty())
					{
						notifyAll();
					}

					return i;
				}
			}
			log.fatal("This should not happen. " + getCurrentState(QUE_CONTENT_FULL));
			return -1;
		}
	}

	public void releaseToken(int token)
	{
		synchronized(this)
		{
			if(token < 0 || token >= tokenHolders.length)
			{
				log.error("This should be an error call this API. token = " + token);
				return;
			}
			if(tokenHolders[token] == Thread.currentThread().getId())
			{
				tokenHolders[token] = -1;
				count++;

				// count must be greater than 0, skip this condition check.
				// if(count > 0 && !requestQueue.isEmpty())
				if(!requestQueue.isEmpty())
				{
					notifyAll();
				}
			}
		}
	}

	public int getTotalCount()
	{
		synchronized(this)
		{
			return tokenHolders.length;
		}
	}

	public int getReservedCount()
	{
		synchronized(this)
		{
			if(count <= 0)
			{
				return tokenHolders.length;
			}
			return tokenHolders.length - count;
		}
	}
	
	private String getCurrentState(int queContentType)
	{
		StringBuilder sb = new StringBuilder("[State]: ");
		sb.append("TokenCount=").append(count).append(", ");
		sb.append("QueueSize=").append(requestQueue.size());

		switch(queContentType)
		{
			case QUE_CONTENT_HEAD:
			{
				sb.append(", ");
				if(requestQueue.isEmpty())
				{
					sb.append("QueueHead=[]");
				}
				else
				{
					sb.append("QueueHead=[");
					sb.append(requestQueue.peek().getName());
					sb.append("]");
				}
				break;
			}
			case QUE_CONTENT_FULL:
			{
				sb.append(", QueueContent=");
				sb.append(getQueueContent(requestQueue));
				break;
			}
			case QUE_CONTENT_NONE:
			default:
			{
				break;
			}
		}
		return sb.toString();
	}
	
	private String getQueueContent(LinkedList<Thread> queue)
	{
		StringBuilder sb = new StringBuilder("[");
		for(Iterator<Thread> iter = queue.iterator(); iter.hasNext(); )
		{
			Thread t = iter.next();
			sb.append(getShortThreadName(t));
			
			if(iter.hasNext())
			{
				sb.append(",");
			}
		}
		sb.append("]");
		
		return sb.toString();
	}
	
	private String getShortThreadName(Thread t)
	{
		String threadName = t.getName();
		if(threadName.contains("@qtp-"))
		{
			int index = threadName.lastIndexOf('-');
			if(threadName.length() > index + 1)
			{
				threadName = threadName.substring(index + 1);
			}
		}
		
		return threadName;
	}
}
