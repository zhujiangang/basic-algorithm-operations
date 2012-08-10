package com.bao.sort;

import java.util.Stack;

import com.bao.sort.SortUtil.Sort;

public class StackQSort implements Sort
{
	private static class Range
	{
		public int x;
		public int y;
		public Range(int x, int y)
		{
			this.x = x;
			this.y = y;
		}
	}
	@Override
	public void sort(int[] data)
	{
		sort(data, 0, data.length - 1);
	}
	
	public void sort(int[] data, int low, int high)
	{
		if(low >= high)
		{
			return;
		}
		Stack<Range> stack = new Stack<Range>();
		Range range = new Range(low, high);
		stack.push(range);
		
		int i, j, k;
		
		while(!stack.isEmpty())
		{
			range = stack.pop();
			i = range.x;
			j = range.y;
			
			int pivotIndex = SortUtil.mid3(data, i, j, (i + j) / 2);
			SortUtil.swap(data, pivotIndex, j);
			k = partition(data, i, j - 1, data[j]);
			SortUtil.swap(data, k, j);
			
			if((k - i) > 1)
			{
				range = new Range(i, k - 1);
				stack.push(range);
			}
			
			if((j - k) > 1)
			{
				range = new Range(k + 1, j);
				stack.push(range);
			}
		}
	}
	
	private int partition(int[] a, int low, int high, int pivot)
	{
		int lo = low, hi = high;
		while(lo <= hi)
		{
			while(lo <= hi && a[lo] < pivot)
			{
				lo++;
			}
			while(lo <= hi && a[hi] > pivot)
			{
				hi--;
			}
			if(lo <= hi)
			{
				SortUtil.swap(a, lo, hi);
				lo++;
				hi--;
			}
		}
		return lo;
	}
}
