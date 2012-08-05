package com.bao.sort;
import java.util.Random;
public class SortUtil
{
	public final static int INSERT = 1;

	public final static int BUBBLE = 2;

	public final static int SELECTION = 3;

	public final static int SHELL = 4;

	public final static int QUICK = 5;

	public final static int IMPROVED_QUICK = 6;

	public final static int MERGE = 7;

	public final static int IMPROVED_MERGE = 8;

	public final static int HEAP = 9;
	
	public final static int DS_QSORT = 10;
	
	public final static int JDKQSORT = 11;
	
	public final static int TESTQSORT = 12;

	public static void sort(int[] data)
	{
		sort(data, TESTQSORT);
	}

	private static String[] name = { "insert", "bubble", "selection", "shell",
			"quick", "improved_quick", "merge", "improved_merge", "heap", "dsqsort",
			"JDKQSort", "TestQSort"};

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
	            new TestQSort()
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
		for(int i = 0; i < array.length; i++)
		{
			if(i > 0)
			{
				System.out.print(" ");
			}
			System.out.print(array[i]);
		}
		System.out.println();
	}
	
	public static void test(int[] a, boolean show)
	{
		sort(a);
		boolean b = checkValid(a);
		if(b)
		{
			System.out.print("Y ");
		}
		else
		{
			System.out.print("N ");
		}
		if(show)
		{
			print(a);
		}
		else
		{
			System.out.println();
		}
	}
	
	private static void main1()
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
		int[][] a = 
			{
			{},
			{5},
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
				arrays[j] = rand.nextInt(100);
			}
			test(arrays, true);
		}
	}
	public static void main(String[] args)
	{
		main2();
	}
}
