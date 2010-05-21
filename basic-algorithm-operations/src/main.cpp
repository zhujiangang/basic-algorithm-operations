#include <iostream.h>

#include "MyUtil.h"
#include "CombineMath.h"
#include "ArraySort.h"
#include "FileOper.h"
#include "LinkedList.h"

void testCombineMath()
{
	const int n = 4;
	int a[n];
	genseq(a, n);

	output(a, n);
	perm(a, 0, n - 1);
}


void testArraySort()
{
	const int n = 20000;
	int datasource[n], copya[n];
	genrand(datasource, n, n);
//	output(datasource, n);

	arryCopy(datasource, 0, copya, 0, n);
//	output(copya, n);

	sorter sorters[] = {bubbleSort, insertSort, mergeSort, quickSort, heapSort};
	
	int ret;
	clock_t start, finish;
	for(int i = 0; i < COUNTOF(sorters); i++)
	{
		start = clock();
		sorters[i](copya, n);
		finish = clock();

		if( (ret = validateInOrder(copya, n)) != -1)
		{
			cout<<"Failed ("<<i<<": "<<ret<<endl;
			//output(copya, n);
			break;
		}
		else
		{
			cout<<"Success ("<<i<<") time = "<<(finish - start)<<"("<<(double)(finish - start)/CLOCKS_PER_SEC<<"s)"<<endl;
		}
		arryCopy(datasource, 0, copya, 0, n);
	}
}

void testFileOper()
{
//	FindFile("C:\\Temp");

	const char* filename = "C:\\Temp\\int.txt";

	writeIntToFile(filename, 0x12345678);
	
	int x;
	readIntFromFile(filename, &x);
	cout<<hex<<x<<endl;
}

void testLinkedList()
{
	int data1[] = {1, 3, 15, 27, 29};
	int data2[] = {1, 3, 4, 8, 18, 30};

	node* p1 = createList(data1, COUNTOF(data1));
	printList(p1);

	node* p2 = createList(data2, COUNTOF(data2));
	printList(p2);

// 	node* p = mergeIter(p1, p2);
// 	printList(p);
	
	assertEquals(p1, p2);
}
int main(int argc, char* argv[])
{
//	testCombineMath();
//	testArraySort();
//	testFileOper();
	testLinkedList();

	return 0;
}

