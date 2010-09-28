package com.e2u.grammar;

public class PrimitiveTypeTest
{
	public static void testShort()
	{
		short s1 = 1; 
		//Error: Type mismatch: cannot convert from int to short
		//s1 = s1 + 1;
		
		short s2 = 1; 
		s2 += 1;
		
		printSeparator();
	}
	
	public static void testChar()
	{
		byte b = (byte)-1;
		
		//char range: '\u0000' - '\uffff' (0 -- 2^16-1)
		System.out.println((int)(char)b);
		
		//short range: (-2^15 -- 2^15-1) (-32768 -- 32767)
		System.out.println((int)(short)b);
		printSeparator();
	}
	
	public static void testMisc()
	{
		System.out.println(Math.round(11.5));
		System.out.println(Math.round(-11.5));
		
		System.out.println(Math.round(11));
		System.out.println(Math.round(-11));
		
		System.out.println(Math.round(11.4999));
		System.out.println(Math.round(-11.4999));
		printSeparator();
	}
	public static void printSeparator()
	{
		System.out.println("===========================");
	}
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		testShort();
		testChar();
		testMisc();
	}

}
