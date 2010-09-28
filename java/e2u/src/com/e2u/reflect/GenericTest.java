package com.e2u.reflect;

import java.lang.reflect.*;

public class GenericTest
{
	public static void test()
	{
		ReflectData rd = new ReflectData();
		
		try
		{
			Class clazz = rd.getClass();
			Constructor constructor = clazz.getConstructor(null);
			
			ReflectData tmp = (ReflectData)constructor.newInstance(null);
			
			Class[] parameterTypes = {int.class};
			Method method = clazz.getDeclaredMethod("method1", parameterTypes);
			
			Object[] parameters = {new Integer(5)};
			method.setAccessible(true);
			Object ret = method.invoke(tmp, parameters);
			
			System.out.println(tmp.a);
			
			Field field = clazz.getDeclaredField("c");
			field.setAccessible(true);
			field.setInt(tmp, 10);
			System.out.println(tmp.getC());
			
			Class[] classes = clazz.getDeclaredClasses();
			
			Class[] innerClassConParamTypes = {clazz};
			for(int i = 0; i < classes.length; i++)
			{
				System.out.println(classes[i]);
				
				constructor = classes[i].getConstructor(innerClassConParamTypes);
				
				Object obj = constructor.newInstance(tmp);
				
				method = classes[i].getMethod("getData", parameterTypes);
				
				method.invoke(obj, parameters);
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
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

class ReflectData
{
	public int a;
	protected int b;
	private int c;
	public ReflectData()
	{
		a = 0;
		b = 0;
		c = 0;
	}
	
	private void method1(int x)
	{
		a = x;
	}
	public int getC()
	{
		return c;
	}
	
	private static int staticMethod1(int x)
	{
		return x * x;
	}
	private static class StaticInnerClass
	{
		private int data;
		
		public StaticInnerClass()
		{
			data = 0;
		}
		
		private int getData(int inc)
		{
			data += inc;
			return data;
		}
		
		private static int staticMethod(int y)
		{
			return y + 100;
		}
	}
	
	private class InnerClass
	{
		private int data;
		
		public InnerClass()
		{
			data = 0;
		}
		
		private int getData(int inc)
		{
			data -= inc;
			return data;
		}
		
		private int staticMethod(int y)
		{
			return y + 100;
		}
	}
}
