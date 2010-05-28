#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

struct node
{
	int data;
	node *next;
	node(int dataValue = -1, node *nextValue = NULL)
		:data(dataValue),next(nextValue)
	{
	}
};

node* createList(int data[], int len);
void deleteList(node* ph);
void printList(node* ph, int limit = -1);
void showNode(node* ph);
node* get(node* ph, int index);
int size(node* ph);

int compareList(node* p1, node* p2, node** pdiff1 = NULL, node** pdiff2 = NULL, int* pos = NULL);
int assertEquals(node* p1, node* p2);

node* merge(node* p1, node* p2);
node* mergeIter(node* p1, node* p2);

node* reverse(node* ph);
node* checkListLoop(node* ph);
node* getListLoopEntrance(node* ph);

node* qsort(node* ph);
node* qsort1(node* ph);

//find the last "index"th node, from 0
node* rget(node* ph, int index);

//find the middle node(s) of the list "ph"
//if the node number of the list is odd, pMid1 is the mid node
//else if the node number is even, pMid1 is the first mid node, and pMid2 is the second mid node
void getMiddleNode(node* ph, node** pMid1, node** pMid2 = NULL);
#endif