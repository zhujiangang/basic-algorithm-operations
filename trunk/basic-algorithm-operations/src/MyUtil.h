#ifndef _MYUTIL_H_
#define _MYUTIL_H_

#include <time.h>

#define COUNTOF(ARRAY) ( sizeof(ARRAY)/sizeof(ARRAY[0]) )

void output(int a[], int begin, int end);
void output(int a[], int n);
void printSep();
void printSep(const char* str);
char* getFileName(const char* fullFileName);


void swap(int& x, int& y);

void genrand(int a[], int n, int limit = 0);
void genseq(int a[], int n, int start = 1);

void arryCopy(int src[], int srcPos, int dst[], int dstPos, int length);

int validateInOrder(int a[], int n, bool isAscending = true);

int bitsNum(int a);
unsigned int flp2(unsigned int x);

#endif