#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct node
{
	int data;
	node *next;
	node(int dataValue = -1, node *nextValue = NULL)
		:data(dataValue),next(nextValue)
	{
	}
}node;

typedef struct cascade_node
{
	node* pnode;
	cascade_node *next;
	cascade_node(node* pnode_val = NULL, cascade_node* next_val = NULL)
		:pnode(pnode_val),next(next_val)
	{
	}
}cascade_node;

/************************************************************************/
/* 0. Basic Single linked list operations                               */
/************************************************************************/
node* createList(int data[], int len);
void deleteList(node* ph);
void printList(node* ph, int limit = -1);
void showNode(node* ph);
node* get(node* ph, int index);
int size(node* ph);
int compareList(node* p1, node* p2, node** pdiff1 = NULL, node** pdiff2 = NULL, int* pos = NULL);
int assertEquals(node* p1, node* p2);

/************************************************************************/
/* 1. Reverse a single linkedlist                                       */
/************************************************************************/
node* reverse(node* ph);

/************************************************************************/
/* 2. find the last "index"th node, from 0                              */
/************************************************************************/
node* rget(node* ph, int index);

/************************************************************************/
/* 3. find the middle node(s) of the list "ph"                           
if the node number of the list is odd, pMid1 is the mid node
else if the node number is even, pMid1 is the first mid node, and pMid2 is the second mid node                                                                    */
/************************************************************************/
void getMiddleNode(node* ph, node** pMid1, node** pMid2 = NULL);

/************************************************************************/
/* 4. Merge two single linked list                                      */
/************************************************************************/
node* merge(node* p1, node* p2);
node* mergeIter(node* p1, node* p2);
node* mergeIter1(node* p1, node* p2);

/************************************************************************/
/* 5. Check if a single linked list has loop. If yes, find the loop entrance
/************************************************************************/
node* checkListLoop(node* ph);
node* getListLoopEntrance(node* ph);

/************************************************************************/
/* 6. sort a single linked list                                         */
/************************************************************************/
node* qsort(node* ph);
node* qsort1(node* ph);
node* insert_sort(node* ph);
node* bubble_sort(node* ph);
node* select_sort(node* ph);

/************************************************************************/
/* 7. Check if two linked list overlapped. If yes, find the first overlapped node
/************************************************************************/
node* getFirstOverlappedNode(node* p1, node* p2);

/************************************************************************/
/* 8. swap 2 linked list node, p1 and p2 are supposed to be one of nodes in the linked list
/************************************************************************/
node* swap(node* ph, node* p1, node* p2);

node* swap1(node* ph, node* p1, node* p2);

void swap(node* prev1, node* p1, node* prev2, node* p2);

/************************************************************************/
/* 9. expand a cascade_node linked list to a simple linked list
/************************************************************************/
node* expand(cascade_node* pch);


/************************************************************************/
/* 10. Simulate the add operation of big number                         */
/************************************************************************/
node* add(node* ph1, int len1, node* ph2, int len2);

/************************************************************************/
/* 11. Remove the duplicated element in list                            */
/************************************************************************/
node* remove_duplicated(node* ph);

void testLinkedList();
#endif