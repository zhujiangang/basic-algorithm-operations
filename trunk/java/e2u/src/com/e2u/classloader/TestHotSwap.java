package com.e2u.classloader;

import java.lang.reflect.Method;
import java.net.URL;

public class TestHotSwap
{
	public static void main(String args[]) throws Exception
	{
		A a = new A();
		B b = new B();
		a.setB(b);

		System.out.println(String.format("A classLoader is %s", a.getClass()
			.getClassLoader()));
		System.out.println(String.format("B classLoader is %s", b.getClass()
			.getClassLoader()));
		System.out.println(String.format("A.b classLoader is %s", a.getB().getClass()
			.getClassLoader()));

		HotSwapClassLoader c1 = new HotSwapClassLoader(new URL[] { new URL(
			"file:\\C:\\Temp\\") }, a.getClass().getClassLoader());
		Class clazz = c1.load("javax.swing.JTable");
		javax.swing.JTable aInstance = (javax.swing.JTable)clazz.newInstance();
		
		Class clazz2 = c1.load("javax.swing.JTable");
		javax.swing.JTable aInstance2 = (javax.swing.JTable)clazz2.newInstance();
		
		aInstance = aInstance2;

		Method method1 = clazz.getMethod("setB", B.class);
		method1.invoke(aInstance, b);

		Method method2 = clazz.getMethod("getB", null);
		Object bInstance = method2.invoke(aInstance, null);
		
		AnonymousInf a2 = (AnonymousInf)aInstance;
		System.out.println(a2.getClass().getClassLoader());
		a2.setB(b);

		System.out.println(String.format("reloaded A.b classLoader is %s", bInstance
			.getClass().getClassLoader()));
	}
}
