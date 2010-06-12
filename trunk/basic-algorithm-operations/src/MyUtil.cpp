#include <iostream.h>
#include <stdlib.h>
#include "MyUtil.h"


void output(int a[], int begin, int end)
{
	for(int i = begin; i <= end; i++)
	{
		cout<<a[i]<<" ";
	}
	cout<<endl;
}

void output(int a[], int n)
{
	output(a, 0, n - 1);
}

void swap(int& x, int& y)
{
	int t = x;
	x = y;
	y = t;
}

void genrand(int a[], int n, int limit)
{
	srand( (unsigned int)(time(NULL)) );
	
	if(limit <= 0)
	{
		for(int i = 0; i < n; i++)
		{
			a[i] = rand();
		}
	}
	else
	{
		for(int i = 0; i < n; i++)
		{
			a[i] = rand() % limit;
		}
	}
}

void genseq(int a[], int n, int start)
{
	for(int i = 0; i < n; i++)
	{
		a[i] = i + start;
	}
}

void arryCopy(int src[], int srcPos, int dst[], int dstPos, int length)
{
	int i, j, k;
	for(k = 0, i = srcPos, j = dstPos; k < length; k++, i++, j++)
	{
		dst[j] = src[i];
	}
}


int validateInOrder(int a[], int n, bool isAscending)
{
	int i;
	if(isAscending)
	{
		for(i = 0; i < n - 1; i++)
		{
			if(a[i] > a[i + 1])
			{
				return i;
			}
		}
	}
	else
	{
		for(i = 0; i < n - 1; i++)
		{
			if(a[i] < a[i + 1])
			{
				return i;
			}
		}
	}
	return -1;
}

int bitsNum(int a)
{
	unsigned int x = (unsigned int)a;
	if(x == 0)
	{
		return 1;
	}
	int count = 0;
	while(x != 0)
	{
		count++;
		x = x >> 1;
	}
	return count;
}
/************************************************************************/
/* Greatest power of 2 less than or equal to x, branch-free.
   From HD 3-2
/************************************************************************/
unsigned int flp2(unsigned int x) 
{
	x = x | (x >> 1); 
	x = x | (x >> 2); 
	x = x | (x >> 4); 
	x = x | (x >> 8); 
	x = x | (x >>16); 
	return x - (x >> 1); 
} 

