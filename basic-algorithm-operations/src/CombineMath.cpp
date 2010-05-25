#include <iostream.h>
#include "MyUtil.h"
#include "CombineMath.h"
#include <stdio.h>

void perm(int a[], int begin, int end)
{
	static int count = 0;
	if(begin == end)
	{
		count++;
		cout<<count<<") ";
		output(a, 0, end);
		return;
	}
	else
	{
		for(int i = begin; i <= end; i++)
		{
			swap(a[begin], a[i]);
			perm(a, begin + 1, end);
			swap(a[begin], a[i]);
		}
	}
}

void combine(int in[], int len, int inBegin, int out[], int outBegin)
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
			combine(in, len, i + 1, out, outBegin + 1);
		}
	}
}

void combine1(int input[], int n, int m, int output[], int outputLen, int depth)
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
			cout<<output[i]<<" ";
		}
		cout<<endl;
		return;
	}

	if(n <= 0 || n < m)
	{
		return;
	}

	output[depth] = input[n - 1];
	combine1(input, n - 1, m - 1, output, outputLen, depth + 1);
	combine1(input, n - 1, m, output, outputLen, depth);
}

void combine2(int input[], int n, int m, int output[], int outputLen)
{
	static int count = 0;

	int i, j;
	for(i = n - 1; i >= m - 1; i--)
	{
		output[m - 1] = input[i];

		//Greater than 1. If m == 1, we just need to select one, and this one has been done by "output[m - 1] = input[i]",
		//the task was finished. We have got one result and should go to "else" to process this result.
		if(m > 1)
		{
			combine2(input, i, m - 1, output, outputLen);
		}
		else
		{
			count++;
			cout<<count<<") ";
			for(j = 0; j < outputLen; j++)
			{
				cout<<output[j]<<" ";
			}
			cout<<endl;
		}
	}
}