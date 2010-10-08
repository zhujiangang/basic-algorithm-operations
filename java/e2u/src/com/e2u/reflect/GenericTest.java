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
			
			Object obj = clazz.getDeclaredMethods();
			
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
				
				if(Modifier.isStatic(classes[i].getModifiers()))
				{
				    constructor = classes[i].getConstructor();
				    obj = constructor.newInstance();
				}
				else
				{
				    constructor = classes[i].getConstructor(innerClassConParamTypes);
				    obj = constructor.newInstance(tmp);
				}				
				
				Class tempClass = classes[i].getEnclosingClass();
				Object[] signers = classes[i].getSigners();
				
				method = classes[i].getDeclaredMethod("getData", parameterTypes);
				method.setAccessible(true);
				Object result = method.invoke(obj, parameters);
				System.out.println(result);
				
                method = classes[i].getDeclaredMethod("setData", parameterTypes);
                method.setAccessible(true);
                result = method.invoke(obj, parameters);
                System.out.println(result);
                
                if(Modifier.isStatic(classes[i].getModifiers()))
                {
                    Field filed = classes[i].getDeclaredField("localClass");
                    result = filed.get(obj);
                    System.out.println(result);
                    
                    tempClass = (Class)result;
                    if(tempClass.isLocalClass())
                    {
                        System.out.println("IsLocalClass");
                    }
                    System.out.println(tempClass.getEnclosingMethod());
                    System.out.println(tempClass.getEnclosingConstructor());
                    System.out.println(tempClass.getEnclosingClass());
                    
                    filed = classes[i].getDeclaredField("anonymousInnerClass");
                    result = filed.get(obj);
                    System.out.println(result); 
                    
                    tempClass = (Class)result;
                    if(tempClass.isAnonymousClass())
                    {
                        System.out.println("isAnonymousClass");
                    }
                    System.out.println(tempClass.getEnclosingMethod());
                    System.out.println(tempClass.getEnclosingConstructor());
                    System.out.println(tempClass.getEnclosingClass());
                }

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
		
		public Class localClass = null;
		public Class anonymousInnerClass = null;
		
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
		
		public void setData(int x)
		{
		    final StringBuffer strBuf = new StringBuffer();
		    Runnable runnable = new Runnable()
		    {
		        public void run()
		        {
		            strBuf.append(System.currentTimeMillis());
		        }
		    };
		    
		    if(anonymousInnerClass == null)
		    {
		        anonymousInnerClass = runnable.getClass();
		    }
		    runnable.run();
		    
		    
		    class LocalClass implements Runnable
		    {
		        public void run()
                {
                    strBuf.append(System.currentTimeMillis());
                }
		    }
		    
		    if(localClass == null)
            {
		        localClass = LocalClass.class;
            }
		    data = (int)(Long.parseLong((strBuf.toString()) + x));
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
		public void setData(int x)
		{
		}
		private int staticMethod(int y)
		{
			return y + 100;
		}
	}
}
