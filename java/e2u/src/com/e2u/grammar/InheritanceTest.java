package com.e2u.grammar;

public class InheritanceTest
{
	public static void output(char x)
	{
		System.out.println(x);
	}
	public static void main(String[] args)
	{
		Base a = new Base();
		System.out.println((a.method2(1)));
		
		Base b = new SubClass1();
		System.out.println((b.method2(1)));
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
		return x + cal(100, 50);
	}
	protected native int method2(char c);
	
	int cal(int a, int b)
	{
		return 0;
	}
	int cal2(int x)
	{
		return x*2;
	}
}

class SubClass1 extends Base
{
	public short method2(Object obj, int i)
	{
		return (short)i;
	}
	public synchronized int method2(char c)
	{
		return c + cal2(10);
	}
	public static synchronized native void method3();
	int cal(int a, int b)
	{
		return (int)(a + b);
	}
}
