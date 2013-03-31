package com.e2u.temp;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.ArrayList;
import java.util.TimeZone;

public class StaticInitTest
{
	private static List list = initList();
	
	private static List initList()
	{
		return new ArrayList();
	}
	public static int gcd(int m, int n)
	{
		if(n == 0)
		{
			return m;
		}
		return gcd(n, m % n);
	}
	public static void test1()
	{
		int[] ms = {15, 9, 8, 7, 55, 100, 0, -5, -99, 90};
		int[] ns = {25, 5, 6, 12, 99, 240, 6, 15, -60, -15};
		for(int i = 0; i < ms.length; i++)
		{
			System.out.printf("gcd(%d, %d) = %d\n", ms[i], ns[i], gcd(ms[i], ns[i]));
		}
		System.out.println((byte)-1);
		System.out.println((int)((byte)0x8F));
		System.out.println((int)((short)((byte)0xFF)));
		System.out.println((int)((char)((byte)0xFF)));
		System.out.println((int)((char)((byte)-1)));
		System.out.println((int)((short)((byte)-1)));
		System.out.println((int)(char)(byte)0x7F);
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		ClassLoader loader = StaticInitTest.class.getClassLoader();
		while (loader != null) 
		{
			System.out.println(loader.toString());
			loader = loader.getParent();
		}
	}

}
