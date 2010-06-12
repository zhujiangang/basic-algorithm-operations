#include <iostream.h>
#include <stack>
#include <queue>
#include "Common.h"
#include "MyUtil.h"
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
	case PRE_ORDER_STACK_B:
		cout<<"PreorderB(S):\t";
		break;
	case IN_ORDER_STACK_B:
		cout<<"InorderB(S):\t";
		break;
	case POST_ORDER_STACK_B:
		cout<<"PostorderB(S):\t";
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

/*
The below 3 functions with a "B" as suffix are from:
http://www.cnblogs.com/Jax/archive/2009/12/28/1633691.html
However, there are several bugs in the implementions.
Please look into the codes to find the "[BUG]" for the details.

void BinTree::preOrderStackB(Visitor* visitor)
{
	preVisit(PRE_ORDER_STACK_B, visitor);

	stack<BinNode*> st;
	BinNode* temp = root;
	while(temp != NULL)
	{
		visitor->visit(temp);
		if(temp->rc != NULL)
		{
			st.push(temp->rc);
		}
		temp = temp->lc;
	}

	while(!st.empty())
	{
		temp = st.top();
		st.pop();

        visitor->visit(temp);
		while(temp != NULL)
		{
		    [BUG]:
		    This will skip some nodes. eg. the node 6 will be skipped
			         5
					  \
                       8
                      /
                     6
					  \
					   7
			if(temp->rc != NULL)
			{
				st.push(temp->rc);
			}
			temp = temp->lc;
		}
	}

	postVisit(visitor);
}
void BinTree::inOrderStackB(Visitor* visitor)
{
	preVisit(IN_ORDER_STACK_B, visitor);

	stack<BinNode*> st;
	BinNode* temp = root;

	while(temp != NULL)
	{
		if(temp != NULL)
		{
			st.push(temp);
		}
		temp = temp->lc;
	}
	while(!st.empty())
	{
		temp = st.top();
		st.pop();

		visitor->visit(temp);
		if(temp->rc != NULL)
		{
			temp = temp->rc;
			st.push(temp);
			while(temp != NULL)
			{
				if(temp->lc != NULL)
				{
					st.push(temp->lc);
				}
				temp = temp->lc;
			}
		}
	}
	postVisit(visitor);
}
void BinTree::postOrderStackB(Visitor* visitor)
{
	preVisit(POST_ORDER_STACK_B, visitor);
	stack<BinNode*> st;
	BinNode* temp = root;

	while(temp != NULL)
	{
		if(temp != NULL)
		{
			st.push(temp);
		}
		temp = temp->lc;
	}

	while(!st.empty())
	{
		BinNode* lastvisit = temp;
		temp = st.top();
		st.pop();
		if(temp->rc == NULL || temp->rc == lastvisit)
		{
			visitor->visit(temp);
		}
		else if(temp->lc == lastvisit)
		{
		    //[BUG]?
			//Until now it seems not a bug, but it push and pop some nodes than once,
			//not a good performance implemention.
			st.push(temp);
			temp = temp->rc;
			st.push(temp);
			while(temp != NULL)
			{
				if(temp->lc != NULL)
				{
					st.push(temp->lc);
				}
				temp = temp->lc;
			}
		}
	}
	postVisit(visitor);
}
*/


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

BinNode* BinTree::getNodeByID(BinNode* t, int id)
{
	if(t == NULL)
	{
		return NULL;
	}
	if(t->value == id)
	{
		return t;
	}
	BinNode* ret = NULL;
	ret = getNodeByID(t->lc, id);
	if(ret != NULL)
	{
		return ret;
	}
	else
	{
		return getNodeByID(t->rc, id);
	}
}

bool BinTree::getPosOfNode(BinNode* t, BinNode* binNode, int* pos)
{
	if(t == NULL || binNode == NULL)
	{
		*pos = 0;
		return false;
	}
	if(t == binNode)
	{
		return true;
	}
	int tmp = *pos;
	*pos = 2 * tmp;
	if(getPosOfNode(t->lc, binNode, pos))
	{
		return true;
	}
	*pos = 2 * tmp + 1;
	if(getPosOfNode(t->rc, binNode, pos))
	{
		return true;
	}
	*pos = 0;
	return false;
}

int BinTree::findParentPos(int pos1, int pos2)
{
	if(pos1 == pos2)
	{
		return pos1;
	}

	if(pos2 > pos1)
	{
		int tmp = pos1;
		pos1 = pos2;
		pos2 = tmp;
	}

	int diff = bitsNum(pos1) - bitsNum(pos2);
	while(diff != 0)
	{
		pos1 = pos1 >> 1;
		diff--;
	}

	while(pos1 != pos2)
	{
		pos1 = pos1 >> 1;
		pos2 = pos2 >> 1;
	}
	return pos1;
}

BinNode* BinTree::getNodeByPos(int pos)
{
	if(pos == 0)
	{
		return NULL;
	}
	BinNode* t = root;
	unsigned int power = flp2(pos);
	power = power >> 1;
	while(power != 0)
	{
		if( (pos & power) == 0)
		{
			t = t->lc;
		}
		else
		{
			t = t->rc;
		}
		power = power >> 1;
	}
	return t;
}

BinNode* BinTree::findLatestParent(BinNode* binNode1, BinNode* binNode2)
{
	int pos1 = 1, pos2 = 1;
	if(!getPosOfNode(root, binNode1, &pos1) || !getPosOfNode(root, binNode2, &pos2))
	{
		return NULL;
	}

	int parentPos = findParentPos(pos1, pos2);
	return getNodeByPos(parentPos);
}