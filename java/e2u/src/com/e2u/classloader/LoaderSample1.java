package com.e2u.classloader;

import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.List;

public class LoaderSample1
{
	public static void testClassLoaderHierarchy()
	{
		ClassLoader cl = null;
		for(cl = ClassLoader.getSystemClassLoader(); ; cl = cl.getParent())
		{
			System.out.print(cl);
			
			if(cl == null)
			{
				break;
			}
			
			System.out.print(" ==> ");
		}
		System.out.println();
	}
	
	public static void testShowClassLoader()
	{
		showClassLoader("java.lang.Object");
		showClassLoader("sun.net.spi.nameservice.dns.DNSNameService");
		showClassLoader(LoaderSample1.class.getName());
	}
	
	public static void showClassLoader(String className)
	{
		Class clazz = null;
		ClassLoader classLoader = null;
		String str = null;
		try
		{
			clazz = Class.forName(className);
			classLoader = clazz.getClassLoader();
			str = String.format("ClassLoader=%s  -- Class[%s]", 
				getClassName(classLoader), className);
			System.out.println(str);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	private static String getClassName(Object obj)
	{
		if(obj == null)
		{
			return null;
		}
		return obj.getClass().getName();
	}
	
	
	public static void testCustomClassLoader()
	{
		String classDir = "C:\\Temp\\bin";
		FileSystemClassLoader fscl = new FileSystemClassLoader(classDir);
		
		List<String> classNameList = new ArrayList<String>();
		
		classNameList.add("javax.swing.JTable");
		classNameList.add("java.lang.Void");
		classNameList.add("java.lang.Object");
		classNameList.add(LoaderSample1.class.getName());
		
		Class clazz = null;
		Object obj = null;
		for(int i = 0, size = classNameList.size(); i < size; i++)
		{
			
			try
			{
				clazz = loadClass(classNameList.get(i), fscl);
				if(clazz == null)
				{
					continue;
				}
				obj = clazz.newInstance();
				System.out.println(obj);
			}
			catch(Exception e)
			{
				System.out.println("class " + classNameList.get(i) + " load exception: " + e.toString());
			}			
		}	
	}
	
	private static Class loadClass(String name, ClassLoader classLoader)
	{
		try
		{
			return classLoader.loadClass(name);
		}
		catch(Exception e)
		{
			System.err.println("class " + name + " load exception: " + e.toString());
		}
		return null;
	}
	
	
	
	public static void main(String[] args)
	{
		testClassLoaderHierarchy();
		testShowClassLoader();
		testCustomClassLoader();
	}
}
