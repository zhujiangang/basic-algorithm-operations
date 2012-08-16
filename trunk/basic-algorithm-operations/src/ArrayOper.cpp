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
	int from = 0, end = 0;
	
	int i;
	for(i = 0; i < n; i++)
	{
		sum += array[i];
		
		if(sum > maxsum)
		{
			maxsum = sum;
			end = i;
			if(pEnd != NULL)
			{
				*pStart = from;
				*pEnd = end;
			}
		}
		
		if(sum < 0)
		{
			sum = 0;
			from = i + 1;
		}
	}
	
	return maxsum;
}
int maxsumsub_dp(int a[], int n, int* pStart, int* pEnd)
{
	int maxsum = 0;

	/*
	int* c = new int[n];
	c[0] = a[0];

	for(int i = 1; i < n; i++)
	{
		if(c[i - 1] > 0)
		{
			c[i] = c[i - 1] + a[i];
		}
		else
		{
			c[i] = a[i];
		}

		if(c[i] > maxsum)
		{
			maxsum = c[i];
		}
	}
	delete[] c;
	*/
	
	//Optimized, without extra space
	int sum = -1;
	for(int i = 0; i < n; i++)
	{
		if(sum > 0)
		{
			sum += a[i];
		}
		else
		{
			sum = a[i];
			*pStart = i;
		}

		if(sum > maxsum)
		{
			maxsum = sum;
			*pEnd = i;
		}
	}

	return maxsum;
}

int lowest_bit(int x)
{
	return x & ~(x - 1);
}

void find2distinct(int a[], int n, int* x, int* y)
{
	int i, s = 0;
	for(i = 0; i < n; i++)
	{
		s ^= a[i];
	}

	*x = 0, *y = 0;
	int lowbit = lowest_bit(s);
	for(i = 0; i < n; i++)
	{
		if( (a[i] & lowbit) != 0)
		{
			*x ^= a[i];
		}
		else
		{
			*y ^= a[i];
		}
	}
}

void find3distinct(int a[], int n, int* x, int* y, int* z)
{
	int i, s = 0;
	for(i = 0; i < n; i++)
	{
		s ^= a[i];
	}

	*x = 0, *y = 0, *z = 0;
	int lowbit = lowest_bit(s);
	int flip = 0;

	for(i = 0; i < n; i++)
	{
		flip ^= lowest_bit(s ^ a[i]);
	}

	//flip = lowest_bit(x ^ y) ^ lowest_bit(x ^ z) ^ lowest_bit(y ^ z);
	for(i = 0; i < n; i++)
	{
		if(lowest_bit(s ^ a[i]) == flip)
		{
			*x ^= a[i];
		}
	}

	//swap out the first value
	for(i = 0; i < n; i++)
	{
		if(a[i] == *x)
		{
			int tmp = a[i];
			a[i] = a[n - 1];
			a[n - 1] = tmp;
			break;
		}
	}

	find2distinct(a, n - 1, y, z);
}

void find2miss(int a[], int n, int* x, int* y)
{
	int i, curr_xor = 0, full_xor = 0;
	for(i = 0; i < n - 2; i++)
	{
		curr_xor ^= a[i];
	}
	for(i = 1; i <= n; i++)
	{
		full_xor ^= i;
	}

	//t = x ^ y
	int t = curr_xor ^ full_xor;
	int lowbit_t = lowest_bit(t);
	//split the array into 2 part, 
	//the 1st part elements with same low bit of t, 
	//the 2nd part elements without same low bit of t
	int a1 = 0, a2 = 0;

	//a1 including x
	for(i = 1; i <= n; i++)
	{
		if(i & lowbit_t)
		{
			a1 ^= i;
		}
	}

	//a2 excluding y
	for(i = 0; i < n - 2; i++)
	{
		if((a[i] & lowbit_t) == 0)
		{
			a2 ^= a[i];
		}
	}

	*x = (a1^a2^curr_xor);
	*y = ((*x) ^ t);
}

/*
http://www.felix021.com/blog/read.php?1587
http://www.iteye.com/topic/1054321
http://yzmduncan.iteye.com/blog/1546503
*/
int lis(int a[], int n, int d[], int mem[])
{
	d[0] = a[0];
	mem[0] = 1;

	int i, j, len = 1;
	for(i = 1; i < n; i++)
	{
		if(a[i] > d[len - 1])
		{
			j = len++;
		}
		else
		{
			int lo = 0, hi = len - 1, mid;
			while(lo <= hi)
			{
				mid = (lo + hi) / 2;
				if(a[i] > d[mid])
				{
					lo = mid + 1;
				}
				else
				{
					hi = mid - 1;
				}
			}
			j = lo;
		}
		d[j] = a[i];
		mem[i] = j + 1;
	}
	return len;
}


void testArrayOper()
{
#if ((ARRAY_OPER_TEST) == 1)
	//int a[] = {3, 4, -5, 50, -90, 7, 6, -12, 80, -4};
	int a[] = {-3, -4, -5, 50, -90, 7, 6, -12, 80, -40, 41};
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
	cout<<"maxsumsub: "<<result<<endl;
	for(i = start; i <= end; i++)
	{
		cout<<a[i]<<" ";
	}
	cout<<endl;
	start = end = 0;

	result = maxsumsub_dp(a, n, &start, &end);
	cout<<"maxsumsub_dp: "<<result<<endl;
	for(i = start; i <= end; i++)
	{
		cout<<a[i]<<" ";
	}
	cout<<endl;
	
	int b[] = {1 ,1 ,1, 1, 1, 1, 1};
	n = sizeof(b)/sizeof(int);
	
	i = binarySearch(b, 0, n - 1, 2);
	cout<<i<<endl;

	int c[] = {1, 10, 20, 9, 20, 8, 9, 1};
	int x = 0, y = 0;
	find2distinct(c, sizeof(c)/sizeof(c[0]), &x, &y);
	printf("The 2 distinct number: x=%d, y=%d\n", x, y);

	int d[] = {1, 10, 20, 9, 20, 8, 9, 1, 23, 23, 4};
	int z = 0;
	find3distinct(d, sizeof(d)/sizeof(d[0]), &x, &y, &z);
	printf("The 3 distinct number: x=%d, y=%d, z=%d\n", x, y, z);


	int e[] = {1, /*2,*/ 3, 4, 5, 6, 7, 8, 9, /*10*/};
	find2miss(e, 10, &x, &y);
	printf("The 2 missing number: x=%d, y=%d\n", x, y);

	int f[] = {2,1,5,3,6,4,8,9,7};
	int tmp1[20], tmp2[20];
	n = sizeof(f)/sizeof(f[0]);
	result = lis(f, n, tmp1, tmp2);
	printf("lis length = %d\n", result);
	printf("lis content = ");
	for(i = n - 1; i >= 0 && result > 0; i--)
	{
		if(tmp2[i] == result)
		{
			printf("%d ", f[i]);
			result--;
		}
	}
	printf("\n");

	printSep(__FILE__);
#endif
}
