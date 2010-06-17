package com.e2u.generic;

import java.lang.reflect.*;
import java.util.*;

public class ParameterizedTypeTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		test1();
	}
	
	public static void test1()
	{
		WrapperClass wc = new WrapperClass();
		HashMap<String, Integer> map = new HashMap<String, Integer>();
		try {
			Field field = WrapperClass.class.getDeclaredField("myObj");
			Type type = field.getGenericType();
			
//			Type type = map.getClass().getGenericSuperclass();
			
			System.out.println("Type=" + type);
			if (type instanceof ParameterizedType)
			{
				ParameterizedType pt = (ParameterizedType) type;
				
				System.out.println("RawType=" + pt.getRawType());
				System.out.println("OwnerType=" + pt.getOwnerType());
				
				Type[] types = pt.getActualTypeArguments();
				if(types != null && types.length > 0)
				{
					for(int i = 0; i < types.length; i++)
					{
						System.out.println("  ActualTypeArgs[" + i + "]=" + types[i]);
					}					
				}				
			}
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
}

class MyObject<T>
{
	private List<T> list = new ArrayList<T>();
	
	public T getFirst()
	{
		if(list.size() > 0)
		{
			return list.get(0);
		}
		return null;
	}
	public void add(T obj)
	{
		list.add(obj);
	}
}

class WrapperClass
{
	private MyObject<List<Map<Integer, String>>> myObj = new MyObject<List<Map<Integer, String>>>();
}
