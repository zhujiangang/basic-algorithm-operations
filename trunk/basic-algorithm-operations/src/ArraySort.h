#ifndef _ARRAY_SORT_H
#define _ARRAY_SORT_H

typedef void (*sorter)(int a[], int n);

void insertSort(int a[], int n);
void bubbleSort(int a[], int n);
void mergeSort(int a[], int n);
void quickSort(int a[], int n);
void heapSort(int a[], int n);

#endif