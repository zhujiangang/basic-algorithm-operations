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
		/*
		System.out.println(StaticInitTest.list);
		Calendar cal = Calendar.getInstance();
		System.out.println(cal);
		String time = null;
		
//		TimeZone tz = cal.getTimeZone();
//		int offset = tz.getRawOffset();
//		offset = offset / (1000 * 60 * 60);
//		System.out.println(offset);
//		Calendar tzCal = Calendar.getInstance(tz);
//		String   time   =   tzCal.getTime().toString();
//		System.out.println(time);//输出为GMT时间		
		
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		time = formatter.format(cal.getTime());
		System.out.println(time);
		
		Date date = null; 
		
		try
		{
			date = formatter.parse(time);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		System.out.println(date);
		time = formatter.format(date);
		System.out.println(time);
		*/
		/*
		double d = 456.897456114d;
		
		String str = String.format("%.5f", d);
		System.out.println(str);
		
		str = String.format("%.1d", d);
		System.out.println(str);
		*/
		test1();
	}

}
