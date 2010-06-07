#include <iostream.h>
#include "Common.h"
#include "StackQueue.h"

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