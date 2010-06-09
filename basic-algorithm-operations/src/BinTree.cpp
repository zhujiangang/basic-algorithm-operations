#include <iostream.h>
#include <stack>
#include <queue>
#include "BinTree.h"

using std::stack;
using std::queue;

OutputVisitor outputVisitor;
const int PREORDER = 1;
const int INORDER = 2;
const int POSTORDER = 3;
const int LEVELORDER = 4;

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
void BinTree::preVisit(Visitor* visitor, void* param)
{
	if(param != NULL)
	{
		int* pt = (int*)param;
		
		switch(*pt)
		{
		case PREORDER:
			cout<<"Preorder:\t";
			break;
		case INORDER:
			cout<<"Inorder:\t";
			break;
		case POSTORDER:
			cout<<"Postorder:\t";
			break;
		case LEVELORDER:
			cout<<"Levelorder:\t";
			break;
		default:
			cout<<"Unknow type:\t";
		}
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
	preVisit(visitor, (int*)&PREORDER);
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
	preVisit(visitor, (int*)&INORDER);
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
	preVisit(visitor, (int*)&POSTORDER);
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

void BinTree::levelOrder(Visitor* visitor)
{
	preVisit(visitor, (int*)&LEVELORDER);
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
