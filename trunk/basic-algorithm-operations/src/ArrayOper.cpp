#include <stdio.h>
#include "ArrayOper.h"


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