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
node* get(node* ph, int index);
int size(node* ph);

int compareList(node* p1, node* p2, node** pdiff1 = NULL, node** pdiff2 = NULL, int* pos = NULL);
int assertEquals(node* p1, node* p2);

node* merge(node* p1, node* p2);
node* mergeIter(node* p1, node* p2);

node* reverse(node* ph);
node* checkListLoop(node* ph);

node* qsort(node* ph);
#endif