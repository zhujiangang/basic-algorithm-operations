#include <iostream.h>
#include "Common.h"
#include "config.h"
#include "StackQueue.h"
#include "MyUtil.h"

void output(stack<int>& st)
{
	while(!st.empty())
	{
		cout<<st.top()<<" ";
		st.pop();
	}
	cout<<endl;
}
void output(queue<int>& que)
{
	while(!que.empty())
	{
		cout<<que.front()<<" ";
		que.pop();
	}
	cout<<endl;
}

void empty(stack<int>& st)
{
	while(!st.empty())
	{
		st.pop();
	}
}

/************************************************************************/
/* 1. Design a stack with operation min() with O(1) time                */
/************************************************************************/
void StackWithMin::push(int val)
{
	if(minStack.empty() || val <= minStack.top())
	{
		minStack.push(val);
	}
	else
	{
		minStack.push(minStack.top());
	}
	dataStack.push(val);
}

int StackWithMin::pop()
{
	if(dataStack.empty())
	{
		return -1;
	}
	minStack.pop();
	int ret = dataStack.top();
	dataStack.pop();
	return ret;
}

int StackWithMin::peek()
{
	if(dataStack.empty())
	{
		return -1;
	}
	return dataStack.top();
}

int StackWithMin::min()
{
	if(minStack.empty())
	{
		return 0x7FFFFFFF;
	}
	return minStack.top();
}

/************************************************************************/
/* 2. Design a queue with 2 stack                                       */
/************************************************************************/
void QueueWith2Stack::enqueue(int val)
{
	s1.push(val);
}
int QueueWith2Stack::dequeue()
{
	if(s2.empty() && s1.empty())
	{
		//no elements
		return -1;
	}
	if(s2.empty())
	{
		while(!s1.empty())
		{
			s2.push(s1.top());
			s1.pop();
		}
	}
	int ret = s2.top();
	s2.pop();
	return ret;
}
int QueueWith2Stack::peek()
{
	if(s2.empty() && s1.empty())
	{
		//no elements
		return -1;
	}
	if(s2.empty())
	{
		while(!s1.empty())
		{
			s2.push(s1.top());
			s1.pop();
		}
	}
	return s2.top();
}


/************************************************************************/
/* 3. Design a stack with 2 queue                                       */
/************************************************************************/

void StackWith2Queue::push(int val)
{
	if(q1.empty())
	{
		q2.push(val);
	}
	else
	{
		q1.push(val);
	}
}

int StackWith2Queue::pop()
{
	if(q1.empty() && q2.empty())
	{
		return -1;
	}
	int result;
	if(!q1.empty())
	{
		while(q1.size() > 1)
		{
			q2.push(q1.front());
			q1.pop();
		}
		result = q1.front();
		q1.pop();
		return result;
	}
	else
	{
		while(q2.size() > 1)
		{
			q1.push(q2.front());
			q2.pop();
		}
		result = q2.front();
		q2.pop();
		return result;		
	}
}

int StackWith2Queue::peek()
{
	if(q1.empty() && q2.empty())
	{
		return -1;
	}
	int result;
	if(!q1.empty())
	{
		while(q1.size() > 1)
		{
			q2.push(q1.front());
			q1.pop();
		}
		result = q1.front();
		q2.push(q1.front());
		q1.pop();
		return result;
	}
	else
	{
		while(q2.size() > 1)
		{
			q1.push(q2.front());
			q2.pop();
		}
		result = q2.front();
		q1.push(q2.front());
		q2.pop();
		return result;		
	}
}


/************************************************************************/
/* 4. Check if a sequence is a pop order of push sequence               */
/* push the elements of "pushSeq" into stack in order, check if the stack 
top equals to the current element of "popSeq". If yes, pop up the stack top,
else, do nothing. When finished pushing all the elements, if the stack is empty,
it's a valid pop up sequence, else not valid.
/************************************************************************/
bool isPopSequence(const int pushSeq[], const int popSeq[], int len)
{
	stack<int> st;
	int i, j;
	for(i = 0, j = 0; i < len; i++)
	{
		st.push(pushSeq[i]);
		while(!st.empty() && st.top() == popSeq[j])
		{
			st.pop();
			j++;
		}
	}
	return st.empty();
}


/************************************************************************/
/* 5. Reverse a stack with recursive method                             */
/************************************************************************/
void reverse(stack<int>& st)
{
	if(st.size() <= 1)
	{
		return;
	}
	int top1 = st.top();
	st.pop();

	reverse(st);

	int top2 = st.top();
	st.pop();

	reverse(st);

	st.push(top1);
	reverse(st);
	st.push(top2);
}

/************************************************************************/
/* 6. Sort a stack with recursive method                             */
/************************************************************************/
void sort(stack<int>& st)
{
	if(st.size() <= 1)
	{
		return;
	}
	int top1 = st.top();
	st.pop();

	sort(st);

	int top2 = st.top();

	if(top1 > top2)
	{
		st.pop();
		st.push(top1);
		sort(st);
		st.push(top2);
	}
	else
	{
		st.push(top1);
	}
}

/************************************************************************/
/* 7. Design 3 stack with an array
/************************************************************************/
StackWith1Array::StackWith1Array(int cap):capacity(cap), top1(-1), top2(cap), top3l(cap/2 + 1), top3r(cap/2), lor(false)
{
	data = new int[cap];
}
StackWith1Array::~StackWith1Array()
{
	delete []data;
}

void StackWith1Array::push(int index, int val)
{
	if(index == 1)
	{		
		if(top1 + 1 >= top3l)
		{
			//throw exception("stack full");
			return;
		}
		top1++;
		data[top1] = val;
	}
	else if(index == 2)
	{		
		if(top2 - 1 <= top3r)
		{
			//throw exception("stack full"); 
			return;
		}
		top2--;
		data[top2] = val;
	}
	else
	{
		//left
		if(lor)
		{
			if(top3r + 1 >= top2)
			{
				//throw exception("stack full"); 
				return;
			}
			top3r++;
			data[top3r] = val;
			lor = !lor;
		}
		else
		{			
			if(top3l - 1 <= top1)
			{	
				//throw exception("stack full"); 
				return;
			}
			top3l--;
			data[top3l] = val;
			lor = !lor;
		}
	}
}

int StackWith1Array::pop(int index)
{
	int ret = -1;
	if(index == 1)
	{
		if(top1 < 0)
		{
			//throw exception("stack empty");
			return -1;
		}
		ret = data[top1--];
		return ret;
	}
	else if(index == 2)
	{
		if(top2 >= capacity)
		{
			//throw exception("stack empty");
			return -1;
		}
		ret = data[top2++];
		return ret;
	}
	else
	{
		if(top3l > top3r)
		{
			//throw exception("stack empty");
			return -1;
		}
		if(lor)
		{
			ret = data[top3l++];
		}
		else
		{
			ret = data[top3r--];
		}
		lor = !lor;
		return ret;
	}
}


int StackWith1Array::top(int index)
{
	int ret = -1;
	if(index == 1)
	{
		if(top1 < 0)
		{
			//throw exception("stack empty");
			return -1;
		}
		ret = data[top1];
		return ret;
	}
	else if(index == 2)
	{
		if(top2 >= capacity)
		{
			//throw exception("stack empty");
			return -1;
		}
		ret = data[top2];
		return ret;
	}
	else
	{
		if(top3l > top3r)
		{
			//throw exception("stack empty");
			return -1;
		}
		if(lor)
		{
			ret = data[top3l];
		}
		else
		{
			ret = data[top3r];
		}
		return ret;
	}
}

void testStackQueue()
{
#if ((STACK_QUEUE_TEST) == 1)
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
	printSep(__FILE__);
#endif
}