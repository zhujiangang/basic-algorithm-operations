package com.e2u.sort;

public class JDKSort
{
	/**
	 * Swaps x[a .. (a+n-1)] with x[b .. (b+n-1)].
	 */
	private static void vecswap(int x[], int a, int b, int n)
	{
		for(int i = 0; i < n; i++, a++, b++)
			swap(x, a, b);
	}

	static void swap(int x[], int a, int b)
	{
		int t = x[a];
		x[a] = x[b];
		x[b] = t;
	}

	public static void qsort(int x[], int off, int len)
	{
		int m = off + (len >> 1); // Small arrays, middle element
		int v = x[m];

		int a = off, b = a, c = off + len - 1, d = c;
		while(true)
		{
			while(b <= c && x[b] <= v)
			{
				if(x[b] == v)
					swap(x, a++, b);
				b++;
			}
			while(c >= b && x[c] >= v)
			{
				if(x[c] == v)
					swap(x, c, d--);
				c--;
			}
			if(b > c)
				break;
			swap(x, b++, c--);
		}

		// Swap partition elements back to middle
		int s, n = off + len;
		s = Math.min(a - off, b - a);
		vecswap(x, off, b - s, s);
		s = Math.min(d - c, n - d - 1);
		vecswap(x, b, n - s, s);

		// Recursively sort non-partition-elements
		if((s = b - a) > 1)
			qsort(x, off, s);
		if((s = d - c) > 1)
			qsort(x, n - s, s);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args)
	{

	}
}
