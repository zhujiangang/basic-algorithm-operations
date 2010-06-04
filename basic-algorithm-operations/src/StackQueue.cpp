#include "Common.h"
#include "StackQueue.h"


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
