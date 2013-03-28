#include "config.h"
#include "EightQueen.h"
#include "MyUtil.h"
#include <stdio.h>

#define N	8

bool checkValid(int a[N][N], int row, int col)
{
	int i;
	//col check
	for(i = 0; i < row; i++)
	{
		if(a[i][col] == 1)
		{
			return false;
		}
	}

	//diagonal check
	int j;
	for(i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
	{
		if(a[i][j] == 1)
		{
			return false;
		}
	}

	//anti-diagonal check
	for(i = row - 1, j = col + 1; i >= 0 && j < N; i--, j++)
	{
		if(a[i][j] == 1)
		{
			return false;
		}
	}
	return true;
}

void printMatrix(int a[N][N])
{
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
}

void queen(int a[N][N], int i)
{
	static int count = 0;
	if(i >= N)
	{
		count++;
		printf("Solution: %d\n", count);
		printMatrix(a); 
		return;
	}

	for(int col = 0; col < N; col++)
	{
		//put chess at a[i][col];
		a[i][col] = 1;

		if(checkValid(a, i, col))
		{
			queen(a, i + 1);
		}

		//remove chess
		a[i][col] = 0;
	}
}

void testEightQueen()
{
#if ((EIGHT_QUEEN_TEST) == 1)
	int a[N][N] = {0};
	queen(a, 0);

	printSep(__FILE__);
#endif
}