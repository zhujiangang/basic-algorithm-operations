#include <iostream.h>
#include <string.h>
#include <stdio.h>

#include "MyUtil.h"
#include "CombineMath.h"
#include "ArraySort.h"
#include "FileOper.h"
#include "LinkedList.h"
#include "StringOper.h"

void testCombineMath()
{
	const int n = 2;
	const int m = 1;
	int a[n];
	int b[n];
	
	genseq(a, n);
//	genrand(a, n, 100);
	output(a, n);
	cout<<endl;

	perm(a, 0, n - 1);
	cout<<endl;

// 	combine(a, n, 0, b, 0);
//	cout<<endl;

 	combine1(a, n, m, b, m, 0);
	cout<<endl;

// 	combine2(a, n, m, b, m);
//	cout<<endl;
	
	int used[n];
	memset(used, 0, n * sizeof(int));
	perm(a, n, m, b, 0, used);
	cout<<endl;
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
	int data2[] = {1, 3, 4, 8, 18, 30, 50, 60, 70};

	node* p1 = createList(data1, COUNTOF(data1));
	printList(p1);

	node* p2 = createList(data2, COUNTOF(data2));
	printList(p2);

	node* pLastNode = get(p2, size(p2) - 1);
	node* pInterNode = get(p2, 2); 

	pLastNode->next = pInterNode;

	printList(p2, 12);

	node* pMeetNode = NULL;
	pMeetNode = checkListLoop(p1);
	cout<<pMeetNode<<endl;

	pMeetNode = checkListLoop(p2);
	cout<<pMeetNode<<endl;
	if(pMeetNode != NULL)
	{
		cout<<pMeetNode->data<<endl;
	}
// 	node* p = mergeIter(p1, p2);
// 	printList(p);
	
//	assertEquals(p1, p2);

//	node* pr = reverse(p2);
//	printList(pr);

	deleteList(p1);

	pLastNode->next = NULL;
	deleteList(p2);
}

void testStringOper()
{
	char string1[60] = "The quick brown dog jumps over the lazy fox";
	char string2[60] = "The quick brown fox jumps over the lazy dog";
	/*                           1         2         3         4         5
	*                   12345678901234567890123456789012345678901234567890
	*/

	printf( "Function:\tmemcpy without overlap\n" );
	printf( "Source:\t\t%s\n", string1 + 40 );
	printf( "Destination:\t%s\n", string1 + 16 );
	gmemcpy( string1 + 16, string1 + 40, 3 );
	printf( "Result:\t\t%s\n", string1 );
	printf( "Length:\t\t%d characters\n\n", strlen( string1 ) );
	
	/* Restore string1 to original contents */
	gmemcpy( string1 + 16, string2 + 40, 3 );
	
	printf( "Function:\tmemmove with overlap\n" );
	printf( "Source:\t\t%s\n", string2 + 4 );
	printf( "Destination:\t%s\n", string2 + 10 );
	gmemmove( string2 + 10, string2 + 4, 40 );
	printf( "Result:\t\t%s\n", string2 );
	printf( "Length:\t\t%d characters\n\n", strlen( string2 ) );
	
	printf( "Function:\tmemcpy with overlap\n" );
	printf( "Source:\t\t%s\n", string1 + 4 );
	printf( "Destination:\t%s\n", string1 + 10 );
	gmemcpy( string1 + 10, string1 + 4, 40 );
	printf( "Result:\t\t%s\n", string1 );
	printf( "Length:\t\t%d characters\n\n", strlen( string1 ) );

}
int main(int argc, char* argv[])
{
	testCombineMath();
//	testArraySort();
//	testFileOper();
//	testLinkedList();
//	testStringOper();
	return 0;
}

