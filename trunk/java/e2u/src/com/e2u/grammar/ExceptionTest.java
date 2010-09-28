package com.e2u.grammar;

public class ExceptionTest
{
	@SuppressWarnings("finally")
	public static int testTry()
	{
		int result = -1;
		try
		{
			result += 101;
			return result;
		}
		catch(Exception e)
		{
			result += 200;
			return result;
		}
		finally
		{
			result += 300;
			return 10;
		}
	}
	public static void main(String[] args)
	{
		System.out.println(testTry());
	}
}
