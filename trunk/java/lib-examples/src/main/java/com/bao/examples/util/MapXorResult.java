package com.bao.examples.util;

import java.util.Map;
import java.util.TreeMap;

public class MapXorResult<K, V>
{
	private Map<K, V> only1;
	private Map<K, V> only2;

	public MapXorResult()
	{
		only1 = new TreeMap<K, V>();
		only2 = new TreeMap<K, V>();
	}

	public Map<K, V> getOnly1()
	{
		return only1;
	}

	public Map<K, V> getOnly2()
	{
		return only2;
	}

	public MapRelation getRelation()
	{
		if(only1.isEmpty() && only2.isEmpty())
		{
			return MapRelation.EQUALS;
		}

		if(only1.isEmpty())
		{
			return MapRelation.LESS;
		}

		if(only2.isEmpty())
		{
			return MapRelation.GREATER;
		}

		return MapRelation.INTERSECTION;
	}
	
	@Override
	public String toString()
	{
		StringBuilder sb = new StringBuilder();
		sb.append("[Only1]: ").append(only1);
		sb.append(", [Only2]: ").append(only2);
		return sb.toString();
	}
}
