package com.bao.sort;

public class ShellSort implements SortUtil.Sort
{

	/* (non-Javadoc)
	 * @see org.rut.util.algorithm.SortUtil.Sort#sort(int[])
	 */
	public void sort(int[] data)
	{
		for (int i = data.length / 2; i > 2; i /= 2)
		{
			for (int j = 0; j < i; j++)
			{
				insertSort(data, j, i);
			}
		}
		insertSort(data, 0, 1);
	}

	/**
	 * @param data
	 * @param j
	 * @param i
	 */
	private void insertSort(int[] data, int start, int inc)
	{
		int temp;
		for (int i = start + inc; i < data.length; i += inc)
		{
			for (int j = i; (j >= inc) && (data[j] < data[j - inc]); j -= inc)
			{
				SortUtil.swap(data, j, j - inc);
			}
		}
	}

}
