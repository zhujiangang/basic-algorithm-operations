package com.e2u.bit;

public class BitTwiddling
{
	public static int min(int a, int b)
	{
		int diff = b - a;
		// b < a: a + (diff & -1) = a + diff = a + (b - a) = b
		// b > a: a + (diff & 0) = a + 0 = a
		return a + (diff & (diff >> 31));
	}

	public static int max(int a, int b)
	{
		int diff = b - a;
		// b < a: b - (diff & -1) = b - diff = b - (b - a) = a
		// b > a: b - (diff & 0) = b - 0 = b
		return b - (diff & (diff >> 31));
	}

	/**
	 * 正数和0的补码同原码，负数的补码是绝对值的反码加1。-1的补码表示是0xFFFFFFFF。
	 * 
	 * 原码 true code 
	 * 补码 complemental code: 负数的补码是绝对值的反码加1 
	 * 反码 ones-complement code: 直接取反(1变0, 0变1)后的结果
	 * 
	 * 1`s complement(one's complement) 指的是正数=原码,负数=反码 
	 * 2`s complement(two's complement) 指的就是通常所指的补码
	 * 
	 * @param a
	 * @return
	 */
	public static int abs(int a)
	{
		// 既然负数的补码是绝对值的反码+1,
		// 那么负数的绝对值就是这个负数-1后的反码
		// 当x为负数时 ==> x = ~|x| + 1 ==> |x| = ~(x - 1)
		int temp = (a >> 31);
		// a >= 0: a >> 31 = 0 --> (a + temp) ^ temp = a ^ 0 = a;
		// a < 0: a >> 31 = -1(0xFFFFFFFF) -->
		return (a + temp) ^ temp;
	}
	/**
	 * 
	 * Reference: HD 2-7 Sign Function
	 * sign(x) = (x >> 31) | (-x >>> 31)
	 * x >>> 31 ==> (x >> 31) & 1
	 * @param a
	 * @param b
	 * @return
	 */
	public static int cmp(int a, int b)
	{
//		return ((a - b) >> 31) | ((b - a) >>> 31);
		
		//Java has the operation >>>, but in the language without >>>,
		//it can be replaced by the below expression
		return ((a - b) >> 31) | (((b - a) >> 31) & 1);		
	}
	/**
	 * Cover the overflow cases
	 * @param a
	 * @param b
	 * @return
	 */
	public static int cmpOF(int a, int b)
	{
		long diff = (long)a - (long)b;
		return (int) ( (diff >> 63) | (-diff >>> 63) );		
	}

	public static void main(String[] args)
	{
		int[][] arrays = { { 1, 1 }, { -2, -2 }, { 0, 0 }, { 10, 20 },
			{ 20, 10 }, { -10, -20 }, { -20, -10 }, { 50, -5 }, { -5, 50 },
			{ Integer.MIN_VALUE, Integer.MIN_VALUE + 100 },
			{ Integer.MAX_VALUE, Integer.MAX_VALUE - 100 },
			{ Integer.MAX_VALUE, Integer.MIN_VALUE },
			{ Integer.MIN_VALUE, Integer.MAX_VALUE }};

		//min
		for(int i = 0; i < arrays.length; i++)
		{
			System.out.printf("min(%d, %d) = %d\n", arrays[i][0], arrays[i][1],
				min(arrays[i][0], arrays[i][1]));
		}
		//max
		for(int i = 0; i < arrays.length; i++)
		{
			System.out.printf("max(%d, %d) = %d\n", arrays[i][0], arrays[i][1],
				max(arrays[i][0], arrays[i][1]));
		}
		//cmp
		for(int i = 0; i < arrays.length; i++)
		{
			System.out.printf("cmp(%d, %d) = %d\n", arrays[i][0], arrays[i][1],
				cmp(arrays[i][0], arrays[i][1]));
		}
		//cmpOF
		for(int i = 0; i < arrays.length; i++)
		{
			System.out.printf("cmpOF(%d, %d) = %d\n", arrays[i][0], arrays[i][1],
				cmpOF(arrays[i][0], arrays[i][1]));
		}
		//abs
		for(int i = 0; i < arrays.length; i++)
		{
			System.out.printf("abs(%d) = %d\n", arrays[i][0], abs(arrays[i][0]));
		}
		
		System.out.printf("abs(%d) = %d\n", Integer.MIN_VALUE, abs(Integer.MIN_VALUE));
		System.out.printf("Math.abs(%d) = %d\n", Integer.MIN_VALUE, Math.abs(Integer.MIN_VALUE));
		System.out.printf("Integer.MIN_VALUE - 1 = %d(%s)\n", Integer.MIN_VALUE - 1, Integer.toHexString(Integer.MIN_VALUE - 1));
	}
}
