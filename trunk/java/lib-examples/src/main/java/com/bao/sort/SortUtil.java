package com.bao.sort;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;
import java.util.TreeMap;
public class SortUtil
{
	public static boolean ENABLE_LOG = false;
	
	public final static int INSERT = 1;

	public final static int BUBBLE = 2;

	public final static int SELECTION = 3;

	public final static int SHELL = 4;

	public final static int QUICK = 5;

	public final static int IMPROVED_QUICK = 6;

	public final static int MERGE = 7;

	public final static int IMPROVED_MERGE = 8;

	public final static int HEAP = 9;
	
	public final static int DSQSORT = 10;
	
	public final static int JDKQSORT = 11;
	
	public final static int TESTQSORT = 12;
	
	public final static int STACKQSORT = 13;
	
	public final static int TREEQSORT = 14;
	
	public static int SORT_IMPL = TESTQSORT;

	public static void sort(int[] data)
	{
		if(ENABLE_LOG)
		{
			print(data);
		}
		sort(data, SORT_IMPL);
	}

	private static String[] name = { "insert", "bubble", "selection", "shell",
			"quick", "improved_quick", "merge", "improved_merge", "heap", "dsqsort",
			"JDKQSort", "TestQSort", "StackQSort", "TreeSort"};

	private static Sort[] impl = new Sort[] {
	            new InsertSort(),
	            new BubbleSort(),
	            new SelectionSort(),
	            new ShellSort(),
	            new QuickSort(),
	            new ImprovedQuickSort(),
	            new MergeSort(),
	            new ImprovedMergeSort(),
	            new HeapSort(),
	            new DSQSort(),
	            new JDKQSort(),
	            new TestQSort(),
	            new StackQSort(),
	            new TreeSort()
	};

	public static String toString(int algorithm)
	{
		return name[algorithm];
	}

	public static void sort(int[] data, int algorithm)
	{
		impl[algorithm - 1].sort(data);
	}

	public static interface Sort
	{
		public void sort(int[] data);
	}

	public static void swap(int[] data, int i, int j)
	{
		int temp = data[i];
		data[i] = data[j];
		data[j] = temp;
	}
	
	public static void log(String format, Object ... args)
	{
		if(ENABLE_LOG)
		{
			System.out.printf(format, args);
		}
	}
	
	public static void logSwap(int x[], int a, int b)
	{
		if(ENABLE_LOG)
		{
			System.out.printf("[SWAP]: a[%2d]=%2d, a[%2d]=%2d ==> a[%2d]=%2d, a[%2d]=%2d\n", 
				a, x[a], b, x[b], a, x[b], b, x[a]);
		}
	}
	
	public static void logIndexAssign(int x[], int a, int b)
	{
		if(ENABLE_LOG)
		{
			System.out.printf("[=IDX]: a[%2d]=%2d, a[%2d]=%2d ==> a[%2d]=%2d, a[%2d]=%2d\n", 
				a, x[a], b, x[b], a, x[b], b, x[b]);
		}
	}
	public static void logPartitionStart(int x[], int a, int b, int pivot)
	{
		if(ENABLE_LOG)
		{
			System.out.printf("[PTTA]: [%2d-%2d] - pivot=%2d: ", a, b, pivot);
			print(x, a, b);
		}
	}
	public static void logPartition(int x[], int a, int b, int k, int high)
	{
		if(ENABLE_LOG)
		{
			System.out.printf("[PART] - [%2d-%2d]: lo=%2d, hi=%2d, k=%2d: ", a, b, k, high, k);
			print(x, a, b);
			
			System.out.printf("[FULL] - [%2d,%2d]:", 0, x.length - 1);
			print(x);
		}
	}
	
	public static void logValueAssign(int x[], int a, int val)
	{
		if(ENABLE_LOG)
		{
			System.out.printf("[=VAL]: a[%2d]=%2d ==> a[%2d]=%2d\n", 
				a, x[a], a, val);
		}
	}
	
	/**
     * Returns the index of the median of the three indexed values.
     */
	public static int mid3(int x[], int a, int b, int c)
	{
		return (x[a] < x[b] ?
			(x[b] < x[c] ? b : x[a] < x[c] ? c : a) :
			(x[b] > x[c] ? b : x[a] > x[c] ? c : a));
	}
	
	public static boolean checkValid(int[] array)
	{
		for(int i = 0; i < array.length - 1; i++)
		{
			if(array[i + 1] < array[i])
			{
				return false;
			}
		}
		return true;
	}
	
	public static void print(int[] array)
	{
		print(array, 0, array.length - 1);
	}
	
	public static void print(int[] array, int a, int b)
	{
		System.out.println(format(array, a, b));
	}
	
	private static String format(int[] a, int x, int y)
	{
		StringBuilder sb = new StringBuilder();
		for(int i = x; i <= y; i++)
		{
			if(i > 0)
			{
				sb.append(" ");
			}
			sb.append(a[i]);
		}
		return sb.toString();
	}
	
	private static void addInMap(int[] a, Map<Integer, Integer> map)
	{
		int val;
		for(int i = 0; i < a.length; i++)
		{
			if(map.containsKey(a[i]))
			{
				val = map.get(a[i]);
				val++;
			}
			else
			{
				val = 1;
			}
			map.put(a[i], val);
		}
	}
	
	public static void test(int[] a, boolean show)
	{		
		Map<Integer, Integer> srcMap = new HashMap<Integer, Integer>();
		addInMap(a, srcMap);	
		
		int[] bak = new int[a.length];
		System.arraycopy(a, 0, bak, 0, a.length);
		
		sort(a);
		
		Map<Integer, Integer> dstMap = new HashMap<Integer, Integer>();
		addInMap(a, dstMap);
		
		boolean b = false;
		if(!srcMap.equals(dstMap))
		{
			System.out.print("X - ");
		}
		else
		{
			b = checkValid(a);
			if(b)
			{
				System.out.print("Y - ");
			}
			else
			{
				System.out.print("N - ");
			}
		}
		
		if(show)
		{
			String str = null;
			if(b)
			{
				str = format(a, 0, a.length - 1);		
			}
			else
			{
				str = format(bak, 0, bak.length - 1);
				str += " ==> ";
				str += format(a, 0, a.length - 1);		
			}
			System.out.println(str);
		}
		else
		{
			System.out.println();
		}
	}
	
	public static void main1()
	{
		final int MAX = 20;
		Random rand=new Random();
		int[] arrays = new int[MAX];
		for(int i = 0; i <impl.length; i++)
		{
			System.out.println(SortUtil.toString(i));
			for(int k = 0; k < MAX; k++)
			{
				arrays[k] = rand.nextInt(MAX);
				System.out.print(arrays[k]);
				System.out.print(",");
			}
			System.out.println();
			System.out.println("##########################################");
			impl[i].sort(arrays);
			for (int j = 0; j < MAX; j++)
			{
				System.out.print(arrays[j]);
				System.out.print(",");
			}
			System.out.println();
			System.out.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
			System.out.println();
			System.out.println();
			System.out.println();
		}
	}
	
	public static void main2()
	{
		SORT_IMPL = TESTQSORT;
		int[][] a = 
			{
			{6, 7, 8},
			{49, 38, 65, 97, 76, 13, 27, 49},
			{72, 6, 57, 88, 85, 42, 83, 73, 48, 60}
			
			};
		for(int i = 0; i < a.length; i++)
		{
			test(a[i], true);
		}
		
		final int MAX = 20;
		Random rand=new Random();
		int[] arrays = new int[MAX];
		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < MAX; j++)
			{
				arrays[j] = rand.nextInt(1000);
			}
			test(arrays, true);
		}
		System.out.println("Done");
	}
	
	public static void main3()
	{
		ENABLE_LOG = true;
		SORT_IMPL = TREEQSORT;
		
		int[][] a = 
			{
			{49, 38, 65, 97, 76, 13, 27, 49},
//			{72, 6, 57, 88, 85, 42, 83, 73, 48, 60}
			};
		for(int i = 0; i < a.length; i++)
		{
			test(a[i], true);
		}
	}
	public static void main(String[] args)
	{
		main3();
	}
}
