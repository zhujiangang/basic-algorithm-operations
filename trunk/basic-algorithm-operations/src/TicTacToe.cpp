#include "config.h"
#include "TicTacToe.h"
#include <stdio.h>

#define NONE 0
#define X    1
#define O    2
#define DRAW 3

#define N    3

//Naive way to find the winner

int checkByRow(int a[N][N]);
int checkByCol(int a[N][N]);
int checkByDiag(int a[N][N]);
int checkByAntiDiag(int a[N][N]);
int ticTacToeNaive(int a[N][N])
{
	int i, j, w;

	w = checkByRow(a);
	if(w != NONE)
	{
		return w;
	}

	w = checkByCol(a);
	if(w != NONE)
	{
		return w;
	}

	w = checkByDiag(a);
	if(w != NONE)
	{
		return w;
	}

	w = checkByAntiDiag(a);
	if(w != NONE)
	{
		return w;
	}

	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			if(a[i][j] == NONE)
			{
				return NONE;
			}
		}
	}
	return DRAW;
}
int checkByRow(int a[N][N])
{
	int i, j, w;
	for(i = 0; i < N; i++)
	{
		w = a[i][0];
		if(w == NONE)
		{
			continue;
		}
		for(j = 1; j < N; j++)
		{
			if(a[i][j] != w)
			{
				break;
			}
		}
		if(j == N)
		{
			return w;
		}
	}
	return NONE;
}
int checkByCol(int a[N][N])
{
	int i, j, w;
	for(j = 0; j < N; j++)
	{
		w = a[0][j];
		if(w == NONE)
		{
			continue;
		}
		for(i = 1; i < N; i++)
		{
			if(a[i][j] != w)
			{
				break;
			}
		}
		if(i == N)
		{
			return w;
		}
	}
	return NONE;
}

int checkByDiag(int a[N][N])
{
	int w = a[0][0];
	if(w == NONE)
	{
		return NONE;
	}
	int i;
	for(i = 1; i < N; i++)
	{
		if(a[i][i] != w)
		{
			break;
		}
	}
	if(i == N)
	{
		return w;
	}
	return NONE;
}
int checkByAntiDiag(int a[N][N])
{
	int w = a[0][N - 1];
	if(w == NONE)
	{
		return NONE;
	}
	int i;
	for(i = 1; i < N; i++)
	{
		if(a[i][N - 1 - i] != w)
		{
			break;
		}
	}
	if(i == N)
	{
		return w;
	}
	return NONE;
}

int checkLine(int a[N][N], int startRow, int startCol, int dirRow, int dirCol);
int ticTacToeDir(int a[N][N])
{
	//row, col
	int i, w;
	for(i = 0; i < N; i++)
	{
		//row
		w = checkLine(a, i, 0, 0, 1);
		if(w != NONE)
		{
			return w;
		}

		//col
		w = checkLine(a, 0, i, 1, 0);
		if(w != NONE)
		{
			return w;
		}
	}

	//diagonal
	w = checkLine(a, 0, 0, 1, 1);
	if(w != NONE)
	{
		return w;
	}

	//anti diagonal
	w = checkLine(a, N - 1, 0, -1, 1);
	if(w != NONE)
	{
		return w;
	}

	int j;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			if(a[i][j] == NONE)
			{
				return NONE;
			}
		}
	}
	return DRAW;
}

int checkLine(int a[N][N], int startRow, int startCol, int dirRow, int dirCol)
{
	int w = a[startRow][startCol];
	if(w == NONE)
	{
		return NONE;
	}
	int i = startRow + dirRow;
	int j = startCol + dirCol;
	for( ; i < N && j < N; i += dirRow, j += dirCol)
	{
		if(a[i][j] != w)
		{
			break;
		}
	}
	if(i >= N || j >= N)
	{
		return w;
	}
	return NONE;
}

int ticTacToeOn(int a[N][N])
{
	int rowCount[N] = {0};
	int colCount[N] = {0};
	int diag = 0, antiDiag = 0, steps = 0;

	int i, row, col;
	for(i = 0; i < N * N; i++)
	{
		row = i / N;
		col = i % N;
		if(a[row][col] == X)
		{
			rowCount[row]++;
			colCount[col]++;
			if(row == col)
			{
				diag++;
			}
			//diagonal and anti diagonal could exist at one time
			if( (row + col + 1) == N)
			{
				antiDiag++;
			}

			steps++;

			if(rowCount[row] == N || colCount[col] == N || diag == N || antiDiag == N)
			{
				return X;
			}
		}
		else if(a[row][col] == O)
		{
			rowCount[row]--;
			colCount[col]--;
			if(row == col)
			{
				diag--;
			}
			if( (row + col + 1) == N )
			{
				antiDiag--;
			}
			
			steps++;

			if(rowCount[row] == -N || colCount[col] == -N || diag == -N || antiDiag == -N)
			{
				return O;
			}
		}
	}

	if(steps == N * N)
	{
		return DRAW;
	}

	return NONE;
}

typedef int (*PlayPtr)(int a[N][N]);

void printBoard(int a[N][N])
{
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			printf("%2d ", a[i][j]);
		}
		printf("\n");
	}
}

void testTicTacToe()
{
#if ((TIC_TAC_TOE_TEST) == 1)

	PlayPtr impls[] = {ticTacToeNaive, ticTacToeDir, ticTacToeOn};
	int implCount = sizeof(impls)/sizeof(impls[0]);

	int boards[][N][N] = 
	{
		{ {X, X, X}, {NONE, O, O}, {O, O, NONE} },
		{ {NONE, NONE, NONE}, {O, O, O}, {X, X, NONE} },
		{ {X, O, X}, {O, X, O}, {O, X, O} },
		{ {NONE, NONE, X}, {NONE, X, O}, {X, O, O} },
		{ {O, NONE, X}, {NONE, X, O}, {NONE, X, O} },
		{ {O, X, X}, {NONE, X, O}, {NONE, X, O} }
	};

	int len = sizeof(boards)/sizeof(boards[0]);
	
	int i, j, w;
	for(i = 0; i < len; i++)
	{
		printBoard(boards[i]);
		printf("(%d) Result: ", i);
		for(j = 0; j < implCount; j++)
		{
			w = (impls[j])(boards[i]);
			printf("%2d ", w);
		}
		printf("\n");
	}
#endif
}