package com.bao.sort;

import com.bao.sort.SortUtil.Sort;

public class DSQSort implements Sort
{

	@Override
	public void sort(int[] data)
	{
		sort(data, 0, data.length - 1);
	}
	
	private void sort(int[] data, int low, int high)
	{
		if(low < high)
		{
			int k = partition(data, low, high);
			sort(data, low, k - 1);
			sort(data, k + 1, high);
		}
	}

	private int partition(int[] data, int low, int high)
	{
		int t = data[low];
		
		int pivot = data[low];
		
		while(low < high)
		{
			while(low < high && data[high] >= pivot)
				--high;
			data[low] = data[high];
			
			while(low < high && data[low] <= pivot)
				++low;
			data[high] = data[low];
		}
		
		data[low] = t;
		return low;
	}
}
