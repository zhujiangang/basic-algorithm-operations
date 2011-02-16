#include <iostream.h>
#include <stdio.h>
#include <typeinfo.h>
#include "BitOper.h"
#include "config.h"
#include "MyUtil.h"

#pragma warning( push )
#pragma warning( disable : 4307 )

bool find(const unsigned char* p, int len, unsigned int x, int* byteIndex, int* bitIndex)
{
	int i, j;
	unsigned int ret = 0;
	for(i = 0; i <= (len - 4); i++)
	{
		for(j = 0; j < 8; j++)
		{
			//Index out of bound
			if(i == (len - 4) && j > 0)
			{
				break;
			}
			ret = 0;
			ret |= (p[i] << (24 + j));
			ret |= (p[i + 1] << (16 + j));
			ret |= (p[i + 2] << (8 + j));
			ret |= (p[i + 3] << j);
			ret |= (p[i + 4] >> (8 - j));

			if(x == ret)
			{
				*byteIndex = i;
				*bitIndex = j;
				return true;
			}
		}
	}

	return false;
}

void showBasicTypeInfo()
{	
	cout<<"typeid(0x7FFFFFFF)="<<typeid(0x7FFFFFFF).name()<<endl;
	cout<<"typeid(0x80000000)="<<typeid(0x80000000).name()<<endl;
	cout<<"typeid(0xFFFFFFFF)="<<typeid(0xFFFFFFFF).name()<<endl;

	cout<<"typeid(2147483647)="<<typeid(2147483647).name()<<endl;
	cout<<"typeid(2147483648)="<<typeid(2147483648).name()<<endl;
	cout<<"typeid(4294967295)="<<typeid(4294967295).name()<<endl;
	cout<<"typeid(-1)="<<typeid(-1).name()<<endl;
	cout<<"typeid(0)="<<typeid(0).name()<<endl;

	cout<<"typeid(0x7FFFFFFF + 1)="<<typeid(0x7FFFFFFF + 1).name();
	cout<<"; (0x7FFFFFFF + 1)="<<(0x7FFFFFFF + 1)<<endl;
	cout<<"typeid(0x80000000 - 1)="<<typeid(0x80000000 - 1).name();
	cout<<"; (0x80000000 - 1)="<<(0x80000000 - 1)<<endl;
	cout<<"typeid(0xFFFFFFFF + 1)="<<typeid(0xFFFFFFFF + 1).name();
	cout<<"; (0xFFFFFFFF + 1)="<<(0xFFFFFFFF + 1)<<endl;

	int a = -100;
	int b = -200;
	cout<<"a = "<<a<<", b = "<<b<<endl;
	cout<<"typeid((b - a) & 0x80000000)="<<typeid((b - a) & 0x80000000).name()<<endl;
	cout<<"typeid((b - a) & 0x7FFFFFFF)="<<typeid((b - a) & 0x7FFFFFFF).name()<<endl;
	cout<<"typeid((a - b) & 0x80000000)="<<typeid((a - b) & 0x80000000).name()<<endl;
	cout<<"typeid((a - b) & 0x7FFFFFFF)="<<typeid((a - b) & 0x7FFFFFFF).name()<<endl;

	cout<<"typeid((a - b) | 0x80000000)="<<typeid((a - b) | 0x80000000).name()<<endl;
	cout<<"typeid((b - a) | 0x80000000)="<<typeid((b - a) | 0x80000000).name()<<endl;

	cout<<"((b - a) | 0x80000000)="<<((b - a) | 0x80000000)<<endl;
}
/************************************************************************/
/* 
The "doz" function is "difference or zero," defined as follows, for signed arguments:
if   x >= y, doz(x, y) = x - y;
else x <  y, doz(x, y) = 0;

HD 2-18 says: 

"There seems to be no very good way to implement doz(x, y), max(x, y), and min(x, y) 
in a branch-free way that is applicable to most computers",

This is NOT true. There's a better way than it provides.

doz(x, y) = (x - y) & ( (y - x) >> 31 )
*/
/************************************************************************/
int doz(int a, int b)
{
	return (a - b) & ((b - a) >> 31);
}
/************************************************************************/
/* 
x & (-1) = x & (0xFFFFFFFF) = x
x & 0 = 0

if   x >= 0, x >> 31 = 0;
else x <  0, x >> 31 = -1

*/
/************************************************************************/
int min(int a, int b)
{
	int diff = b - a;
	return a + (diff & (diff >> 31));
}
int max(int a, int b)
{
	int diff = b - a;
	return b - (diff & (diff >> 31));
}

/************************************************************************/
/* 
Reference: HD 2-7 Sign Function

sign(x) = (x >> 31) | (-x >>> 31) 

There's no operation >>> in C/C++, but it can be replaced by: x >>> 31 ==> (x >> 31) & 1
*/
/************************************************************************/
int cmp(int a, int b)
{
	return ( ((a - b) >> 31) | (((b - a) >> 31) & 1) );
}
int abs(int a)
{
	int temp = a >> 31;
	return (a + temp) ^ temp;
}


void testBitOper()
{
#ifdef BIT_OPER_TEST
	int i;
	//Test 1.
	int len = 10;
	char p[1024];
	char sc = (char)(0xC3 & 0xFF);
	for(i = 0; i < len; i++)
	{
		p[i] = sc + i;
	}
	
	int x = 0x898b8d8f;
	
	int byteIndex = 0, bitIndex = 0;
	
	if(find((unsigned char*)p, len, x, &byteIndex, &bitIndex))
	{
		printf("x = %d, y = %d\n", byteIndex, bitIndex);
	}
	else
	{
		printf("Failed to find\n");
	}
	
	//Test 2.
	showBasicTypeInfo();

	//Test
	int arrays[][2] = { { 1, 1 }, { -2, -2 }, { 0, 0 }, { 10, 20 },
	{ 20, 10 }, { -10, -20 }, { -20, -10 }, { 50, -5 }, { -5, 50 },
	{ 0x80000000, 0x80000000 + 100 },
	{ 0x7FFFFFFF, 0x7FFFFFFF - 100 },
	{ 0x7FFFFFFF, 0x80000000 },
	{ 0x80000000, 0x7FFFFFFF }};
	
	int length = sizeof(arrays) / sizeof(arrays[0][0]) / 2;
	//doz
	for(i = 0; i < length; i++)
	{
		printf("doz(%d, %d) = %d\n", arrays[i][0], arrays[i][1],
			doz(arrays[i][0], arrays[i][1]));
	}
	//min
	for(i = 0; i < length; i++)
	{
		printf("min(%d, %d) = %d\n", arrays[i][0], arrays[i][1],
			min(arrays[i][0], arrays[i][1]));
	}
	//max
	for(i = 0; i < length; i++)
	{
		printf("max(%d, %d) = %d\n", arrays[i][0], arrays[i][1],
			max(arrays[i][0], arrays[i][1]));
	}
	//cmp
	for(i = 0; i < length; i++)
	{
		printf("cmp(%d, %d) = %d\n", arrays[i][0], arrays[i][1],
			cmp(arrays[i][0], arrays[i][1]));
	}
	//abs
	for(i = 0; i < length; i++)
	{
		printf("abs(%d) = %d\n", arrays[i][0], abs(arrays[i][0]));
	}

	printSep(__FILE__);
#endif
}

#pragma warning( pop )