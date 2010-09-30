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

#endif