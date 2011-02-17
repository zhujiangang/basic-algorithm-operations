package com.e2u.gc.weakref;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class MessagePublisher
{

	private static List listeners = new ArrayList();

	public static void addMessageListener(MessageListener l)
	{
		// you can try : listeners.add(l);
		listeners.add(l);
//		listeners.add(new WeakListener(l));
	}

	public static void removeMessageListener(MessageListener l)
	{
		listeners.remove(l);
	}

	public static void publishMessage(String message)
	{
		for(int i = 0; i < listeners.size(); i++)
		{
			MessageListener l = (MessageListener) listeners.get(i);
			l.recevied(message);
		}
	}

	static
	{
		Thread thread = new Thread(new Runnable()
		{
			public void run()
			{
				while(true)
				{
					Runtime runtime = Runtime.getRuntime();
					long free = runtime.freeMemory();
					int value = (int) (free * 100 / runtime.totalMemory());
					publishMessage(new Date() + " Memory:" + value + "%");

					try
					{
						Thread.sleep(200);
					}
					catch(InterruptedException e)
					{
					}
				}
			}
		});
		thread.setPriority(Thread.MAX_PRIORITY);
		thread.setDaemon(true);
		thread.start();
	}
}
