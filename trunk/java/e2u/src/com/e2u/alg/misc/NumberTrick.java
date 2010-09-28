package com.e2u.alg.misc;

public class NumberTrick
{
	public static int gcd(int m, int n)
	{
		if(n == 0)
		{
			return m;
		}
		return gcd(n, m % n);
	}
	public static void test()
	{
		int[] ms = {4, 5, 6, 7, 8, 9, 10, 11};
		int[] ns = {6, 8, 20, 21, 56, 12, 55, 7};
		
		for(int i = 0; i < ms.length; i++)
		{
			System.out.printf("gcd(%d, %d)=%d\n", ms[i], ns[i], gcd(ms[i], ns[i]));
		}
	}
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		
		test();
	}

}
