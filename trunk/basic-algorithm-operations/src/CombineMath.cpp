#include <iostream.h>
#include "MyUtil.h"
#include "CombineMath.h"
#include <stdio.h>

/************************************************************************/
/* P(n,n)                                                               */
/************************************************************************/
void perm(int in[], int begin, int end)
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
			perm(in, begin + 1, end);
			swap(in[begin], in[i]);
		}
	}
}

/************************************************************************/
/* P(n,m)                                                               */
/************************************************************************/
void perm(int in[], int n, int m, int out[], int depth, int used[])
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
				perm(in, n, m, out, depth + 1, used);
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

/************************************************************************/
/* C(n, m) = C(n-1, m-1) + C(n-1, m)                                   */
/************************************************************************/
void combine1(int in[], int n, int m, int out[], int outputLen, int depth)
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
	combine1(in, n - 1, m - 1, out, outputLen, depth + 1);
	combine1(in, n - 1, m, out, outputLen, depth);
}

/*************************************************************************/
/* C(n, m) = C(n-1, m-1) + C(n-2, m-1) + C(n-3, m-1) + .... + C(m-1, m-1)*/
/*************************************************************************/
void combine2(int in[], int n, int m, int out[], int outputLen)
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
			combine2(in, i, m - 1, out, outputLen);
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