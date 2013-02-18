package com.bao.examples.thread;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class DefaultTokenPool implements TokenPool
{
	private static Log log = LogFactory.getLog(DefaultTokenPool.class);

	private int count = -1;
	private long[] tokenHolders = null;

	public DefaultTokenPool()
	{
		count = MAX_TOKEN_COUNT;
		log.info("TokenPool TotalCount = " + count);
		if(count <= 0)
		{
			throw new IllegalArgumentException("Invalid total token count = " + count);
		}

		tokenHolders = new long[count];
		for(int i = 0; i < count; i++)
		{
			tokenHolders[i] = -1;
		}
	}

	public int getToken()
	{
		synchronized(this)
		{
			if(count <= 0)
				return -1;
			for(int i = tokenHolders.length - 1; i >= 0; i--)
			{
				if(tokenHolders[i] == -1)
				{
					tokenHolders[i] = Thread.currentThread().getId();
					count--;
					return i;
				}
			}
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
}
