#ifndef _STACK_QUEUE_H_
#define _STACK_QUEUE_H_

#include <stack>
#include <queue>

using std::stack;
using std::queue;

/************************************************************************/
/* 1. Design a stack with operation min() with O(1) time                */
/************************************************************************/
class StackWithMin
{
public:
	void push(int val);
	int pop();
	int peek();	
	int min();
private:
	stack<int> dataStack;
	stack<int> minStack;
};

/************************************************************************/
/* 2. Design a queue with 2 stack                                       */
/************************************************************************/
class QueueWith2Stack
{
public:
	void enqueue(int val);
	int dequeue();
	int peek();
private:
	stack<int> s1;
	stack<int> s2;
};

/************************************************************************/
/* 3. Design a stack with 2 queue                                       */
/************************************************************************/

class StackWith2Queue
{
public:
	void push(int val);
	int pop();
	int peek();
private:
	queue<int> q1;
	queue<int> q2;
};

#endif