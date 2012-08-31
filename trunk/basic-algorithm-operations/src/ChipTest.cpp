// ChipTest.cpp: implementation of the CChipTest class.
//
//////////////////////////////////////////////////////////////////////

#include "config.h"
#include "ChipTest.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChipTest::CChipTest()
{

}

CChipTest::~CChipTest()
{

}

unsigned char CChipTest::Test(char chips[], int x, int y)
{
	//both good chips
	if( (chips[x] & 0x1) && (chips[y] & 0x1) )
	{
		return (unsigned char)0x11;
	}
	
	srand( (unsigned int)(time(NULL)) );

	//x is good, y is bad
	if(chips[x] & 01)
	{
		return (unsigned char)((rand() % 2) << 1);
	}
	//y is good, x is bad
	if(chips[y] & 01)
	{
		return (unsigned char)((rand() % 2));
	}

	//both bad
	return (unsigned char)(((rand() % 2) << 1) | ((rand() % 2)));
}
int CChipTest::FindGood(char chips[], int n)
{
	if(n <= 2)
	{
		return 0;
	}

	int isolated = -1;
	int* candidates = new int[n];
	int candidateCount = n;
	int i, x, y, result, count;
	for(i = 0; i < n; i++)
	{
		candidates[i] = i;
	}
	
	unsigned char testResult = 0;
	do 
	{
		//odd number
		if(candidateCount & 0x1)
		{
			isolated = candidateCount - 1;
			candidateCount--;
		}

		/*
		if the two chips test result is both good, 
		insert any one of them into the candidate set
		*/
		count = 0;
		for(i = 0; i < candidateCount; i += 2)
		{
			x = candidates[i];
			y = candidates[i + 1];
			testResult = Test(chips, x, y);
			if(testResult == (unsigned char)0x11)
			{
				candidates[count++] = x;
			}
		}
		//new candidate count
		candidateCount = count;

		//This happens when the last chip test cycle all occurs between good and bad chips
		if(candidateCount == 0)
		{
			assert(isolated >= 0);
			result = isolated;
			break;
		}
		else if(candidateCount == 1)
		{
			result = candidates[0];
			break;
		}

	} while (candidateCount > 1);

	delete [] candidates;

	return result;
}


void testChipTest()
{
#if CHIP_TEST == 1
	const int n = 1000001;
	int i, j, count, result;
//	char chips[] = {0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0};
	char chips[n];

	for(j = 0; j < 100; j++)
	{
		srand( (unsigned int)(time(NULL)) );
		for(i = 0, count = 0; i < n; i++)
		{
			chips[i] = (char)(rand() % 2);
			if(chips[i])
			{
				count++;
			}
		}
		for(i = n - 1; i >= 0 && count <= n / 2; i--)
		{
			if(chips[i] == 0)
			{
				chips[i] = 1;
				count++;
			}
		}
		
		result = CChipTest::FindGood(chips, sizeof(chips)/sizeof(chips[0]));
		printf("Good chip: %d - %d\n", result, (int)chips[result]);
	}
#endif
}
