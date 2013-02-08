#pragma warning(disable:4786)

#include <iostream.h>
#include <map>
#include <utility>
#include <queue>
#include "LinkedList.h"
#include "LinkedListEx.h"
#include "MyUtil.h"
#include "config.h"

using std::map;
using std::make_pair;

/*
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
*/
node* createList(int data[], int len)
{
	node *head = NULL, **curr = &head;
	for(int i = 0; i < len; i++)
	{
		*curr = new node(data[i]);
		curr = &(*curr)->next;
	}
	return head;
}

/*
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
*/

void deleteList(node* ph)
{
	node *curr = ph;
	while(curr)
	{
		ph = curr;
		curr = curr->next;
		delete ph;
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

void swapadjacent(node** head)
{
	node **curr = head, *next;
	while(*curr)
	{
		next = (*curr)->next;
		if(next == NULL)
		{
			break;
		}
		(*curr)->next = next->next;
		next->next = *curr;
		*curr = next;

		curr = &(*curr)->next->next;
	}
}

node* swapadjacent(node* head)
{
	node *prev = NULL, *curr = head, *next;
	while(curr)
	{
		next = curr->next;
		if(next == NULL)
		{
			break;
		}

		curr->next = next->next;
		next->next = curr;
		if(prev == NULL)
		{
			head = next;
		}
		else
		{
			prev->next = next;
		}

		prev = curr;
		curr = curr->next;
	}

	return head;
}

/*
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
*/

int compareList(node* p1, node* p2, node** pdiff1, node** pdiff2, int* pos)
{
	int ret = 0, i = 0;
	node *prev1 = NULL, *curr1 = p1;
	node *prev2 = NULL, *curr2 = p2;

	while((curr1 == curr2) && (curr1 != NULL))
	{
		prev1 = curr1, curr1 = curr1->next;
		prev2 = curr2, curr2 = curr2->next;
		i++;
	}

	//equals
	if(curr1 == curr2)
	{
		return 0;
	}

	if(pdiff1 && pdiff2 && pos)
	{
		*pdiff1 = curr1;
		*pdiff2 = curr2;
		*pos = i;
	}

	if(curr1 == NULL)
	{
		return -1;
	}
	else if(curr2 == NULL)
	{
		return 1;
	}
	else
	{
		return curr1->data - curr2->data;
	}
}

node* remove(node* head, int x)
{
	node *prev = NULL, *curr = head, *next;
	while(curr != NULL)
	{
		next = curr->next;
		if(curr->data == x)
		{
			if(prev)
			{
				prev->next = next;
			}
			else
			{
				head = next;
			}
			delete curr;
		}
		else
		{
			prev = curr;
		}
		curr = next;
	}

	return head;
}
void remove(node** head, int x)
{
	for(node** curr = head; *curr; )
	{
		node* entry = *curr;
		if(entry->data == x)
		{
			*curr = entry->next;
			delete entry;
		}
		else
		{
			curr = &entry->next;
		}
	}
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

node* mergeIter1(node* p1, node* p2)
{
	if(p1 == NULL)
	{
		return p2;
	}
	if(p2 == NULL)
	{
		return p1;
	}
	node head;
	node* p = &head;

	while(p1 != NULL && p2 != NULL)
	{
		if(p1->data <= p2->data)
		{
			p->next = p1;
			p = p->next;
			p1 = p1->next;
		}
		else
		{
			p->next = p2;
			p = p->next;
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

	return head.next;
}

node* mergeIter2(node* p1, node* p2)
{
	node *ph, *p3;
	if(p1->data <= p2->data)
	{
		ph = p1;
		p1 = p1->next;
	}
	else
	{
		ph = p2;
		p2 = p2->next;
	}

	p3 = ph;
	while(p1 != NULL && p2 != NULL)
	{
		if(p1->data <= p2->data)
		{
			p3->next = p1;
			p1 = p1->next;
		}
		else 
		{
			p3->next = p2;
			p2 = p2->next;
		}
		p3 = p3->next;
	}

	if(p1 != NULL)
	{
		p3->next = p1;
	}
	if(p2 != NULL)
	{
		p3->next = p2;
	}
	return ph;
}

node* mergeIter3(node* p1, node* p2)
{
	node head;
	head.next = NULL;

	node *p3 = &head;
	while(p1 != NULL && p2 != NULL)
	{
		if(p1->data <= p2->data)
		{
			p3->next = p1;
			p1 = p1->next;
		}
		else
		{
			p3->next = p2;
			p2 = p2->next;
		}
		p3 = p3->next;
	}
	if(p1 != NULL)
	{
		p3->next = p1;
	}
	else if(p2 != NULL)
	{
		p3->next = p2;
	}
	return head.next;
}

node* merge_sort(node* ph)
{
	if(ph == NULL)
	{
		return ph;
	}

	node *curr = ph, *next;

	std::queue<node*> que;
	que.push(curr);
	
	while(curr != NULL)
	{
		next = curr->next;
		while(next != NULL && curr->data <= next->data)
		{
			curr = next;
			next = next->next;
		}

		if(next == NULL)
		{
			break;
		}

		curr->next = NULL;
		curr = next;
		que.push(curr);
	}

	curr = NULL;

	node *merged;
	while(!que.empty())
	{
		curr = que.front();
		que.pop();

		if(que.empty())
		{
			break;
		}

		next = que.front();
		que.pop();

		merged = mergeIter3(curr, next);
		que.push(merged);
	}
	return curr;
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

/*
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
*/

//two star @ http://wordaligned.org/articles/two-star-programming
node* partition(node** left, node* right)
{
	//int key = (*left)->data;
	node* pivot = *left;

	for(node **curr = &(*left)->next; *curr != right; )
	{
		node* entry = *curr;
		if(entry->data < pivot->data)
		{
			*curr = entry->next;
			entry->next = *left;
			*left = entry;
		}
		else
		{
			curr = &entry->next;
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
						
			ptr->next = (*left);
			(*left) = ptr;
			
			ptr = prev_ptr->next;

			//No need to update prev_ptr in this case.
		}
		//go to the next node
		else
		{
			prev_ptr = ptr;
			ptr = prev_ptr->next;
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
	if(ph == NULL)
	{
		return ph;
	}
	bool bInserted = false;
	node **head = &ph, **curr_i = head, **curr_j, *entry;
	for(curr_i = &(*head)->next; *curr_i != NULL; )
	{
		bInserted = false;
		entry = *curr_i;
		for(curr_j = head; *curr_j != entry; curr_j = &(*curr_j)->next)
		{
			if(entry->data < (*curr_j)->data)
			{
				//insert
				*curr_i = entry->next;
				entry->next = *curr_j;
				*curr_j = entry;

				bInserted = true;
				break;
			}
		}
		if(!bInserted)
		{
			curr_i = &entry->next;
		}
	}
	return *head;
}

node* bubble_once(node** head, node* last)
{
	node** curr = head;
	for(; (*curr)->next != last; )
	{
		if( (*curr)->data > (*curr)->next->data )
		{
			swap_next(curr);
		}
		else
		{
			curr = &(*curr)->next;
		}
	}
	return *curr;
}

node* bubble_sort(node* ph)
{
	node **head = &ph, *last = NULL, **curr;
	while( *head != last )
	{
		//last = bubble_once(head, last);
		for(curr = head; (*curr)->next != last;)
		{
			if( (*curr)->data > (*curr)->next->data )
			{
				//swap_next(curr);
				node* next = (*curr)->next;
				(*curr)->next = next->next;
				next->next = *curr;
				*curr = next;
			}
			else
			{
				curr = &(*curr)->next;
			}
		}
		last = *curr;
	}
	return *head;
}

node* select_sort(node* ph)
{
	node **head = &ph;
	for(node **first = head; *first != NULL; first = &(*first)->next)
	{
		node **min = first;
		for(node **curr = &(*first)->next; *curr; curr = &(*curr)->next)
		{
			if( (*curr)->data < (*min)->data )
			{
				min = curr;
			}
		}

		if(min != first)
		{
			swap(first, min);
		}
	}
	return *head;
}

/*
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
			//pj is the first node bigger than pi. so pi should be insert before pj
			if(pi->data < pj->data)
			{
				break;
			}
		}
		//This means the inner "for loop" goes to "break".
		if(pj != pi)
		{
			//insert pi before pj
			prev_pi->next = pi->next;
			pi->next = pj;
			prev_pj->next = pi;

			pi = prev_pi->next;
			//prev_pi remain no change
		}
		//never goes to "break", pi is bigger than all the ordered previous nodes. 
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
				//pi should be the smallest node. now it should be pj because pi and pj has been swaped.
				pi = pj;

				//the same to pj
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

	node *prev_pi, *pi, *prev_pj, *pj, *prev_min, *min, *tmp;
	for(prev_pi = &dummy, pi = dummy.next; pi != NULL; )
	{
		prev_min = prev_pi;
		min = pi;
		for(prev_pj = pi, pj = pi->next; pj != NULL; prev_pj = pj, pj = pj->next)
		{
			if(pj->data < min->data)
			{
				prev_min = prev_pj;
				min = pj;
			}
		}
		//Not the same node, need to swap
		if(min != pi)
		{
			if(pi->next == min)
			{
				pi->next = min->next;
				min->next = pi;
				prev_pi->next = min;

				prev_pi = min;
			}
			else
			{
				tmp = min->next;
				min->next = pi->next;
				pi->next = tmp;

				prev_pi->next = min;
				prev_min->next = pi;

				prev_pi = min;
				pi = min->next;
			}
		}
		else
		{
			prev_pi = pi;
			pi = pi->next;
		}
	}

	return dummy.next;
}
*/

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

void swap_next(node** ptr)
{
	node* next = (*ptr)->next;
	if(!next)
	{
		return;
	}
	(*ptr)->next = next->next;
	next->next = *ptr;
	*ptr = next;
}

void swap(node** p1, node** p2)
{
	if( (*p1)->next == *p2 )
	{
		swap_next(p1);
	}
	else if( (*p2)->next == *p1 )
	{
		swap_next(p2);
	}
	else
	{
		node* tmp = (*p1)->next;
		(*p1)->next = (*p2)->next;
		(*p2)->next = tmp;
		
		tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
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


/************************************************************************/
/* 10. Simulate the add operation of big number                         */
/************************************************************************/

/************************************************************************/
/* make sure len1 >= len2                                               */
/************************************************************************/
int add(node* ph1, int len1, node* ph2, int len2, node** p)
{
	if(len1 == 0 || len2 == 0)
	{
		return 0;
	}
	int ret, sum;
	*p = new node();

	if(len1 > len2)
	{
		ret = add(ph1->next, len1 - 1, ph2, len2, &(*p)->next);
		sum = ph1->data + ret;
		(*p)->data = sum % 10;
		return sum >= 10 ? 1 : 0;
	}
	else
	{
		ret = add(ph1->next, len1 - 1, ph2->next, len2 - 1, &(*p)->next);
		sum = ph1->data + ph2->data + ret;
		(*p)->data = sum % 10;
		return sum >= 10 ? 1 : 0;
	}
}

node* add(node* ph1, int len1, node* ph2, int len2)
{
	if(len1 == 0 || len2 == 0)
	{
		return NULL;
	}
	node* ph = new node();
	int carry;
	if(len1 >= len2)
	{
		carry = add(ph1, len1, ph2, len2, &ph);
	}
	else
	{
		carry = add(ph2, len2, ph1, len1, &ph);
	}
	
	if(carry > 0)
	{
		return new node(carry, ph);
	}
	return ph;
}

/************************************************************************/
/* 11. Remove the duplicated element in list                            */
/************************************************************************/
node* remove_duplicated(node* ph)
{
	if(ph == NULL || ph->next == NULL)
	{
		return ph;
	}
	map<int, int> nodeMap;
	nodeMap.insert(make_pair(ph->data, 1));

	node* curr = ph->next;
	node* prev = ph;
	node* next = NULL;
	while(curr != NULL)
	{
		next = curr->next;
		if(nodeMap.find(curr->data) != nodeMap.end())
		{
			nodeMap[curr->data]++;
			delete curr;
		}
		else
		{
			nodeMap.insert(make_pair(curr->data, 1));
			prev->next = curr;
			prev = curr;
		}
		curr = next;
	}
	prev->next = NULL;

	nodeMap.clear();

	return ph;
}

class Point
{
public:
	int x, y;
	virtual ~Point()
	{
		cout<<"Destructor: x = "<<x<<", y="<<y<<endl;
	}
};



void testLinkedListEx()
{
#if ((LINKED_LIST_EX_TEST) == 1)
	Point point;
	point.x = 0;
	point.y = 10;
	
	ListNode<Point>* head = new ListNode<Point>(point);
	
	ListNode<Point>* p = head;
	for(int i = 0; i < 3; i++)
	{
		p->next = new ListNode<Point>();
		p = p->next;
	}
	
	p = head;
	ListNode<Point>* temp;
	while(p != NULL)
	{
		temp = p;
		p = p->next;
		delete temp;
	}
	ListNode<Point>::free();
	cout<<"end"<<endl;
	printSep(__FILE__);
#endif
}

void testLinkedList()
{
#if ((LINKED_LIST_TEST) == 1)
	//(0). Set up linkedlist
	int data1[] = {9, 8, 9, 8, 9, 8, 9};
	int data2[] = {24,38,86,97,18,84,56,86,97,24};

	node* p1 = createList(data1, COUNTOF(data1));
	printList(p1);

	node* p2 = createList(data2, COUNTOF(data2));
	printList(p2);

	const int n = 10;
	int a[n];
	genrand(a, n, 100);
	
	node* p3 = createList(a, n);
 	printList(p3);

// 	p1 = remove_duplicated(p1);
// 	printList(p1);
// 
// 	p2 = remove_duplicated(p2);
// 	printList(p2);
// 
// 	p3 = remove_duplicated(p3);
// 	printList(p3);

	remove(&p1, 9);
	printList(p1);

	p2 = remove(p2, 24);
	printList(p2);

// 	node* pTemp = NULL;
// 	pTemp = mergeIter(p1, p2);
// 	printList(pTemp);
// 
// 	pTemp = NULL;
// 	pTemp = mergeIter1(p1, p2);
// 	printList(pTemp);

// 	node* pLastNode = get(p2, size(p2) - 1);
// 	node* pInterNode = get(p2, 2); 

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
	node* ph = createList(a, n);
	printf("[quick sort]\n");
	printList(ph);
	ph = qsort(ph);
	printList(ph);

	node* ph1 = createList(a, n);
	printf("[bubble sort]\n");
	printList(ph1);
	ph1 = bubble_sort(ph1);
	printList(ph1);

	assertEquals(ph, ph1);

	node* ph2 = createList(a, 10);
	printf("[insert sort]\n");
	printList(ph2);
	ph2 = insert_sort(ph2);
	printList(ph2);

	assertEquals(ph, ph2);
	
	node* ph3 = createList(a, n);
	printf("[select sort]\n");
	printList(ph3);
	ph3 = select_sort(ph3);
	printList(ph2);
	
	assertEquals(ph, ph3);

	node* ph3_1 = createList(a, n);
	printf("[merge sort]\n");
	printList(ph3_1);
	ph3_1 = merge_sort(ph3_1);
	printList(ph3_1);
	
	assertEquals(ph, ph3_1);

	node* ph4 = createList(a, n);
	printf("[swap adjacent]\n");
	printList(ph4);
	swapadjacent(&ph4);
	printList(ph4);

	node* ph5 = createList(a, n);
	printf("[swap adjacent]\n");
	printList(ph5);
	ph5 = swapadjacent(ph5);
	printList(ph5);

	printf("[merge list]\n");
	node* ph6 = createList(a, n);
	printList(ph6);
	node* ph7 = createList(data2, COUNTOF(data2));
	printList(ph7);
	node* pMergeHead = mergeIter1(ph6, ph7);
	printList(pMergeHead);

// 	node* result = add(p1, size(p1), p2, size(p2));
// 	printList(result);
// 	deleteList(result);
	
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
	printSep(__FILE__);
#endif
}

