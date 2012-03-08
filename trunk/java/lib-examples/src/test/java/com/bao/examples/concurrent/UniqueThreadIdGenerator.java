package com.bao.examples.concurrent;

import java.util.concurrent.atomic.AtomicInteger;

public class UniqueThreadIdGenerator
{
	private static final AtomicInteger uniqueId = new AtomicInteger(0);

	private static final ThreadLocal<Integer> uniqueNum = new ThreadLocal<Integer>()
	{
		@Override
		protected Integer initialValue()
		{
			System.out.println("Called by: " + Thread.currentThread().getName());
			return uniqueId.getAndIncrement();
		}
	};

	public static int getCurrentThreadId()
	{
		return uniqueNum.get();
	}

	private static class TestThread extends Thread
	{
		private int prev = -1;

		public void run()
		{
			try
			{
				Thread.sleep(1000);
			}
			catch(Exception e)
			{
				// TODO: handle exception
			}
			for(int j = 0; j < 10; j++)
			{
				int curr = UniqueThreadIdGenerator.getCurrentThreadId();
				if(prev != -1)
				{
					System.err.println("[FAIL]: curr=" + curr + ", prev=" + prev);
				}
				System.out.println(getName() + ": " + curr);

				try
				{
					Thread.sleep(100);
				}
				catch(Exception e)
				{
				}
			}
		}
	}

	public static void main(String[] args)
	{
		int count = 10;

		for(int i = 0; i < count; i++)
		{
			Thread t = new TestThread();
			t.setName("T-" + i);
			t.start();
		}
	}
} // UniqueThreadIdGenerator

