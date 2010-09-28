package com.e2u.classloader;

public class LoaderSample1
{
	public static void showClassLoader(String className)
	{
		Class clazz = null;
		ClassLoader classLoader = null;
		String str = null;
		try
		{
			clazz = Class.forName(className);
			classLoader = clazz.getClassLoader();
			str = String.format("ClassLoader = %s  -- for Class[%s]", 
				classLoader, className);
			System.out.println(str);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	public static void main(String[] args)
	{
		Class c;
		ClassLoader cl;
		cl = ClassLoader.getSystemClassLoader();
		System.out.println(cl);
		while(cl != null)
		{
			cl = cl.getParent();
			System.out.println(cl);
		}
		showClassLoader("java.lang.Object");
		showClassLoader("sun.net.spi.nameservice.dns.DNSNameService");
		showClassLoader("com.e2u.classloader.LoaderSample1");
		
		FileClassLoader fcl = new FileClassLoader();
		c = fcl.findClass("Object");
		System.out.println(c);
	}
}
