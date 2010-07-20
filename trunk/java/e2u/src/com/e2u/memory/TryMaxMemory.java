package com.e2u.memory;

public class TryMaxMemory
{
	private static int[][] occupy;
	private static int count = 0;
	private static int MAX_MALLOC_TIMES = 10;
	static
	{
		occupy = new int[MAX_MALLOC_TIMES][];
	}
	private static void malloc(long capacity)
	{
		if(count >= MAX_MALLOC_TIMES)
		{
			System.err.println("Too many times");
			return;
		}
		long numberOfInt = capacity >> 2; // capacity / 4
		
		if(numberOfInt <= Integer.MAX_VALUE)
		{
			occupy[count++] = new int[(int)numberOfInt];
			System.out.println("(" + count + ") malloced memeory: " + capacity + " in bytes");
		}
		else
		{
			System.out.println("Too big");
		}
	}
	private static void showMemory()
	{
		System.out.printf("Maximum:\t %dM\n", Runtime.getRuntime().maxMemory() / (1024 * 1024));
		System.out.printf("Total:\t\t %dM\n", Runtime.getRuntime().totalMemory() / (1024 * 1024));
		System.out.printf("Free:\t\t %dM\n", Runtime.getRuntime().freeMemory() / (1024 * 1024));
	}
	
	/**
	 * @param args
	 */
	//vmargs: -Xms64M -Xmx512M
	public static void main(String[] args)
	{
		showMemory();
		malloc(1024 * 1024* 63);
		showMemory();
		malloc(1024 * 1024* 63);
		showMemory();
		malloc(1024 * 1024* 63);
		showMemory();
		malloc(1024 * 1024* 63);
		showMemory();
	}

}
