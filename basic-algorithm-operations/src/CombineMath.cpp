#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "MyUtil.h"
#include "CombineMath.h"
#include "config.h"


/************************************************************************/
/* P(n,n)                                                               */
/************************************************************************/
void perm_Pnn(int in[], int begin, int end)
{
	static int count = 0;
	if(begin == end)
	{
		count++;
		cout<<count<<") ";
		output(in, 0, end);
		return;
	}
	else
	{
		for(int i = begin; i <= end; i++)
		{
			swap(in[begin], in[i]);
			perm_Pnn(in, begin + 1, end);
			swap(in[begin], in[i]);
		}
	}
}

/************************************************************************/
/* P(n,m)                                                               */
/************************************************************************/
void perm_Pnm(int in[], int n, int m, int out[], int depth, int used[])
{
	static int count = 0;

	for(int i = 0; i < n; i++)
	{
		if(!used[i])
		{
			out[depth] = in[i];

			//Not last one
			if(depth < m - 1)
			{
				used[i] = 1;
				perm_Pnm(in, n, m, out, depth + 1, used);
				used[i] = 0;
			}
			//Last one. Get enough elements to output
			else
			{
				count++;
				cout<<count<<") ";
				output(out, m);
			}			
		}
	}
}

/************************************************************************/
/* collection. 2^n                                                      */
/************************************************************************/
void combine_2n(int in[], int len, int inBegin, int out[], int outBegin)
{
	static int count = 0;
	int i;
	for(i = inBegin; i < len; i++)
	{
		out[outBegin] = in[i];
		
		//Get one combine
		count++;
		cout<<count<<") ";
		output(out, 0, outBegin);
		//Get one combine end
		
		if(i < len - 1)
		{
			combine_2n(in, len, i + 1, out, outBegin + 1);
		}
	}
}

void combine_2n1(int id, int n, std::vector<int>& vec)
{
	static int count = 0;
	if(id > n)
	{
		count++;
		printf("%d) ", count);
		for(int i = 0; i < vec.size(); i++)
		{
			printf("%d ", vec[i]);
		}
		printf("\n");
		return;
	}
	vec.push_back(id);
	combine_2n1(id + 1, n, vec);

	vec.pop_back();
	combine_2n1(id + 1, n, vec);
}

void combine_2n2(int n)
{
	static int count = 0;
	
	//n should be less than 32
	unsigned int max = (1 << n);
	for(unsigned int i = 0; i < max; i++)
	{
		//output
		count++;
		printf("%d) ", count);
		for(int j = 0; j < n; j++)
		{
			if(i & (1 << j))
			{
				printf("%d ", j + 1);
			}
		}
		printf("\n");
	}
}

void combine_alpha_order(int n)
{
	static int count = 0;
	int* data = new int[n];
	int pos = 0;
	data[pos] = 1;
	while(true)
	{
		//output
		count++;
		printf("%d) ", count);
		for(int i = 0; i <= pos; i++)
		{
			printf("%d ", data[i]);
		}
		printf("\n");

		if(data[pos] < n)
		{
			data[pos + 1] = data[pos] + 1;
			pos++;
		}
		//data[pos] == n
		else if(pos > 0)
		{
			pos--;
			data[pos]++;
		}
		//data[pos] == n && pos == 0
		else
		{
			break;
		}
	}

	delete data;
}

//show out all the combinations less than m elements out of n
void combine_Cn_less_m_alpha_order(int n, int m)
{
	static int count = 0;

	int* data = new int[n];
	int pos = 0;
	data[pos] = 1;
	while(true)
	{
		//output
		count++;
		printf("%d) ", count);
		for(int i = 0; i <= pos; i++)
		{
			printf("%d ", data[i]);
		}
		printf("\n");
		
		if(data[pos] < n && (pos + 1) < m)
		{
			data[pos + 1] = data[pos] + 1;
			pos++;
		}
		else if( data[pos] < n )
		{
			data[pos]++;
		}
		//data[pos] == n
		else if(pos > 0)
		{
			pos--;
			data[pos]++;
		}
		//data[pos] == n && pos == 0
		else
		{
			break;
		}
	}
	
	delete data;
}

//Cnm
void combine_Cnm_alpha_order(int n, int m)
{
	static int count = 0;
	
	int* data = new int[n];
	int pos = 0;

	data[pos] = 1;
	while(pos + 1 < m)
	{
		data[pos + 1] = data[pos] + 1;
		pos++;
	}
	while(true)
	{
		//output
		count++;
		printf("%d) ", count);
		for(int i = 0; i <= pos; i++)
		{
			printf("%d ", data[i]);
		}
		printf("\n");
		
		if(data[pos] < n)
		{
			data[pos]++;
		}
		else
		{
			int diff = 0;
			while(pos > 0)
			{
				pos--;
				diff++;
				if(data[pos] + diff < n)
				{
					break;
				}
			}

			if(data[pos] + diff >= n)
			{
				break;
			}

			data[pos]++;
			while(pos + 1 < m)
			{
				data[pos + 1] = data[pos] + 1;
				pos++;
			}
		}
	}
	
	delete data;
}

void combine_Cnm_alpha_order2(int n, int m)
{
	if(m <= 0)
	{
		return;
	}
	static int count = 0;
	
	int* data = new int[m];
	int i = 0, dist;
	data[i++] = 1;
	for(; i < m; i++)
	{
		data[i] = data[i - 1] + 1;
	}
	while(true)
	{
		//output
		count++;
		printf("%d) ", count);
		for(int i = 0; i < m; i++)
		{
			printf("%d ", data[i]);
		}
		printf("\n");

		//find first not full element from right to left
		for(i = m - 1, dist = 0; i >= 0; i--, dist++)
		{
			if(data[i] + dist < n)
			{
				break;
			}
		}

		//all is full, last one
		if(i < 0)
		{
			break;
		}

		//refresh all the elements
		data[i++]++;
		for( ; i < m; i++)
		{
			data[i] = data[i - 1] + 1;
		}
	}
	
	delete data;
}

/************************************************************************/
/* C(n, m) = C(n-1, m-1) + C(n-1, m)                                   */
/************************************************************************/
void combine_Cnm(int in[], int n, int m, int out[], int outputLen, int depth)
{
	static int count = 0;
	if(m < 0)
	{
		return;
	}
	int i;
	//Got one result
	if(m == 0)
	{
		count++;
		cout<<count<<") ";
		for(i = 0; i < outputLen; i++)
		{
			cout<<out[i]<<" ";
		}
		cout<<endl;
		return;
	}

	if(n <= 0 || n < m)
	{
		return;
	}

	out[depth] = in[n - 1];
	combine_Cnm(in, n - 1, m - 1, out, outputLen, depth + 1);
	combine_Cnm(in, n - 1, m, out, outputLen, depth);
}


void combineCnm(int id, int n, std::vector<int>& vec)
{

}

/*************************************************************************/
/* C(n, m) = C(n-1, m-1) + C(n-2, m-1) + C(n-3, m-1) + .... + C(m-1, m-1)*/
/*************************************************************************/
void combine_Cnm2(int in[], int n, int m, int out[], int outputLen)
{
	static int count = 0;

	int i, j;
	for(i = n - 1; i >= m - 1; i--)
	{
		out[m - 1] = in[i];

		//Greater than 1. If m == 1, we just need to select one, and this one has been done by "output[m - 1] = input[i]",
		//the task was finished. We have got one result and should go to "else" to process this result.
		if(m > 1)
		{
			combine_Cnm2(in, i, m - 1, out, outputLen);
		}
		else
		{
			count++;
			cout<<count<<") ";
			for(j = 0; j < outputLen; j++)
			{
				cout<<out[j]<<" ";
			}
			cout<<endl;
		}
	}
}

void testCombineMath()
{
#if ((COMBINE_MATH_TEST) == 1)
	const int n = 5;
	const int m = 3;
	int a[n];
	int b[n];
	
	genseq(a, n);
	//	genrand(a, n, 100);
	printf("Content of array 'a': \n");
	output(a, n);
	cout<<endl;
	
	printf("perm_Pnn: \n");
	//perm_Pnn(a, 0, n - 1);
	cout<<endl;
	
	printf("combine_2n: \n");
	combine_2n(a, n, 0, b, 0);
	cout<<endl;

	std::vector<int> vec;
	printf("combine_2n1: \n");
	combine_2n1(1, n, vec);

	printf("combine_2n2: \n");
	combine_2n2(n);

	printf("combine_alpha_order: \n");
	combine_alpha_order(n);
	
	printf("combine_Cnm: \n");
	combine_Cnm(a, n, m, b, m, 0);
	cout<<endl;
	
	printf("combine_Cnm2: \n");
	combine_Cnm2(a, n, m, b, m);
	cout<<endl;

	printf("combine_Cn_less_m_alpha_order: \n");
	combine_Cn_less_m_alpha_order(n, m);

	printf("combine_Cnm_alpha_order: \n");
	combine_Cnm_alpha_order(n, m);

	printf("combine_Cnm_alpha_order2: \n");
	combine_Cnm_alpha_order2(n, m);
	
	int used[n];
	memset(used, 0, n * sizeof(int));
	printf("perm_Pnm: \n");
	perm_Pnm(a, n, m, b, 0, used);
	cout<<endl;

	printSep(__FILE__);
#endif
}
