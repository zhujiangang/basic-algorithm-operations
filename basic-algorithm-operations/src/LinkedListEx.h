#ifndef _LINKED_LIST_EX_H_
#define _LINKED_LIST_EX_H_

template <class T>
class ListNode
{
public:
	T element;
	ListNode* next;
public:
	ListNode(const T& element_var, ListNode* next_var = NULL) : element(element_var), next(next_var)
	{
	}
	ListNode() : next(NULL)
	{
	}
	virtual ~ListNode()
	{
	}
	static ListNode<T>* freelist;
	static void free()
	{
		ListNode *p = freelist, *temp = NULL;
		while(p != NULL)
		{
			temp = p;
			p = p->next;
			::delete temp;
		}
		freelist = NULL;
	}
	void* operator new(size_t)
	{
		if(freelist == NULL)
		{
			return ::new ListNode;
		}
		ListNode* temp = freelist;
		freelist = freelist->next;
		return temp;	
	}
	void* operator new(size_t, const T& element_var)
	{
		if(freelist == NULL)
		{
			return ::new ListNode(element_var);
		}
		ListNode* temp = freelist;
		freelist = freelist->next;

		temp->element = element_var;
		return temp;	
	}
	void operator delete(void* ptr)
	{
		ListNode* p = (ListNode*)ptr;
		p->next = freelist;
		freelist = p;
	}
};

template <class T>
ListNode<T>* ListNode<T>::freelist = NULL;


void testLinkedListEx();
#endif