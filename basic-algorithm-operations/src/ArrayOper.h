#ifndef _ARRAY_OPER_H_
#define _ARRAY_OPER_H_

/************************************************************************/
/* This file contains the algorithms based on arrays                    */
/************************************************************************/


/************************************************************************/
/* 1. Print all the pairs in the array that their sum equals to input param "sum"
      This array is in order.
/************************************************************************/
void printSumPairs(int a[], int n, int sum);
void printSumPairsDup(int a[], int n, int sum);

/************************************************************************/
/* 2. Check an array if a post order of BST                             */
/************************************************************************/
bool checkSeqIfPostOrderOfBST(int a[], int n);

/************************************************************************/
/* 3. Binary search from an array                                       */
/************************************************************************/
int binarySearch(int a[], int from, int to, int key);

/************************************************************************/
/* 4. max sum of continuous sub-array                                   */
/************************************************************************/
int maxsumsub(int array[], int n, int* pStart = NULL, int* pEnd = NULL);
int maxsumsub_dp(int array[], int n, int* pStart = NULL, int* pEnd = NULL);

/*
5. http://blog.csdn.net/hackbuteer1/article/details/6889217
There are 2 numbers only occur in the array once, and other numbers occur twice.
The 2 numbers are different from each other. Find out them.
*/
void find2distinct(int a[], int n, int* x, int* y);

/*
6. http://blog.csdn.net/hackbuteer1/article/details/6889217
Same to the question "find2distinct", but there are 3 numbers only occur in once,
find out them
*/
void find3distinct(int a[], int n, int* x, int* y, int* z);

/*
7. http://blog.csdn.net/jcwkyl/article/details/3957941
Find the missing 2 numbers of the array
*/
void find2miss(int a[], int n, int* x, int* y);

/*
8. Longest Increasing Subsequence
*/
int lis(int a[], int n, int d[], int mem[]);

void reverse(int a[], int low, int high);
/*
9. Rotate to right
*/
void rotate1(int a[], int low, int high, int distance);
void rotate2(int a[], int low, int high, int distance);

/*
10. merge in place -- without extra space
*/
void merge(int a[], int l, int r, int m);

void testArrayOper();
#endif