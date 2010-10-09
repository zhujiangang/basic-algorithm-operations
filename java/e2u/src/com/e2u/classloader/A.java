package com.e2u.classloader;

public class A implements AnonymousInf
{
	private B b;

	public void setB(B b)
	{
		this.b = b;
//		System.out.println("In setB: " + b);
	}

	public B getB()
	{
//		System.out.println("In getB: " + b);
		return b;		
	}
}
