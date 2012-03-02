package com.bao.lc.util;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import java.util.TimeZone;

import junit.framework.TestCase;

public class CalendarTest extends TestCase
{
	private static final String DATE_PATTERN = "yyyy-MM-dd HH:mm:ss.SSS Z";
	private static final DateFormat dateFormat = new SimpleDateFormat(DATE_PATTERN);
	
	public void testCalendar()
	{
		Calendar calNow = Calendar.getInstance();
		System.out.println(calNow);
		
		Date dateNow = calNow.getTime();
		System.out.println(dateFormat.format(dateNow));
		
		Locale defaultLocale = Locale.getDefault();
		System.out.println(defaultLocale);
		
		TimeZone defaulTimeZone = TimeZone.getDefault();
		System.out.println(defaulTimeZone);
		
		for(int i = 1; i <= 5; i++)
		{
			for(int j = 1; j <= 6; j++)
			{
				updateCalendar(calNow, i, j);
			}
		}
	}
	
	public void updateCalendar(Calendar cal, int week, int dayOfWeek)
	{		
		cal.add(Calendar.WEEK_OF_YEAR, week - 1);
//		cal.set(Calendar.DAY_OF_WEEK, convertDayOfWeek(dayOfWeek));
		
		System.out.println(dateFormat.format(cal.getTime()));
	}
	
	public static int convertDayOfWeek(int x)
	{
		if(x < 1 || x > 7)
		{
			throw new IllegalArgumentException("Out of range 1-7");
		}
		if(x == 7)
		{
			return Calendar.SUNDAY;
		}
		return x + 1;
	}
}
