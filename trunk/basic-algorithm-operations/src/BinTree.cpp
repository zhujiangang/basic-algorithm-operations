#include <iostream.h>
#include <stack>
#include <queue>
#include "BinTree.h"

using std::stack;
using std::queue;

OutputVisitor outputVisitor;

void OutputVisitor::visit(BinNode* root)
{
	if(root != NULL)
	{
		cout<<root->value<<" ";
	}	
}
BinTree::BinTree(int data[], int len)
{
	if(data == NULL || len <= 0)
	{
		root = NULL;
		return;
	}
	root = buildInOrder(data, 0, len - 1);
}

BinNode* BinTree::buildInOrder(int data[], int start, int end)
{
	if(start > end)
	{
		return NULL;
	}
	int mid = (start + end) >> 1;

	BinNode* left = buildInOrder(data, start, mid - 1);
	BinNode* right = buildInOrder(data, mid + 1, end);

	BinNode* parent = new BinNode(data[mid], left, right);
	if(left != NULL)
	{
		left->p = parent;
	}
	if(right != NULL)
	{
		right->p = parent;
	}
	return parent;
}

BinTree::~BinTree()
{
	deleteTree(root);
	root = NULL;
}

void BinTree::deleteTree(BinNode* t)
{
	if(t == NULL)
	{
		return;
	}
	deleteTree(t->lc);
	deleteTree(t->rc);
	delete t;
	t = NULL;
}
void BinTree::preVisit(VisitType vt, Visitor* visitor)
{
	switch(vt)
	{
	case PRE_ORDER:
		cout<<"Preorder:\t";
		break;
	case IN_ORDER:
		cout<<"Inorder:\t";
		break;
	case POST_ORDER:
		cout<<"Postorder:\t";
		break;
	case LEVEL_ORDER:
		cout<<"Levelorder:\t";
		break;
	case PRE_ORDER_STACK:
		cout<<"Preorder(S):\t";
		break;
	case IN_ORDER_STACK:
		cout<<"Inorder(S):\t";
		break;
	case POST_ORDER_STACK:
		cout<<"Postorder(S):\t";
		break;
	default:
		cout<<"Unknow type:\t";
		break;
	}
}

void BinTree::postVisit(Visitor* visitor, void* param)
{
	if(visitor == &outputVisitor)
	{
		cout<<endl;
	}
}
void BinTree::preOrder(Visitor* visitor)
{
	preVisit(PRE_ORDER, visitor);
	preOrderHelper(root, visitor);
	postVisit(visitor);
}	
void BinTree::preOrderHelper(BinNode* t, Visitor* visitor)
{
	if(t != NULL)
	{
		visitor->visit(t);
		preOrderHelper(t->lc, visitor);
		preOrderHelper(t->rc, visitor);
	}	
}

void BinTree::inOrder(Visitor* visitor)
{
	preVisit(IN_ORDER, visitor);
	inOrderHelper(root, visitor);
	postVisit(visitor);
}
void BinTree::inOrderHelper(BinNode* t, Visitor* visitor)
{
	if(t != NULL)
	{
		inOrderHelper(t->lc, visitor);
		visitor->visit(t);
		inOrderHelper(t->rc, visitor);
	}
}


void BinTree::postOrder(Visitor* visitor)
{
	preVisit(POST_ORDER, visitor);
	postOrderHelper(root, visitor);
	postVisit(visitor);
}
void BinTree::postOrderHelper(BinNode* t, Visitor* visitor)
{
	if(t != NULL)
	{
		postOrderHelper(t->lc, visitor);
		postOrderHelper(t->rc, visitor);
		visitor->visit(t);
	}
}

void BinTree::preOrderStack(Visitor* visitor)
{
	preVisit(PRE_ORDER_STACK, visitor);
	stack<BinNode*> st;
	BinNode* t = root;
	while(t != NULL || !st.empty())
	{
		while(t != NULL)
		{
			visitor->visit(t);
			st.push(t);
			t = t->lc;
		}
		if(!st.empty())
		{
			t = st.top();
			st.pop();
			t = t->rc;
		}
	}
	postVisit(visitor);
}
void BinTree::inOrderStack(Visitor* visitor)
{
	preVisit(IN_ORDER_STACK, visitor);
	stack<BinNode*> st;
	BinNode* t = root;
	while(t != NULL || !st.empty())
	{
		while(t != NULL)
		{
			st.push(t);
			t = t->lc;
		}
		if(!st.empty())
		{
			t = st.top();
			visitor->visit(t);

			st.pop();
			t = t->rc;
		}
	}
	postVisit(visitor);
}
void BinTree::postOrderStack(Visitor* visitor)
{
	preVisit(POST_ORDER_STACK, visitor);

	stack<BinNode*> st;
	BinNode* t = root;
	BinNode* p = NULL;

	while(t != NULL || !st.empty())
	{
		while(t != NULL)
		{
			st.push(t);
			t = t->lc;
		}

 		p = NULL;
		
		while(!st.empty())
		{
			t = st.top();
			if(t->rc == p)
			{
				visitor->visit(t);

				st.pop();				
				p = t;
				t = NULL;
			}
			else
			{
				t = t->rc;
				break;
			}
		}

//		t = st.top();
// 		while(p == t->rc)  
// 		{  
// 			t = st.top();
// 			visitor->visit(t);
// 			
// 			st.pop();
// 			
// 			p = t;                  
// 			if(st.empty())  
// 			{  
// 				return;  
// 			}  
// 			t = st.top();
// 		}  
//      t = t->rc; 
	}

	postVisit(visitor);
}

void BinTree::levelOrder(Visitor* visitor)
{
	preVisit(LEVEL_ORDER, visitor);
	if(root == NULL)
	{
		return;
	}
	queue<BinNode*> que;
	que.push(root);

	BinNode* binNode = NULL;
	while(!que.empty())
	{
		binNode = que.front();
		que.pop();

		visitor->visit(binNode);

		if(binNode->lc != NULL)
		{
			que.push(binNode->lc);
		}
		if(binNode->rc != NULL)
		{
			que.push(binNode->rc);
		}
	}

	postVisit(visitor);
}
