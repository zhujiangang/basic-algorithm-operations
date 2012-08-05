package com.bao.sort;

import com.bao.sort.SortUtil.Sort;

public class TestQSort implements Sort
{

	@Override
	public void sort(int[] data)
	{
		sort(data, 0, data.length - 1);
	}
	
	private static void sort(int[] array, int i, int j)
	{
		if(i >= j)
		{
			return;
		}
		int pivotIndex = i + (j - i)/20;
		
		//stick pivot at end
		SortUtil.swap(array, pivotIndex, j);
		
		//k wiil be the first position in the right subarray
		int k = partition(array, i, j - 1, array[j]);
		
		SortUtil.swap(array, k, j);
		
		if((k - i) > 1)
		{
			sort(array, i, k - 1);
		}
		if((j - k) > 1)
		{
			sort(array, k + 1, j);
		}
	}
	
	public static int partition(int[] array, int i, int j, int pivot)
	{
		/*
		if(i == j)
		{
			if(array[i] <= pivot)
			{
				return i + 1;
			}
			else
			{
				return i;
			}
		}
		*/
		int lo = i, hi = j;
		while(lo <= hi)
		{
			while(lo <= hi && array[lo] < pivot)
			{
				lo++;
			}
			while(lo <= hi && array[hi] > pivot)
			{
				hi--;
			}
			if(lo <= hi)
			{
				SortUtil.swap(array, lo, hi);
				lo++;
				hi--;
			}
		}
		return lo;
	}
}
