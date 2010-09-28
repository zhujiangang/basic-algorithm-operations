package com.e2u.grammar;

public class InheritanceTest
{
	public static void output(char x)
	{
		System.out.println(x);
	}
	public static void main(String[] args)
	{
		Base b = new SubClass1();
		System.out.println((b.method2('x')));
	}
}

class Base
{
	public Base()
	{
		
	}
	public void method1(int i, char c)
	{
		
	}
	public char method1(int i, char c, short s)
	{
		return ' ';
	}
	
	public short method2(Object obj, int i)
	{
		return (short)i;
	}
	protected int method2(int x)
	{
		return x;
	}
	protected native int method2(char c);
}

class SubClass1 extends Base
{
	public short method2(Object obj, int i)
	{
		return (short)i;
	}
	public synchronized int method2(int x)
	{
		return x;
	}
	public synchronized int method2(char c)
	{
		return c;
	}
	public static synchronized native void method3();
}
