package com.e2u.grammar;

public class ExceptionTest
{
	// @SuppressWarnings("finally")
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

	private static void test1() throws CustomedException
	{
		CustomedException exception = null;

		try
		{
			throwException(209);
		}
		catch(CustomedException e)
		{
			exception = e;
		}
		finally
		{
			// release the lock now
			System.err.println("finally goes here");
		}
		if(exception != null)
		{
			throw exception;
		}
	}

	private static void throwException(int x) throws CustomedException
	{
		if(x < 100)
		{
			throw new CustomedException("The x is lower than 100, x = " + x);
		}
		if(x > 200)
		{
			throw new IllegalArgumentException(
				"The x is greater than 200, x = " + x);
		}

		System.out.println("In normal block, x = " + x);
	}

	public static void main(String[] args)
	{
//		System.out.println(testTry());
		
		try
		{
			test1();
		}
		catch(CustomedException e)
		{
			e.printStackTrace();
		}		
	}
}

class CustomedException extends Exception
{
	public CustomedException()
	{
		super();
	}

	public CustomedException(String message)
	{
		super(message);
	}
}
