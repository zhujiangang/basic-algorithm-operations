package com.bao.sort;

import com.bao.sort.SortUtil.Sort;

public class JDKQSort implements Sort
{

	@Override
	public void sort(int[] data)
	{
		sort(data, 0, data.length - 1);

	}

	// ${JAVA_HOME}\demo\applets\SortDemo\QSortAlgorithm.java
	private void sort(int[] a, int i, int j)
	{
		int lo = i;
		int hi = j;
		int mid;

		if(j > i)
		{

			/*
			 * Arbitrarily establishing partition element as the midpoint of the
			 * array.
			 */
			mid = a[(i + j) / 2];

			// loop through the array until indices cross
			while(lo <= hi)
			{
				/*
				 * find the first element that is greater than or equal to the
				 * partition element starting from the left Index.
				 */
				while((lo < j) && (a[lo] < mid))
					++lo;

				/*
				 * find an element that is smaller than or equal to the
				 * partition element starting from the right Index.
				 */
				while((hi > i) && (a[hi] > mid))
					--hi;

				// if the indexes have not crossed, swap
				if(lo <= hi)
				{
					SortUtil.swap(a, lo, hi);
					++lo;
					--hi;
				}
			}

			/*
			 * If the right index has not reached the left side of array must
			 * now sort the left partition.
			 */
			if(i < hi)
				sort(a, i, hi);

			/*
			 * If the left index has not reached the right side of array must
			 * now sort the right partition.
			 */
			if(lo < j)
				sort(a, lo, j);

		}
	}
}
