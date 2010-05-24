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

void Combin(int m,int n,int a) 
{ 
    if(m == n)
	{ 
        if(a>0)
			printf("%d",a); 
        while(n > 0) 
			printf("%d",n--); 
        printf("\n"); 
        return; 
    } 
    else if(0 == n)
	{ 
        printf("%d ",a); 
        return; 
    } 
    Combin(m-1,n-1,a*10+m); 
    Combin(m-1,n,a); 
}

void search( int depth, int n, int m, int b[], int len)
{
	static int count = 0;
	int i; 
	if( m == 0 ) //满足条件则输出 
	{ 
		count++; 
		printf("%d) ", count);
		for( i = 0; i < len; i++ ) 
			printf( "%d ", b[i] ); 
		printf( "\n" ); 
		
		return ; 
	} 
	if( m > n || n <= 0 ) return; //依常识剪枝 
	b[depth] = n; 
	search( depth + 1, n - 1, m - 1 , b, len); 
	b[depth] = 0; 
	search( depth, n - 1, m, b, len ); 
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

	if(m <= 0)
	{
		count++;
		cout<<count<<") ";
		for(j = 0; j < outputLen; j++)
		{
			cout<<output[j]<<" ";
		}
		cout<<endl;
		return;
	}

	for(i = n - 1; i >= m - 1; i--)
	{
		output[m - 1] = input[i];
		if(m > 1)
		{
			combine2(input, n - 1, m - 1, output, outputLen);
		}
	}
}