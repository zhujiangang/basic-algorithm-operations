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

void deleteList(node* ph)
{
	node* p = 0;
	while(ph)
	{
		p = ph;
		ph = ph->next;
		delete p;
	}
}
void printList(node* ph)
{
	while(ph)
	{
		cout<<ph->data<<" ";
		ph = ph->next;
	}
	cout<<endl;
}

node* merge(node* p1, node* p2)
{
	if(p1 == NULL)
	{
		return p2;
	}
	if(p2 == NULL)
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

node* mergeIter(node* p1, node* p2)
{
	if(p1 == NULL)
	{
		return p2;
	}
	if(p2 == NULL)
	{
		return p1;
	}
	
	node* ph = NULL;
	node* p = NULL;
	
	while(p1 != NULL && p2 != NULL)
	{
		if(p1->data <= p2->data)
		{
			if(p == NULL)
			{
				ph = p = p1;
			}
			else
			{
				p->next = p1;
				p = p->next;
			}
			p1 = p1->next;
		}
		else
		{
			if(p == NULL)
			{
				ph = p = p2;
			}
			else
			{
				p->next = p2;
				p = p->next;
			}
			p2 = p2->next;
		}
	}
	
	if(p1 != NULL)
	{
		p->next = p1;
	}
    else if(p2 != NULL)
	{
		p->next = p2;
	}

	return ph;
}

int compareList(node* p1, node* p2, node** pdiff1, node** pdiff2, int* pos)
{
	if(p1 == NULL && p2 == NULL)
	{
		return 0;
	}

	int ret = 0;
	int i = 0;
	while(p1 != NULL && p2 != NULL)
	{
		if(p1->data != p2->data)
		{
			if(pdiff1 != NULL && pdiff2 != NULL && pos != NULL)
			{
				*pdiff1 = p1;
				*pdiff2 = p2;
				*pos = i;
			}
			ret = p1->data - p2->data;
			if(ret > 0)
			{
				return 1;
			}
			else if(ret < 0)
			{
				return -1;
			}
		}
		p1 = p1->next;
		p2 = p2->next;
		i++;
	}

	if(p1 != NULL)
	{
		if(pdiff1 != NULL && pdiff2 != NULL && pos != NULL)
		{
			*pdiff1 = p1;
			*pdiff2 = p2;
			*pos = i;
		}
		return 1;
	}

	if(p2 != NULL)
	{
		if(pdiff1 != NULL && pdiff2 != NULL)
		{
			*pdiff1 = p1;
			*pdiff2 = p2;
			*pos = i;
		}
		return -1;
	}
	return 0;
}

int assertEquals(node* p1, node* p2)
{
	node* pdiff1 = NULL;
	node* pdiff2 = NULL;
	int pos = 0;

	int ret = 0;
	ret = compareList(p1, p2, &pdiff1, &pdiff2, &pos);

	if(ret == 0)
	{
		cout<<"EQUALS"<<endl;
	}
	else
	{
		cout<<"DIFFER -- ";
		cout<<"pos("<<pos<<") p1 = "<<pdiff1->data<<", p2 = "<<pdiff2->data<<endl;
	}
	return ret;
}