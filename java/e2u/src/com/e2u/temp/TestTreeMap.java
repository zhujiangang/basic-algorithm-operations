package com.e2u.temp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import java.util.TreeMap;
import java.util.Map;

public class TestTreeMap
{
	private TreeMap<Integer, Integer> map = new TreeMap<Integer, Integer>();

	public TestTreeMap()
	{
		init();
	}
	
	private void init()
	{
		int count = 10;
		for(int i = 0; i < count; i++)
		{
			map.put(Integer.valueOf(i + 1), 1);
		}
	}
	
	private void test()
	{
		for(Iterator<Map.Entry<Integer, Integer>> iter = map.entrySet().iterator(); iter.hasNext(); )
		{
			Map.Entry<Integer, Integer> entry = iter.next();
			System.out.println(entry.getKey());
			Integer key = entry.getKey();
			if(entry.getKey().intValue() == 5)
			{
				System.out.printf("Lower key = %d, Higher key = %d\n", map.lowerKey(key), map.higherKey(key));
			}
		}
	}
	private void test2()
	{
		List<Integer> list = new ArrayList<Integer>();
		
		for(int i = 0, size = 10; i < size; i++)
		{
			list.add(i + 1);
		}
		System.out.println();
		System.out.println(list);
		System.out.println();
		for(int i = 0, size = list.size(); i < size; i++)
		{
			System.out.printf("d=%d, list=%s\n", i, list.toString());			
			if(i == 5)
			{
				Collections.rotate(list, i);
				break;
			}
			Collections.rotate(list, -1);	
			
			//restore
//			list.clear();
//			for(int j = 0; j < 10; j++)
//			{
//				list.add(j + 1);
//			}
		}
		System.out.println();
		System.out.println(list);
		System.out.println();
//		System.out.println(list);
//		for(int i = 90; i >= 0; i--)
//		{
//			Collections.rotate(list, -i);
//			System.out.printf("d=%d, list=%s\n", -i, list.toString());
//			
//			//restore
//			list.clear();
//			for(int j = 0; j < 10; j++)
//			{
//				list.add(j + 1);
//			}
//		}
//		System.out.println();
	}
	
	
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		TestTreeMap ttm = new TestTreeMap();
		//ttm.test();
		ttm.test2();
	}

}
