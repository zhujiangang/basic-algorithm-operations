package com.bao.lc.util;

import java.util.Map;

public class MapOperation<K, V>
{
	public static final int XOR_KEY_VALUE = 0;
	public static final int XOR_KEY_ONLY = 1;

	private Map<K, V> map1 = null;
	private Map<K, V> map2 = null;

	public MapOperation(Map<K, V> map1, Map<K, V> map2)
	{
		this.map1 = map1;
		this.map2 = map2;
	}

	public MapXorResult<K, V> xor()
	{
		return xor(XOR_KEY_VALUE);
	}

	public MapXorResult<K, V> xor(int flag)
	{
		MapXorResult<K, V> result = new MapXorResult<K, V>();

		// Both empty
		if(isEmpty(map1) && isEmpty(map2))
		{
			return result;
		}

		// map1 is empty, map2 not empty
		if(isEmpty(map1))
		{
			result.getOnly2().putAll(map2);
			return result;
		}

		// map1 not empty, map2 empty
		if(isEmpty(map2))
		{
			result.getOnly1().putAll(map1);
			return result;
		}

		// Real work, both not empty
		findOnly1(map1, map2, result.getOnly1(), result.getOnly2(), flag);
		findOnly1(map2, map1, result.getOnly2(), result.getOnly1(), flag);

		return result;
	}

	private void findOnly1(Map<K, V> map1, Map<K, V> map2, Map<K, V> only1, Map<K, V> only2,
		int flag)
	{
		// Real work, both not empty
		for(K key1 : map1.keySet())
		{
			if(!map2.containsKey(key1))
			{
				only1.put(key1, map1.get(key1));
				continue;
			}

			if(flag == XOR_KEY_VALUE)
			{
				V value1 = map1.get(key1);
				V value2 = map2.get(key1);
				if(!equals(value1, value2))
				{
					only1.put(key1, value1);
				}
			}
		}
	}

	private static boolean isEmpty(Map<?, ?> map)
	{
		return (map == null || map.isEmpty());
	}

	private static boolean equals(Object object1, Object object2)
	{
		if(object1 == object2)
		{
			return true;
		}
		if((object1 == null) || (object2 == null))
		{
			return false;
		}
		return object1.equals(object2);
	}
}
