#include <iostream.h>
#include "ArraySort.h"
#include "MyUtil.h"
#include "config.h"

void insertSort(int a[], int n)
{
	for(int i = 0; i < n - 1; i++)
	{
		for(int j = i + 1; j > 0 && (a[j] < a[j - 1]); j--)
		{
			swap(a[j], a[j - 1]);
		}
	}
}

void bubbleSort(int a[], int n)
{
	for(int i = 0; i < n - 1; i++)
	{
		for(int j = n - 1; j > i; j--)
		{
			if(a[j] < a[j - 1])
			{
				swap(a[j], a[j - 1]);
			}
		}
	}
}

void merge(int src[], int dst[], int l, int r, int m)
{
	int i, j, k;

	for(i = l, j = m + 1, k = l; i <= m && j <= r; k++)
	{
		if(src[i] <= src[j])
		{
			dst[k] = src[i];
			i++;
		}
		else
		{
			dst[k] = src[j];
			j++;
		}
	}

	while(i <= m)
	{
		dst[k++] = src[i++];
	}
	while(j <= r)
	{
		dst[k++] = src[j++];
	}
}

void mergeSort(int src[], int dst[], int l, int r)
{
	if(l >= r)
	{
		return;
	}
	int m = (l + r) / 2;

	mergeSort(dst, src, l, m);
	mergeSort(dst, src, m + 1, r);

	merge(src, dst, l, r, m);
}

void mergeSort(int a[], int n)
{
	int* b = new int[n];
	arryCopy(a, 0, b, 0, n);

	mergeSort(b, a, 0, n - 1);

	delete b;
}

int partition(int a[], int l, int r, int pivot)
{
	do 
	{
		while(a[++l] < pivot)
			;
		while(r > 0 && a[--r] > pivot)
			;
		swap(a[l], a[r]);
	} while (l < r);
	swap(a[l], a[r]);

	return l;
}

void quickSort(int a[], int l, int r)
{
	if(l < r)
	{
		int pivotIndex = (l + r) / 2;
		swap(a[r], a[pivotIndex]);
		int k = partition(a, l - 1, r, a[r]);
		swap(a[r], a[k]);

		quickSort(a, l, k - 1);
		quickSort(a, k + 1, r);
	}
}

void quickSort(int a[], int n)
{
	quickSort(a, 0, n - 1);
}

void shiftDown(int a[], int n, int pos)
{
	int c = pos * 2 + 1;

	while(c < n)
	{
		if(c < n - 1 && a[c] < a[c + 1])
		{
			c++;
		}

		if(a[pos] >= a[c])
		{
			break;
		}
		swap(a[pos], a[c]);
		pos = c;
		c = pos * 2 + 1;
	}
}

void heapSort(int a[], int n)
{
	int i;
	for(i = n / 2 - 1; i >= 0; i--)
	{
		shiftDown(a, n, i);
	}

	for(i = 0; i < n - 1; i++)
	{
		swap(a[0], a[n - i - 1]);
		shiftDown(a, n - i - 1, 0);
	}
}

void testArraySort()
{
#if ((ARRAY_SORT_TEST) == 1)
	const int n = 2000;
	int datasource[n], copya[n];
	genrand(datasource, n, n);
	//	output(datasource, n);
	
	arryCopy(datasource, 0, copya, 0, n);
	//	output(copya, n);
	
	sorter sorters[] = {bubbleSort, insertSort, mergeSort, quickSort, heapSort};
	
	int ret;
	clock_t start, finish;
	for(int i = 0; i < COUNTOF(sorters); i++)
	{
		start = clock();
		sorters[i](copya, n);
		finish = clock();
		
		if( (ret = validateInOrder(copya, n)) != -1)
		{
			cout<<"Failed ("<<i<<": "<<ret<<endl;
			//output(copya, n);
			break;
		}
		else
		{
			cout<<"Success ("<<i<<") time = "<<(finish - start)<<"("<<(double)(finish - start)/CLOCKS_PER_SEC<<"s)"<<endl;
		}
		arryCopy(datasource, 0, copya, 0, n);
	}
	printSep(__FILE__);
#endif
}
