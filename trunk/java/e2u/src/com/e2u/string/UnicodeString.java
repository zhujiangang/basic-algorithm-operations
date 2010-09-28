package com.e2u.string;

import java.io.UnsupportedEncodingException;

public class UnicodeString
{
	public static int getLastLetterIndex(byte[] bytes, int end)
	{
		if(end == 0)
		{
			return 0;
		}
		int index = -1;
		for(index = end - 1; index >= 0; index--)
		{
			if(bytes[index] > 0)
			{
				break;
			}
		}
		if(index < 0)
		{
			index = 0;
		}
		return index;
	}
	public static String substring(String str, int beginIndex, int endIndex) throws UnsupportedEncodingException
	{
		byte[] bytes = str.getBytes("GBK");
		int index = -1;
		if(beginIndex > 0)
		{
			index = getLastLetterIndex(bytes, beginIndex);
			if( (beginIndex - 1 - index) % 2 == 1)
			{
				beginIndex--;
			}
		}
		index = getLastLetterIndex(bytes, endIndex);
		if( (endIndex - 1 - index) % 2 == 0)
		{
			endIndex--;
		}
		
		String ret = new String(bytes, beginIndex, endIndex - beginIndex + 1, "GBK");
		return ret;
	}
	public static void test()
	{
		String str = "我国123与他国3";
		try
		{
			System.out.println(substring(str, 0, 7));
		}
		catch(Exception e)
		{
			// TODO: handle exception
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
