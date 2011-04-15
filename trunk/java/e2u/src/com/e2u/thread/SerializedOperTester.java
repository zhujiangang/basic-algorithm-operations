package com.e2u.thread;

public class SerializedOperTester
{
	private static void testCreate(String name, long ms)
	{
		System.out.println("[Start]: Create " + name + ", ms = " + ms);

		SerializedOperBooker.getInstance().book(name,
			Thread.currentThread());
		SerializedOperBooker.getInstance().checkExecutable(name,
			Thread.currentThread());

		try
		{
			System.out.println("[Work]: Create: Thread "
				+ Thread.currentThread().getName() + ", name " + name);
			try
			{
				Thread.sleep(ms);
			}
			catch(InterruptedException e)
			{
				// Ignore
			}
		}
		catch(Exception e)
		{
		}
		finally
		{
			System.out.println("[Finish]: Create " + name);
			SerializedOperBooker.getInstance().unbook(name);
		}
	}

	private static void testRemove(String name, long ms)
	{
		System.out.println("[Start]: Remove " + name + ", ms = " + ms);
		SerializedOperBooker.getInstance().book(name,
			Thread.currentThread());
		SerializedOperBooker.getInstance().checkExecutable(name,
			Thread.currentThread());

		try
		{
			System.out.println("[Work]: Remove: Thread "
				+ Thread.currentThread().getName() + ", name " + name);
			try
			{
				Thread.sleep(ms);
			}
			catch(InterruptedException e)
			{
				// Ignore
			}
		}
		catch(Exception e)
		{
		}
		finally
		{
			System.out.println("[Finish]: Remove " + name);
			SerializedOperBooker.getInstance().unbook(name);
		}
	}

	private static class WorkerThread extends Thread
	{
		private static int counter = 0;
		private int id = counter++;

		private static int threadcount = 0;

		public static int threadCount()
		{
			return threadcount;
		}

		public WorkerThread()
		{
			threadcount++;
			start();
		}

		public void run()
		{
			String name = "name" + (int) (id * Math.random()) % 4;
			long ms = (long) (20000 * (Math.random()));

			if((int) (id * Math.random()) % 2 == 0)
			{
				testCreate(name, ms);
			}
			else
			{
				testRemove(name, ms);
			}
			threadcount--;
		}
	}

	public static void main(String[] args) throws InterruptedException
	{
		int MAX_THREAD_COUNT = 10;

		while(true)
		{
			if(WorkerThread.threadCount() < MAX_THREAD_COUNT)
			{
				new WorkerThread();
			}
			Thread.sleep(500);
		}
	}
}
