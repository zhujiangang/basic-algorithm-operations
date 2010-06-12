#include <iostream.h>
#include <string.h>
#include <stdio.h>

#include "MyUtil.h"
#include "CombineMath.h"
#include "ArraySort.h"
#include "FileOper.h"
#include "LinkedList.h"
#include "StringOper.h"
#include "ArrayOper.h"
#include "StackQueue.h"
#include "BinTree.h"

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
	//(0). Set up linkedlist
	int data1[] = {9, 9, 9, 9, 9, 9, 9};
	int data2[] = {1};

	node* p1 = createList(data1, COUNTOF(data1));
	printList(p1);

	node* p2 = createList(data2, COUNTOF(data2));
	printList(p2);

	const int n = 10;
	int a[n];
	genrand(a, n, 100);
	
	node* p3 = createList(a, n);
 	printList(p3);

	node* pLastNode = get(p2, size(p2) - 1);
	node* pInterNode = get(p2, 2); 

	//(0.1). 
// 	node** ppNode = &pInterNode;
// 	showNode(*ppNode);
// 	*ppNode = (*ppNode)->next;
// 	printList(p2);
// 
// 	ppNode = &(p2->next->next);
// 	showNode(*ppNode);
// 	*ppNode = (*ppNode)->next;
// 	printList(p2);

	//(1). Test for loop
// 	pLastNode->next = pInterNode;
// 
// 	printList(p2, 20);
// 
// 	node* pMeetNode = NULL;
// 	pMeetNode = checkListLoop(p1);
// 	showNode(pMeetNode);
// 
// 	pMeetNode = checkListLoop(p2);
// 	showNode(pMeetNode);
// 
// 	node* pLoopEntrance = NULL;
// 	pLoopEntrance = getListLoopEntrance(p2);
// 	showNode(pLoopEntrance);
// 
// 	pLastNode->next = p2;
// 	printList(p2, 20);
// 
// 
// 	pLoopEntrance = getListLoopEntrance(p2);
// 	showNode(pLoopEntrance);
// 	pLastNode->next = NULL;

	//(2). Test for merge and reverse
// 	node* p = mergeIter(p1, p2);
// 	printList(p);
// 	
// 	assertEquals(p1, p2);
// 
// 	node* pr = reverse(p2);
// 	printList(pr);

	//(3). Test for qsort

// 	node* ph = createList(a, n);
// 	printList(ph);
// 	ph = qsort1(ph);
// 	printList(ph);
// 
// 	node* ph1 = createList(a, n);
// 	printList(ph1);
// 	ph1 = bubble_sort(ph1);
// 	printList(ph1);
// 
// 	assertEquals(ph, ph1);
// 
// 	node* ph2 = createList(a, n);
// 	printList(ph2);
// 	ph2 = insert_sort(ph2);
// 	printList(ph2);
// 
// 	assertEquals(ph, ph2);
// 	
// 	node* ph3 = createList(a, n);
// 	printList(ph3);
// 	ph3 = select_sort(ph3);
// 	printList(ph2);
// 	
// 	assertEquals(ph, ph3);

	node* result = add(p1, size(p1), p2, size(p2));
	printList(result);
	deleteList(result);
	
// 	node* rnthNode = rget(p1, 0);
// 	showNode(rnthNode);
// 
// 	rnthNode = rget(p1, 4);
// 	showNode(rnthNode);
// 
// 	rnthNode = rget(p1, 6);
// 	showNode(rnthNode);
// 
// 	node* pMid1 = NULL, *pMid2 = NULL;
// 	getMiddleNode(p1, &pMid1, &pMid2);
// 
// 	showNode(pMid1);
// 	showNode(pMid2);

// 	node* pOverLapped = NULL;
// 	pOverLapped = getFirstOverlappedNode(p1, p2);
// 	showNode(pOverLapped);
// 
// 	node* p1InterNode = get(p1, 4);
// 	showNode(p1InterNode);
// 	node* p2InterNode = get(p2, 9);
// 	showNode(p2InterNode);
// 
// 	node* p1Next = p1InterNode->next;
// 	p1InterNode->next = p2InterNode;
// 
// 	pOverLapped = getFirstOverlappedNode(p1, p2);
// 	showNode(pOverLapped);
// 
// 	//restore
// 	p1InterNode->next = p1Next;
// 
// 	p2 = swap(p2, get(p2, 4), get(p2, 5));
// 	printList(p2);

	
// 	cascade_node* pcn = NULL;
// 	pcn = new cascade_node(p1);
// 	pcn = new cascade_node(p2, pcn);
// 	pcn = new cascade_node(NULL, pcn);
// 	pcn = new cascade_node(p3, pcn);
// 
// 	node* pe = expand(pcn);
// 	printList(pe);

// 	deleteList(p1);	
// 	deleteList(p2);
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


void testArrayOper()
{
	const int n = 100;
	int a[n];
	genseq(a, n);
	genrand(a, n, 100);
	quickSort(a, n);
	output(a, n);

	printSumPairsDup(a, n, 60);
}


void testStackQueue()
{
	const int n = 10;
	int a[n];
	genrand(a, n, 100);
	output(a, n);

	int x = n/2;

	int i;
// 	StackWithMin st;
// 	for(i = 0; i < n; i++)
// 	{
// 		st.push(a[i]);
// 		cout<<st.min()<<" ";
// 	}
// 	cout<<endl;
// 	for(i = 0; i < n; i++)
// 	{
// 		cout<<st.pop()<<" ";
// 	}
// 	cout<<endl;
// 	
// 	QueueWith2Stack que;
// 	for(i = 0; i < n; i++)
// 	{
// 		que.enqueue(a[i]);
// 	}
// 	for(i = 0; i < n; i++)
// 	{
// 		cout<<que.dequeue()<<" ";
// 	}
// 	cout<<endl;
// 
// 
// 	StackWith2Queue st2;
// 	for(i = 0; i < n; i++)
// 	{
// 		st2.push(a[i]);
// 	}
// 	for(i = 0; i < n; i++)
// 	{
// 		cout<<st2.pop()<<" ";
// 	}
// 	cout<<endl;

// 	const int len = 5;
// 	int pushSeq[n] = {1, 2, 3, 4, 5};
// 	int popSeq[n] = {2, 4, 1, 5, 3};
// 	cout<<isPopSequence(pushSeq, popSeq, len)<<endl;
// 
// 	int len2 = 3;
	stack<int> st;
// 	for(i = 0; i < len2; i++)
// 	{
// 		st.push(i+1);
// 	}
// 	reverse(st);
// 	output(st);


	const int len = 10;
// 	int b[len] = {10, 2, 3, 1, 9, 8, 5, 4, 6, 7};
// 	empty(st);
// 	for(i = 0; i < len; i++)
// 	{
// 		st.push(b[i]);
// 	}
// 	sort(st);
// 	output(st);


	StackWith1Array sw1a(100);
	for(i = 0; i < 3; i++)
	{
		sw1a.push(1, i+1);
	}
	for(i = 0; i < 3; i++)
	{
		sw1a.push(2, (i+1) * 10);
	}
	for(i = 0; i < 6; i++)
	{
		sw1a.push(3, (i+1) * 100);
	}

	for(i = 1; i <= 3; i++)
	{
		while(sw1a.top(i) >= 0)
		{
			cout<<sw1a.pop(i)<<" ";
		}
		cout<<endl;
	}
}

void testBinTree()
{
	const int n = 10;
	int a[n];
	genseq(a, n);
	output(a, n);

	BinTree binTree(a, n);
	binTree.preOrder();
	binTree.preOrderStack();
	binTree.inOrder();
	binTree.inOrderStack();
	binTree.postOrder();
	binTree.postOrderStack();
	binTree.levelOrder();
	
	int pos = 1;
	BinNode* binNode = binTree.getNodeByID(binTree.getRoot(), 1);
	if(binNode != NULL)
	{
		if(binTree.getPosOfNode(binTree.getRoot(), binNode, &pos))
		{
			cout<<"Node="<<binNode->value<<", pos="<<pos<<endl;
		}
	}

	BinNode* binNode2 = binTree.getNodeByID(binTree.getRoot(), 4);
	BinNode* parentNode = binTree.findLatestParent(binNode, binNode2);
	if(parentNode != NULL)
	{
		cout<<"Parent("<<binNode->value<<","<<binNode2->value<<")="<<parentNode->value<<endl;
	}
	else
	{
		cout<<"Can't find parent"<<endl;
	}
}
int main(int argc, char* argv[])
{
//	testCombineMath();
//	testArraySort();
//	testFileOper();
//	testLinkedList();
//	testStringOper();
//	testArrayOper();
//	testStackQueue();
	testBinTree();
	return 0;
}

