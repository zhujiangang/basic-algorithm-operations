package com.e2u.hash;

import java.util.*;

public class TestHash
{
	private static int counter = 0;
	private static Map<String, HashAlg> map = new TreeMap<String, HashAlg>();

	private static void init()
	{

		map.put("00-Remain", new HashAlg()
		{
			public int hash(int h)
			{
				return h;
			}
		});

		map.put("01-hx127", new HashAlg()
		{
			public int hash(int h)
			{
				return h - (h << 7); // i.e., -127 * h
			}
		});

		/**
		 * HashMap's supplemental hash function is a simplified version of the
		 * shift-add hash function in the Linux buffer cache. It is described in
		 * CITI Technical Report 00-1,
		 * "Linux Kernel Hash Table Behavior: Analysis and Improvements", by
		 * Chuck Lever
		 * (http://www.citi.umich.edu/techreports/reports/citi-tr-00-1.pdf). The
		 * full version of this hash function is:
		 * 
		 * @param h
		 * @return
		 */
		map.put("02-Linux", new HashAlg()
		{
			public int hash(int h)
			{
				return ((h << 7) - h + (h >>> 9) + (h >>> 17));
			}
		});

		map.put("03-discussion", new HashAlg()
		{
			public int hash(int key)
			{
				key += ~(key << 9);
				key ^= (key >>> 14);
				key += (key << 4);
				key ^= (key >>> 10);
				return key;
			}
		});

		/**
		 * From: HashMap Applies a supplemental hash function to a given
		 * hashCode, which defends against poor quality hash functions. This is
		 * critical because HashMap uses power-of-two length hash tables, that
		 * otherwise encounter collisions for hashCodes that do not differ in
		 * lower bits. Note: Null keys always map to hash 0, thus index 0.
		 */
		map.put("04-HashMap", new HashAlg()
		{
			public int hash(int h)
			{
				// This function ensures that hashCodes that differ only by
				// constant multiples at each bit position have a bounded
				// number of collisions (approximately 8 at default load
				// factor).
				h ^= (h >>> 20) ^ (h >>> 12);
				return h ^ (h >>> 7) ^ (h >>> 4);
			}
		});
	}

	private static void find(Map map, Object key)
	{
		if(map.containsKey(key))
		{
			System.out.println(String.format("Find key=%s OK. Value=%s", key,
				map.get(key)));
		}
		else
		{
			System.out.println(String.format("Find key=%s Failed.", key));
		}
	}

	public static void testHash()
	{
		Hashtable<HashTester, Integer> map = new Hashtable<HashTester, Integer>();

		HashTester ht1 = new HashTester(1, 1);
		HashTester ht2 = new HashTester(1, 2);
		HashTester ht3 = new HashTester(1, 12);

		map.put(ht1, ++counter);
		map.put(ht2, ++counter);
		map.put(ht3, ++counter);

		find(map, ht1); // OK. This key itself is in the map
		find(map, ht2); // OK. Same reason as above
		find(map, ht3); // OK. Same reason as above

		// OK. This key has a same hashCode and equals to an object in the map.
		find(map, new HashTester(1, 2));

		// Failed. There's no object in the map has a same hashCode with this
		// input param
		// Though it's equals to several keys in the map.
		find(map, new HashTester(1, 3));

		// Failed. Same reason as above
		find(map, new HashTester(1, 11));

		// OK. The cloned object has the same hashCode and equals to the
		// original object.
		HashTester tmp = (HashTester) ht1.clone();
		find(map, tmp);
	}

	/**
	 * From: http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4669519
	 */
	private static void testHashAlg()
	{
		final int SIZE = 500;
		final int TABLE_LENGTH = 1024;

		Set s = new HashSet();

		for(Iterator<String> iter = map.keySet().iterator(); iter.hasNext(); )
		{
			String algName = iter.next();
			HashAlg ha = map.get(algName);

			for(int i = 0; i < SIZE; i++)
			{
				int h = new Double(i).hashCode();
				int hPrime = ha.hash(h);
				int bucket = hPrime & (TABLE_LENGTH - 1);
				s.add(new Integer(bucket));
			}
			String output = String.format("Alg Name = %s, size = %d", algName, s.size());
			System.out.println(output);
			s.clear();
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		init();
		// testHash();
		testHashAlg();
	}
}

class HashTester implements Cloneable
{
	private int hash;
	private int id;

	public HashTester(int id, int hash)
	{
		this.id = id;
		this.hash = hash;
	}

	public boolean equals(Object obj)
	{
		if(!(obj instanceof HashTester))
		{
			return false;
		}
		HashTester other = (HashTester) obj;
		return id == other.id;
	}

	public int hashCode()
	{
		return hash;
	}

	public String toString()
	{
		return "{id=" + id + ", hash=" + hash + "}";
	}

	public Object clone()
	{
		HashTester result = null;
		try
		{
			result = (HashTester) super.clone();
		}
		catch(CloneNotSupportedException e)
		{
			// assert false;
			e.printStackTrace();
		}
		return result;
	}
}

interface HashAlg
{
	public int hash(int h);
}
