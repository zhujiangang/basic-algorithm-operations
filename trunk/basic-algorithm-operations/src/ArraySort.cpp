#include <iostream.h>
#include "ArraySort.h"
#include "MyUtil.h"
#include "ArrayOper.h"
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

void mergeSort2(int a[], int l, int r)
{
	if(l >= r)
	{
		return;
	}
	int m = (l + r) / 2;
	
	mergeSort2(a, l, m);
	mergeSort2(a, m + 1, r);
	
	merge(a, l, r, m);
}

void mergeSort2(int a[], int n)
{
	mergeSort2(a, 0, n - 1);
}

void mergePass(int src[], int dst[], int n, int len)
{
	int i = 0;
	for(i = 0; i + 2 * len <= n; i += 2 * len)
	{
		merge(src, dst, i, i + 2 * len - 1, i + len - 1);
	}

	if(i + len < n)
	{
		merge(src, dst, i, n - 1, i + len - 1);
	}
	else
	{
		for( ; i < n; i++)
		{
			dst[i] = src[i];
		}
	}
}

void mergeSortIter(int a[], int n)
{
	int* b = new int[n];
	
	int len = 1;

	while(len < n)
	{
		mergePass(a, b, n, len);
		len *= 2;

		mergePass(b, a, n, len);
		len *= 2;
	}
	
	delete b;
}

typedef struct DataNode
{
	int data;
	int index;
	int active;
}DataNode;

static int pow2(int n)
{
	int x = 1;
	while(x < n)
	{
		x <<= 1;
	}
	return x;
}

void updateTree(DataNode* pTree, int i)
{   
	int j;
	if ( i % 2 == 0 )
		pTree[(i-1)/2] = pTree[i-1];    //i偶数, 对手左结点
	else 
		pTree[(i-1)/2] = pTree[i+1]; //i奇数, 对手右结点

	i = (i-1) / 2;                            //向上调整    
	while ( i ) //直到 i==0
	{                             
		if ( i % 2 == 0) 
			j = i-1;
		else 
			j = i+1;

		if ( !pTree[i].active || !pTree[j].active )
		{
			if ( pTree[i].active )
				pTree[(i-1)/2] = pTree[i];       //i可参选, i上
			else  
				pTree[(i-1)/2] = pTree[j];   //否则, j上
		}
		else			//两方都可参选	
		{
			if ( pTree[i].data < pTree[j].data ) 
				pTree[(i-1)/2] = pTree[i];        //关键码小者上
			else  
				pTree[(i-1)/2] = pTree[j];
		}
		i = (i-1) / 2;      // i上升到双亲
    }
}

void tournamentSort(int a[], int n)
{
	DataNode* pTree = NULL;

	int bottomSize = pow2(n);
	int treeSize = 2 * bottomSize - 1;
	int loadIndex = bottomSize - 1;

	pTree = new DataNode[treeSize];

	//copy data to winner tree
	int i, j = 0;
	for(i = loadIndex; i < treeSize; i++)
	{
		pTree[i].index = i;
		if(j < n)
		{
			pTree[i].active = 1;
			pTree[i].data = a[j++];
		}
		else
		{
			pTree[i].active = 0;
		}
	}

	//first pass compare, select the min
	//i is the first element of this level
	i = loadIndex;
	while(i)
	{
		j = i;
		//(2 * i) is the biggest index of this level
		while(j < 2 * i)
		{
			if(!pTree[j + 1].active || pTree[j].data <= pTree[j+1].data)
			{
				pTree[(j-1)/2] = pTree[j];
			}
			else
			{
				pTree[(j-1)/2] = pTree[j+1];
			}
			j += 2;
		}

		//go up the upper level
		i = (i - 1)/2;
	}

	for(i = 0; i < n - 1; i++)
	{
		a[i] = pTree[0].data;
		pTree[pTree[0].index].active = 0;
		updateTree(pTree, pTree[0].index);
	}
	a[n-1] = pTree[0].data;

	delete [] pTree;
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

/*
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
*/
void shiftDown(int a[], int n, int pos)
{
	int p = pos, c = 2 * p + 1;
	while(c < n)
	{
		if(c < n - 1 && a[c] < a[c + 1])
		{
			c++;
		}
		if(a[p] >= a[c])
		{
			break;
		}
		swap(a[p], a[c]);
		
		p = c;
		c = 2 * p + 1;
	}
}

void shiftUp(int a[], int n, int pos)
{
	int c = pos, p = (c + 1) / 2 - 1;
	while(p >= 0)
	{
		if(a[p] >= a[c])
		{
			break;
		}
		swap(a[p], a[c]);

		c = p;
		p = (c + 1) / 2 - 1;
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

void heapSort2(int a[], int n)
{
	/*
	int i;
	for(i = n / 2 - 1; i >= 0; i--)
	{
		shiftDown(a, n, i);
	}
	for(i = n - 1; i > 0; i--)
	{
		swap(a[0], a[i]);
		shiftDown(a, i, 0);
	}
	*/

	int gap = 3;
	n -= gap;
	int i;
	for(i = n / 2 - 1; i >= 0; i--)
	{
		shiftDown(a, n, i);
	}

	for(i = 0; i < gap; i++)
	{
		n++;
		shiftUp(a, n, n - 1);
	}

	for(i = n - 1; i > 0; i--)
	{
		swap(a[0], a[i]);
		shiftDown(a, i, 0);
	}
}

void testArraySort()
{
#if ((ARRAY_SORT_TEST) == 1)
	const int n = 5000;
	int datasource[n], copya[n];
	genrand(datasource, n, n);
	//	output(datasource, n);
	
	arryCopy(datasource, 0, copya, 0, n);
	//	output(copya, n);

	int a[] = {21, 25, 49, 25, 16, 8/*, 63*/};
	tournamentSort(a, COUNTOF(a));
	output(a, COUNTOF(a));
	
	sorter sorters[] = {bubbleSort, insertSort, mergeSort, mergeSort2, mergeSortIter, tournamentSort, quickSort, heapSort, heapSort2};
	
	int ret;
	clock_t start, finish;
	for(int i = 0; i < COUNTOF(sorters); i++)
	{
		start = clock();
		sorters[i](copya, n);
		finish = clock();
		
		if( (ret = validateInOrder(copya, n)) != -1)
		{
			cout<<"Failed ("<<i<<"): "<<ret<<endl;
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
