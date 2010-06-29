package com.e2u.thread;

public class ThreadPoolTest
{
	public static void test1()
	{
		ThreadPool pool = new ThreadPool();

		for(int i = 0; i < 100; i++)
		{
			TestRunner r = new TestRunner("TestRunner-" + i, (int) (Math.random() * 30));
			pool.run(r);
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		test1();
	}

}

class TestRunner implements Runnable
{
	private int runTime = 0;
	private String runnerName = null;

	public TestRunner(String name, int runTime)
	{
		this.runTime = runTime;
		runnerName = name;
	}

	public void run()
	{
		System.out.println("[START]-" + Thread.currentThread().getName() + ", " + runnerName + " start to run in " + runTime + " seconds.");
		try
		{
			Thread.sleep(runTime * 1000);
		}
		catch(InterruptedException e)
		{
			e.printStackTrace();
		}
		System.out.println("[FINISH]-" + Thread.currentThread().getName() + ", " + runnerName + " end to run in " + runTime + " seconds.");
	}
}
