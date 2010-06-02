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

void showNode(node* ph)
{
	if(ph == NULL)
	{
		cout<<"NULL"<<endl;
	}
	else
	{
		cout<<ph->data<<endl;
	}
}
void printList(node* ph, int limit)
{
	if(limit < 0)
	{
		while(ph)
		{
			cout<<ph->data<<" ";
			ph = ph->next;
		}
		cout<<endl;
	}
	else
	{
		int i = 0;

		while(ph && i < limit)
		{
			cout<<ph->data<<" ";
			ph = ph->next;
			i++;
		}
		cout<<endl;
	}
}

int size(node* ph)
{
	int i = 0;
	while(ph != NULL)
	{
		i++;
		ph = ph->next;
	}
	return i;
}

node* get(node* ph, int index)
{
	if(index < 0)
	{
		return NULL;
	}

	int i = 0;
	while(ph != NULL)
	{
		if(i == index)
		{
			return ph;
		}
		ph = ph->next;
		i++;
	}
	return NULL;
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

node* reverse(node* ph)
{
	node* prev = NULL;
	node* curr = ph;
	node* next = NULL;

	while(curr != NULL)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	return prev;
}

node* checkListLoop(node* ph)
{
	node* p1 = ph;
	node* p2 = ph;

	while(p1 != NULL && p2 != NULL)
	{
		p1 = p1->next;
		if(p2->next == NULL)
		{
			return NULL;
		}
		p2 = p2->next->next;

		if(p1 == p2)
		{
			return p1;
		}
	}
	return NULL;
}
node* getListLoopEntrance(node* ph)
{
	node* p1 = ph;
	node* p2 = ph;
	
	while(p1 != NULL && p2 != NULL)
	{
		p1 = p1->next;
		//no loop
		if(p2->next == NULL)
		{
			return NULL;
		}
		p2 = p2->next->next;
		
		//Found encountering point
		if(p1 == p2)
		{
			break;
		}
	}

	//no loop
	if(p1 == NULL || p2 == NULL)
	{
		return NULL;
	}

	p1 = ph;
	while(p1 != p2)
	{
		p1 = p1->next;
		p2 = p2->next;
	}
	return p1;
}


node* partition(node** left, node* right)
{
	int key = (*left)->data;
	node* pivot = *left;
	
	node** ptr = &(*left)->next; 
	while (*ptr != right)
	{
		//move the node "*ptr" to the left of the node "*left"
		if ( (*ptr)->data < key )
		{
			node *tmp = *ptr;
			(*ptr) = tmp->next;
			tmp->next = (*left);
			(*left) = tmp;
		}
		//go to the next node
		else
		{
			ptr=&(*ptr)->next;
		}
	}
	return pivot;
}

void qsort(node **left, node *right)
{
	if(*left!=right && (*left)->next!=right)
	{
		node *tmp = partition(left, right);
		qsort(left, tmp);
		qsort(&tmp->next, right);
	}
}

node* qsort(node* ph)
{
	qsort(&ph, NULL);
	return ph;
}

node* partition1(node** left, node* right)
{
	int key = (*left)->data;
	node* pivot = *left;
	
	node* ptr = (*left)->next;
	node* prev_ptr = *left;
	while (ptr != right)
	{
		//move the node "*ptr" to the left of the node "*left"
		if ( ptr->data < key )
		{
			prev_ptr->next = ptr->next;
			
			node *tmp = ptr->next;			
			ptr->next = (*left);
			(*left) = ptr;
			
			ptr = tmp;

			//No need to update prev_ptr in this case.
		}
		//go to the next node
		else
		{
			prev_ptr = ptr;
			ptr = ptr->next;
		}
	}
	return pivot;
}

void qsort1(node **left, node *right)
{
	if(*left!=right && (*left)->next!=right)
	{
		node *tmp = partition1(left, right);
		qsort1(left, tmp);
		qsort1(&tmp->next, right);
	}
}

node* qsort1(node* ph)
{
	qsort1(&ph, NULL);
	return ph;
}

node* insert_sort(node* ph)
{
	//empty or one node
	if(ph == NULL || ph->next == NULL)
	{
		return ph;
	}

	//set a dummy header node to make changing list header easier (no need to extra check when insert node before head)
	node dummyHeader;
	dummyHeader.next = ph;

	node *pi, *prev_pi, *pj, *prev_pj;
	for(prev_pi = ph, pi = ph->next; pi != NULL; )
	{
		for(prev_pj = &dummyHeader, pj = dummyHeader.next; pj != pi; prev_pj = pj, pj = pj->next)
		{
			if(pi->data < pj->data)
			{
				break;
			}
		}
		if(pj != pi)
		{
			prev_pi->next = pi->next;
			pi->next = pj;
			prev_pj->next = pi;

			pi = prev_pi->next;
			//prev_pi remain no change
		}
		else
		{
			prev_pi = pi;
			pi = pi->next;
		}
	}

	return dummyHeader.next;
}

node* bubble_sort(node* ph)
{
	if(ph == NULL || ph->next == NULL)
	{
		return ph;
	}
	node dummy;
	dummy.next = ph;

	node *prev_pi, *pi, *prev_pj, *pj, *tmp;
	for(prev_pi = &dummy, pi = dummy.next; pi != NULL; prev_pi = pi, pi = pi->next)
	{
		
		for(prev_pj = pi, pj = pi->next; pj != NULL; )
		{
			if(pj->data < pi->data)
			{
				swap(prev_pi, pi, prev_pj, pj);
				
				tmp = pi;
				pi = pj;
				prev_pj = tmp;
				pj = prev_pj->next;
			}
			else
			{
				prev_pj = pj;
				pj = pj->next;
			}
		}
	}
	return dummy.next;
}

node* select_sort(node* ph)
{
	if(ph == NULL || ph->next == NULL)
	{
		return ph;
	}
	node dummy;
	dummy.next = ph;

	return dummy.next;
}

node* rget(node* ph, int index)
{
	int i = 0;
	node* pEnd = ph;
	while(pEnd != NULL && i < index)
	{
		pEnd = pEnd->next;
		i++;
	}

	if(pEnd == NULL)
	{
		return NULL;
	}

	node* pStart = ph;
	while(pEnd->next != NULL)
	{
		pEnd = pEnd->next;
		pStart = pStart->next;
	}

	return pStart;
}

void getMiddleNode(node* ph, node** pMid1, node** pMid2)
{
	if(ph == NULL)
	{
		*pMid1 = NULL;
		*pMid2 = NULL;
		return;
	}
	node* p1 = ph;
	node* p2 = ph;
	while(true)
	{
		//odd number
		if(p2->next == NULL)
		{
			*pMid1 = p1;
			*pMid2 = NULL;
			return;
		}
		//even number
		else if(p2->next->next == NULL)
		{
			*pMid1 = p1;
			*pMid2 = p1->next;
			return;
		}
		else
		{
			p1 = p1->next;
			p2 = p2->next->next;
		}
	}
}

/************************************************************************/
/* 7. Check if two linked list overlapped. If yes, find the first overlapped node
/************************************************************************/
node* getFirstOverlappedNode(node* p1, node* p2)
{
	if(p1 == NULL || p2 == NULL)
	{
		return NULL;
	}

	int len1 = 1, len2 = 1;

	node* curr1 = p1;
	while(curr1->next != NULL)
	{
		len1++;
		curr1 = curr1->next;
	}

	node* curr2 = p2;
	while(curr2->next != NULL)
	{
		len2++;
		curr2 = curr2->next;
	}

	//The last node is different, the two linked list doesn't overlap
	if(curr1 != curr2)
	{
		return NULL;
	}

	int i;
	curr1 = p1;
	curr2 = p2;
	//overlapped, find the first overlapped node
	if(len1 > len2)
	{
		//forward len1-len2 steps
		for(i = 0; i < len1 - len2; i++)
		{
			curr1 = curr1->next;
		}
	}
	else if(len1 < len2)
	{
		for(i = 0; i < len2 - len1; i++)
		{
			curr2 = curr2->next;
		}
	}

	while(curr1 != NULL && curr2 != NULL)
	{
		if(curr1 == curr2)
		{
			break;
		}
		curr1 = curr1->next;
		curr2 = curr2->next;
	}
	return curr1;
}

/************************************************************************/
/* 8. swap 2 linked list node
/************************************************************************/
node* swap(node* ph, node* p1, node* p2)
{
	if(ph == NULL || p1 == NULL || p2 == NULL)
	{
		return ph;
	}
	if(p1 == p2)
	{
		return ph;
	}

	node* prev1 = NULL, *prev2 = NULL;
	node* curr = ph;
	while(curr != NULL)
	{
		if(curr->next == p1)
		{
			prev1 = curr;
			if(prev2 != NULL)
			{
				break;
			}
			if(p2 == ph)
			{
				break;
			}
		}
		else if(curr->next == p2)
		{
			prev2 = curr;
			if(prev1 != NULL)
			{
				break;
			}
			if(p1 == ph)
			{
				break;
			}
		}
		curr = curr->next;
	}

	//the p1 or p2 is not a valid node in the list
	if( (prev1 == NULL && p1 != ph) || (prev2 == NULL && p2 != ph) )
	{
		return ph;
	}

	node* next1 = p1->next;
	node* next2 = p2->next;

	//p1 is the header
	if(p1 == ph)
	{
		if(p1->next == p2)
		{
			p1->next = next2;
			p2->next = p1;

			ph = p2;						
		}
		else
		{
			p1->next = next2;
			prev2->next = p1;
			p2->next = next1;

			ph = p2;				
		}
	}
	//p2 is the header
	else if(p2 == ph)
	{
		if(p2->next == p1)
		{
			p2->next = next1;
			p1->next = p2;

			ph = p1;
		}
		else
		{
			p2->next = next1;
			prev1->next = p2;
			p1->next = next2;

			ph = p1;
		}
	}
	//neither p1 nor p2 is header
	else
	{
		//p1 is the previous node of p2
		if(p1->next == p2)
		{
			p1->next = next2;
			p2->next = p1;
			prev1->next = p2;
		}
		//p2 is the previous node of p1
		else if(p2->next == p1)
		{
			p2->next = next1;
			p1->next = p2;
			prev2->next = p1;
		}
		//neither
		else
		{
			prev1->next = p2;
			p2->next = next1;
			
			prev2->next = p1;
			p1->next = next2;
		}
	}
	return ph;
}

/************************************************************************/
/* add an extra header node                                             */
/************************************************************************/
node* swap1(node* ph, node* p1, node* p2)
{
	if(ph == NULL || p1 == NULL || p2 == NULL)
	{
		return ph;
	}
	if(p1 == p2)
	{
		return ph;
	}

	node dummy;
	dummy.next = ph;

	node* prev1 = NULL, *prev2 = NULL;
	node* curr = &dummy;
	while(curr != NULL)
	{
		if(curr->next == p1)
		{
			prev1 = curr;
			if(prev2 != NULL)
			{
				break;
			}
		}
		else if(curr->next == p2)
		{
			prev2 = curr;
			if(prev1 != NULL)
			{
				break;
			}
		}
		curr = curr->next;
	}

	if(p1->next == p2)
	{
		p1->next = p2->next;
		p2->next = p1;
		prev1->next = p2;
	}
	else if(p2->next == p1)
	{
		p2->next = p1->next;
		p1->next = p2;
		prev2->next = p1;
	}
	else
	{
		curr = p1->next;

		p1->next = p2->next;
		p2->next = curr;
		prev1->next = p2;
		prev2->next = p1;
	}

	return dummy.next;
}

void swap(node* prev1, node* p1, node* prev2, node* p2)
{
	if(p1->next == p2)
	{
		p1->next = p2->next;
		p2->next = p1;
		prev1->next = p2;
	}
	else if(p2->next == p1)
	{
		p2->next = p1->next;
		p1->next = p2;
		prev2->next = p1;
	}
	else
	{
		node* tmp = p1->next;

		p1->next = p2->next;
		p2->next = tmp;
		prev1->next = p2;
		prev2->next = p1;
	}	
}


/************************************************************************/
/* 9. expand a cascade_node linked list to a simple linked list
/************************************************************************/
node* expand(cascade_node* pch)
{
	if(pch == NULL)
	{
		return NULL;
	}

	node* ph = NULL, *curr = NULL, *prev = NULL;

	while(pch != NULL)
	{
		curr = pch->pnode;

		if(curr == NULL)
		{
			pch = pch->next;
			continue;
		}

		if(prev == NULL)
		{
			ph = prev = curr;
		}
		else
		{
			prev->next = curr;
		}

		while(curr != NULL && curr->next != NULL)
		{
			curr = curr->next;
		}
		prev = curr;

		pch = pch->next;
	}
	return ph;
}
