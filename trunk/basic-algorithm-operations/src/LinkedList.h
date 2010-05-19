#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct node
{
	int data;
	struct node *next;
	node(int dataValue, struct node *nextValue = NULL):data(dataValue),next(nextValue)
	{
	}
}node;

node* createList(int data[], int len);
void deleteList(node* header);
void printList(node* header);

node* merge(node* p1, node* p2);
#endif