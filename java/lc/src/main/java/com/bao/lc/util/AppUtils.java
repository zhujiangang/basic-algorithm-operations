package com.bao.lc.util;

import java.io.File;

import com.bao.lc.AppConfig;

public class AppUtils
{
	
	public static String getOutputFilePath(String fileName)
	{
		return AppConfig.getInstance().getOutputDir() + File.separator + fileName;
	}
	
	public static String getTempFilePath(String fileName)
	{
		return AppConfig.getInstance().getTempDir() + File.separator + fileName;
	}
	
	public static String getUserFilePath(String fileName)
	{
		return AppConfig.getInstance().getUserDir() + File.separator + fileName;
	}
	
	public static String digitToCNWord(int dayOfWeek)
	{
		if(dayOfWeek < 1 || dayOfWeek > 7)
		{
			throw new IllegalArgumentException("dayOfWeek out of range (1-7).");
		}
		String wordTable = AppConfig.getInstance().getPropInternal("common.dayOfWeek.CN.words");
		if(wordTable.length() < 7)
		{
			throw new IllegalStateException("dayOfWeek word table length too short.");
		}
		
		return String.valueOf(wordTable.charAt(dayOfWeek - 1));
	}
	
	public static String letterToCNWord(char c)
	{
		if(c == 'a')
		{
			return AppConfig.getInstance().getPropInternal("common.am");
		}
		else if(c == 'b')
		{
			return AppConfig.getInstance().getPropInternal("common.pm");
		}
		return null;
	}
	
	public static String getDayOfWeekTextCN1(int dayOfWeek)
	{
		return AppConfig.getInstance().getPropInternal("common.week.CN1") + digitToCNWord(dayOfWeek);
	}
	
	public static String getDayOfWeekTextCN1(String dayOfWeek)
	{
		if(dayOfWeek == null || dayOfWeek.isEmpty())
		{
			throw new IllegalArgumentException("Empty dayOfWeek string");
		}
		char c = dayOfWeek.charAt(0);
		if(!Character.isDigit(c))
		{
			throw new IllegalArgumentException("The char at index(0) is not digit");
		}
		String result = getDayOfWeekTextCN1(Character.digit(c, 10));
		
		if(dayOfWeek.length() > 1)
		{
			c = dayOfWeek.charAt(1);
			result += letterToCNWord(c);
		}
		
		return result;
	}
}
