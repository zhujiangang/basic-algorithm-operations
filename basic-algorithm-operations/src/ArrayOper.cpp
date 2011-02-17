#include <stdio.h>
#include <iostream.h>
#include "ArrayOper.h"
#include "config.h"
#include "MyUtil.h"

void printSumPairs(int a[], int n, int sum)
{
	int i, j;
	for(i = 0, j = n - 1; i < j;)
	{
		//bigger, go to the smaller j
		if(a[i] + a[j] > sum)
		{
			j--;
		}
		//smaller, go to the bigger i
		else if(a[i] + a[j] < sum)
		{
			i++;
		}
		//find one pair
		else
		{
			printf("(a[%d]=%d,a[%d]=%d) \n", i, a[i], j, a[j]);
			i++;
			j--;
		}
	}
}

void printSumPairsDup(int a[], int n, int sum)
{
	int i, j, k, l;
	for(i = 0, j = n - 1; i < j;)
	{
		//bigger, go to the smaller j
		if(a[i] + a[j] > sum)
		{
			j--;
		}
		//smaller, go to the bigger i
		else if(a[i] + a[j] < sum)
		{
			i++;
		}
		//find one pair
		else
		{
			k = i;
			l = j;
			for(i = k; i < n && a[k] == a[i]; i++)
			{
				for(j = l; j >= 0 && a[l] == a[j]; j--)
				{
					if(i < j)
					{
						printf("(a[%d]=%d,a[%d]=%d) \n", i, a[i], j, a[j]);
					}
				}
			}
		}
	}
}

/************************************************************************/
/* 2. Check an array if a post order of BST                             */
/************************************************************************/

bool checkSeqIfPostOrderOfBST(int a[], int start, int end)
{
	if(start == end)
	{
		return true;
	}

	int i, j;
	for(i = start; i < end; i++)
	{
		if(a[i] > a[end])
		{
			break;
		}
	}
	for(j = i; j < end; j++)
	{
		if(a[j] <= a[end])
		{
			return false;
		}
	}
	if(i > start)
	{
		if(!checkSeqIfPostOrderOfBST(a, start, i - 1))
		{
			return false;
		}
		
	}
	if(end - 1 >= i)
	{
		return checkSeqIfPostOrderOfBST(a, i, end - 1);
	}
	return true;
}
bool checkSeqIfPostOrderOfBST(int a[], int n)
{
	return checkSeqIfPostOrderOfBST(a, 0, n - 1);
}

/************************************************************************/
/* 3. Binary search from an array                                       */
/************************************************************************/
int binarySearch(int a[], int from, int to, int key)
{
	if(from > to)
	{
		return -1;
	}
	while(from <= to)
	{
		int mid = (from + to) >> 1;
		if(key < a[mid])
		{
			to = mid - 1;
		}
		else if(key > a[mid])
		{
			from = mid + 1;
		}
		else
		{
			return mid;
		}
	}
	return -(from + 1);
}
/************************************************************************/
/* 4. max sum of continuous sub-array                                   */
/************************************************************************/
int maxsumsub(int array[], int n, int* pStart, int* pEnd)
{
	int maxsum = 0, sum = 0;
	
	int i;
	for(i = 0; i < n; i++)
	{
		sum += array[i];
		
		if(sum > maxsum)
		{
			maxsum = sum;
			if(pEnd != NULL)
			{
				*pEnd = i;
			}
		}
		
		if(sum < 0)
		{
			sum = 0;
			if(pStart != NULL)
			{
				*pStart = i + 1;
			}
		}
	}
	
	return maxsum;
}


void testArrayOper()
{
#if ((ARRAY_OPER_TEST) == 1)
	// 	const int n = 10;
	// 	int a[n] = {3, 4, -5, 50, -90, 7, 6, -12, 80, -4};
	
	int a[] = {3, 4, -5, 50, -90, 7, 6, -12, 80, -4};
	int n = sizeof(a)/sizeof(int);
	//	genseq(a, n);
	//	genrand(a, n);
	//	quickSort(a, n);
	output(a, n);
	
	int i;
	int result = 0;
	//	printSumPairsDup(a, n, n * 3);
	
	//	cout<<binarySearch(a, 0, n - 1, 0.3 * n)<<endl;
	
	int start, end;
	result = maxsumsub(a, n, &start, &end);
	cout<<result<<endl;
	for(i = start; i <= end; i++)
	{
		cout<<a[i]<<" ";
	}
	cout<<endl;
	
	int b[] = {1 ,1 ,1, 1, 1, 1, 1};
	n = sizeof(b)/sizeof(int);
	
	i = binarySearch(b, 0, n - 1, 2);
	cout<<i<<endl;
	printSep(__FILE__);
#endif
}
