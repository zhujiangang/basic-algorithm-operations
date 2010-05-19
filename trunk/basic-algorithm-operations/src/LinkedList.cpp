#include <iostream.h>
#include "LinkedList.h"

node* createList(int data[], int len)
{
	if(len <= 0)
	{
		return 0;
	}
	node* header = new node(data[0]);

	node* p = header;
	int i;
	for(i = 1; i < len; i++)
	{
		p->next = new node(data[i]);
		p = p->next;
	}

	return header;
}

void deleteList(node* header)
{
	node* p = 0;
	while(header)
	{
		p = header;
		header = header->next;
		delete p;
	}
}
void printList(node* header)
{
	while(header)
	{
		cout<<header->data<<" ";
		header = header->next;
	}
	cout<<endl;
}

node* merge(node* p1, node* p2)
{
	if(p1 == 0)
	{
		return p2;
	}
	if(p2 == 0)
	{
		return p1;
	}
	if(p1->data <= p2->data)
	{
		p1->next = merge(p1->next, p2);
		return p1;
	}
	else
	{
		p2->next = merge(p1, p2->next);
		return p2;
	}
}