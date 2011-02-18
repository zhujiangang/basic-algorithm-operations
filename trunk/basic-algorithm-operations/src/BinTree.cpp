#include <iostream.h>
#include <stack>
#include <queue>
#include <list>
#include "Common.h"
#include "MyUtil.h"
#include "BinTree.h"
#include "config.h"
#include "ArrayOper.h"

using std::stack;
using std::queue;
using std::list;

OutputVisitor outputVisitor;

bool compare(BinNode* t1, BinNode* t2)
{
	//Both are NULL
	if(t1 == NULL && t2 == NULL)
	{
		return true;
	}
	//One of them is NULL
	if(t1 == NULL || t2 == NULL)
	{
		return false;
	}
	if(t1->value != t2->value)
	{
		return false;
	}
	if(!compare(t1->lc, t2->lc))
	{
		return false;
	}
	return compare(t1->rc, t2->rc);
}

bool compare(BinTree& t1, BinTree& t2)
{
	return compare(t1.getRoot(), t2.getRoot());
}
BinNode* createTreeByInLevel(const int in[], const int level[], int n)
{
	if(n < 1)
	{
		return NULL;
	}
	LevelOrderVisitNode s;
	queue<LevelOrderVisitNode> que;
	int r = 0;

	BinNode* root = NULL;
	BinNode* p = NULL;
	root = p = new BinNode(level[0]);

	int i;
	bool found = false;
	for(i = 0; i < n; i++)
	{
		if(in[i] == level[0])
		{
			found = true;
			break;
		}
	}
	//The in-order and level-order is not the one tree's valid visit order
	if(!found)
	{
		return NULL;
	}
	if(i == 0) //no left child
	{
		s.lvl = ++r;
		s.l = i + 1;
		s.h = n - 1;
		s.p = p;
		s.lr = RIGHT_CHILD;
		que.push(s);
	}
	else if(i == n - 1) //no right child
	{
		s.lvl = ++r;
		s.l = 0;
		s.h = i - 1;
		s.p = p;
		s.lr = LEFT_CHILD;
		que.push(s);
	}
	else //has both left and right child
	{
		s.lvl = ++r;
		s.l = 0;
		s.h = i - 1;
		s.p = p;
		s.lr = LEFT_CHILD;
		que.push(s);

		s.lvl = ++r;
		s.l = i + 1;
		s.h = n - 1;
		s.p = p;
		s.lr = RIGHT_CHILD;
		que.push(s);
	}

	
	BinNode* father;
	int low, high;
	while(!que.empty())
	{
		s = que.front();
		que.pop();

		father = s.p;
		found = false;
		for(i = s.l; i <= s.h; i++)
		{
			if(in[i] == level[s.lvl])
			{
				found = true;
				break;
			}
		}
		//The in-order and level-order is not the one tree's valid visit order
		if(!found)
		{
			return NULL;
		}
		p = new BinNode(level[s.lvl]);
		if(s.lr == LEFT_CHILD)
		{
			father->lc = p;
		}
		else
		{
			father->rc = p;
		}

		if(s.l == s.h)
		{
			continue;
		}

		if(i == s.l)
		{
			s.lvl = ++r;
			s.l = i + 1;
			s.p = p;
			s.lr = RIGHT_CHILD;
			que.push(s);
		}
		else if(i == s.h)
		{
			s.lvl = ++r;
			s.h = i - 1;
			s.p = p;
			s.lr = LEFT_CHILD;
			que.push(s);
		}
		else
		{
			low = s.l;
			high = s.h;


			s.lvl = ++r;
			s.l = low;
			s.h = i - 1;
			s.p = p;
			s.lr = LEFT_CHILD; 
			que.push(s);

			s.lvl = ++r;
			s.l = i + 1;
			s.h = high;
			s.p = p;
			s.lr = RIGHT_CHILD;
			que.push(s);
		}
	}

	return root;
}

BinNode* createTreeByInLevel2(const int in[], const int level[], int n)
{
	if(n < 1)
	{
		return NULL;
	}
	LevelOrderVisitNode s;
	queue<LevelOrderVisitNode> que;
	int r = 0;
	
	BinNode* root = NULL;
	BinNode* p = NULL;
	root = p = new BinNode(level[0]);

	s.l = 0;
	s.h = n - 1;
	s.lvl = r;
	s.p = p;
	que.push(s);
	
	int i;
	bool found = false;

	int l, h, lvl;

	while(!que.empty())
	{
		s = que.front();
		que.pop();

		for(i = s.l; i <= s.h; i++)
		{
			if(in[i] == level[s.lvl])
			{
				found = true;
				break;
			}
		}
		if(!found)
		{
			return NULL;
		}

		lvl = s.lvl;
		l = s.l;
		h = s.h;
		p = s.p;
		//No left child
		if(i == l)
		{
			p->lc = NULL;
		}
		else
		{
			++r;
			p->lc = new BinNode(level[r]);
			
			s.l = l;
			s.h = i - 1;
			s.lvl = r;
			s.p = p->lc;

			que.push(s);
		}

		//No right child
		if(i == h)
		{
			p->rc = NULL;
		}
		else
		{
			++r;
			p->rc = new BinNode(level[r]);

			s.l = i + 1;
			s.h = h;
			s.lvl = r;
			s.p = p->rc;

			que.push(s);
		}
	}
	return root;
}

BinNode* createTreeByInPre(const int in[], int inBegin, int inEnd, const int pre[], int preBegin, int preEnd)
{
	int i;
	bool found = false;
	for(i = inBegin; i <= inEnd; i++)
	{
		if(in[i] == pre[preBegin])
		{
			found = true;
			break;
		}
	}
	//Invalid in-order and pre-order sequence
	if(!found)
	{
		return NULL;
	}

	BinNode* root = new BinNode(in[i]);

	if(inBegin == inEnd)
	{
		return root;
	}

	//no left child
	if(i == inBegin)
	{
		root->lc = NULL;
		root->rc = createTreeByInPre(in, i + 1, inEnd, pre, preBegin + 1, preEnd);
		root->rc->p = root;
	}
	//no right child
	else if(i == inEnd)
	{
		root->lc = createTreeByInPre(in, inBegin, inEnd - 1, pre, preBegin + 1, preEnd);
		root->rc = NULL;
		root->lc->p = root;		
	}
	//have both left and right child
	else
	{
		root->lc = createTreeByInPre(in, inBegin, i - 1, pre, preBegin + 1, preBegin + i - inBegin);
		root->rc = createTreeByInPre(in, i + 1, inEnd, pre, preBegin + i - inBegin + 1, preEnd);

		root->lc->p = root;
		root->rc->p = root;
	}
	return root;
}

BinNode* createTreeByInPost(const int in[], int inBegin, int inEnd, const int post[], int postBegin, int postEnd)
{
	int i;
	bool found = false;
	for(i = inBegin; i <= inEnd; i++)
	{
		if(in[i] == post[postEnd])
		{
			found = true;
			break;
		}
	}
	//Invalid in-order and pre-order sequence
	if(!found)
	{
		return NULL;
	}
	
	BinNode* root = new BinNode(in[i]);
	
	if(inBegin == inEnd)
	{
		return root;
	}
	
	//no left child
	if(i == inBegin)
	{
		root->lc = NULL;
		root->rc = createTreeByInPost(in, i + 1, inEnd, post, postBegin, postEnd - 1);
		root->rc->p = root;
	}
	//no right child
	else if(i == inEnd)
	{
		root->lc = createTreeByInPost(in, inBegin, inEnd - 1, post, postBegin, postEnd - 1);
		root->rc = NULL;
		root->lc->p = root;		
	}
	//have both left and right child
	else
	{
		root->lc = createTreeByInPost(in, inBegin, i - 1, post, postBegin, postBegin + i - inBegin - 1);
		root->rc = createTreeByInPost(in, i + 1, inEnd, post, postBegin + i - inBegin, postEnd - 1);
		
		root->lc->p = root;
		root->rc->p = root;
	}
	return root;
}

BinNode* createTreeByInPreWithStack(const int in[], int inBegin, int inEnd, const int pre[], int preBegin, int preEnd)
{
	int i;
	bool found = false;

	BinNode* root = new BinNode(pre[preBegin]);

	//Only one element
	if(inBegin == inEnd)
	{
		return root;
	}

	queue<PreOrPostOrderVisitNode> que;

	PreOrPostOrderVisitNode node;
	node.p = root;
	node.li = inBegin;
	node.hi = inEnd;
	node.lp = preBegin;
	node.hp = preEnd;

	que.push(node);

	int li, hi;
	int lp, hp;
	BinNode* p;

	while(!que.empty())
	{
		node = que.front();
		que.pop();

		found = false;
		for(i = node.li; i <= node.hi; i++)
		{
			if(in[i] == pre[node.lp])
			{
				found = true;
				break;
			}
		}
		//Invalid in-order and pre-order sequence
		if(!found)
		{
			return NULL;
		}

		//no children
		if(node.li == node.hi)
		{
			continue;
		}

		li = node.li;
		hi = node.hi;
		lp = node.lp;
		hp = node.hp;
		p = node.p;

		//no left child
		if(i == li)
		{
			p->lc = NULL;
		}
		else
		{
			p->lc = new BinNode(pre[lp + 1]);
			node.p = p->lc;
			node.li = li;
			node.hi = i - 1;
			node.lp = lp + 1;
			node.hp = lp + i - li;

			que.push(node);
		}

		if(i == hi)
		{
			p->rc = NULL;
		}
		else
		{
			p->rc = new BinNode(pre[lp + i - li + 1]);
			node.p = p->rc;
			node.li = i + 1;
			node.hi = hi;
			node.lp = lp + i - li + 1;
			node.hp = hp;
			
			que.push(node);
		}
	}

	return root;
}

BinNode* createTreeByInPostWithStack(const int in[], int inBegin, int inEnd, const int post[], int postBegin, int postEnd)
{
	int i;
	bool found = false;
	
	BinNode* root = new BinNode(post[postEnd]);
	
	//Only one element
	if(inBegin == inEnd)
	{
		return root;
	}
	
	queue<PreOrPostOrderVisitNode> que;
	
	PreOrPostOrderVisitNode node;
	node.p = root;
	node.li = inBegin;
	node.hi = inEnd;
	node.lp = postBegin;
	node.hp = postEnd;
	
	que.push(node);
	
	int li, hi;
	int lp, hp;
	BinNode* p;
	
	while(!que.empty())
	{
		node = que.front();
		que.pop();
		
		found = false;
		for(i = node.li; i <= node.hi; i++)
		{
			if(in[i] == post[node.hp])
			{
				found = true;
				break;
			}
		}
		//Invalid in-order and pre-order sequence
		if(!found)
		{
			return NULL;
		}
		
// 		//no children
// 		if(node.li == node.hi)
// 		{
// 			continue;
// 		}
		
		li = node.li;
		hi = node.hi;
		lp = node.lp;
		hp = node.hp;
		p = node.p;
		
		//no left child
		if(i == li)
		{
			p->lc = NULL;
		}
		else
		{
			p->lc = new BinNode(post[lp + i - li - 1]);
			node.p = p->lc;
			node.li = li;
			node.hi = i - 1;
			node.lp = lp;
			node.hp = lp + i - li - 1;
			
			que.push(node);
		}
		
		if(i == hi)
		{
			p->rc = NULL;
		}
		else
		{
			p->rc = new BinNode(post[hp - 1]);
			node.p = p->rc;
			node.li = i + 1;
			node.hi = hi;
			node.lp = lp + i - li;
			node.hp = hp - 1;
			
			que.push(node);
		}
	}
	
	return root;
}

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


BinNode* BinTree::findLatestParentStack(BinNode* binNode1, BinNode* binNode2)
{
	stack<BinNode*> st1;
	if(!getPosOfNode(root, binNode1, st1))
	{
		return NULL;
	}

	stack<BinNode*> st2;
	if(!getPosOfNode(root, binNode2, st2))
	{
		return NULL;
	}

	BinNode* ret = NULL;

	while(!st1.empty() && !st2.empty() && st1.top() == st2.top())
	{
		ret = st1.top();
		st1.pop();
		st2.pop();
	}
	return ret;
}
bool BinTree::getPosOfNode(BinNode* t, BinNode* binNode, stack<BinNode*>& st)
{
	if(t == NULL)
	{
		return false;
	}
	if(t == binNode)
	{
		st.push(t);
		return true;
	}
	if(getPosOfNode(t->lc, binNode, st) || getPosOfNode(t->rc, binNode, st))
	{
		st.push(t);
		return true;
	}
	return false;
}

BinNode* BinTree::findLatestParentPtr(BinNode* binNode1, BinNode* binNode2)
{
	stack<BinNode*> st1;
	while(binNode1 != NULL)
	{
		st1.push(binNode1);
		binNode1 = binNode1->p;
	}

	stack<BinNode*> st2;
	while(binNode2 != NULL)
	{
		st2.push(binNode2);
		binNode2 = binNode2->p;
	}
	
	BinNode* ret = NULL;
	
	while(!st1.empty() && !st2.empty() && st1.top() == st2.top())
	{
		ret = st1.top();
		st1.pop();
		st2.pop();
	}
	return ret;
}

void BinTree::findPath(BinNode* t, int sum, stack<BinNode*>& st, bool isParentInPath)
{
	if(t == NULL)
	{
		return;
	}

	//Select parent, at least one ancestor in path
	st.push(t);
	if(t->value == sum)
	{
		stack<BinNode*> tempSt;
		while(!st.empty())
		{
			tempSt.push(st.top());
			st.pop();
		}
		while(!tempSt.empty())
		{
			cout<<tempSt.top()->value<<" ";
			st.push(tempSt.top());
			tempSt.pop();
		}
		cout<<endl;
	}
	findPath(t->lc, sum - t->value, st, true);
	//st doesn't include the result of the call of "t->lc", because the "st.pop()" called after every "push".
	findPath(t->rc, sum - t->value, st, true);
	st.pop();

	//No ancestor in path
	if(!isParentInPath)
	{
		findPath(t->lc, sum, st, false);
		findPath(t->rc, sum, st, false);
	}
}

BinNode* BinTree::findClosestToMid(int x)
{
	if(root == NULL)
	{
		return NULL;
	}
	BinNode* min = root;
	while(min->lc != NULL)
	{
		min = min->lc;
	}

	BinNode* max = root;
	while(max->rc != NULL)
	{
		max = max->rc;
	}

	int f = (min->value + max->value) >> 1;
	if(x != -1)
	{
		f = x;
	}

	BinNode* t = root;

	BinNode* prev = NULL;
	while(t != NULL)
	{
		if(t->value <= f)
		{
			t = t->rc;
		}
		else
		{
			prev = t;
			t = t->lc;
		}
	}
	return prev;
}


void BinTree::transformToDoubleLink(BinNode* t, DoubleLinkNode*& tail)
{
	if(t == NULL)
	{
		return;
	}

	if(t->lc != NULL)
	{
		transformToDoubleLink(t->lc, tail);
	}

	tail->next = new DoubleLinkNode(t->value, tail, NULL);
	tail = tail->next;

	if(t->rc != NULL)
	{
		transformToDoubleLink(t->rc, tail);
	}
}

void BinTree::transformToDoubleLink2(BinNode* t, DoubleLinkNode*& head)
{
	if(t == NULL)
	{
		return;
	}
	
	if(t->rc != NULL)
	{
		transformToDoubleLink2(t->rc, head);
	}
	

	if(head == NULL)
	{
		head = new DoubleLinkNode(t->value);
	}
	else
	{
		head->prev = new DoubleLinkNode(t->value, NULL, head);
	 	head = head->prev;
	}
	
	if(t->lc != NULL)
	{
		transformToDoubleLink2(t->lc, head);
	}
}

void BinTree::transformToDoubleLink3(BinNode* t, BinNode*& head)
{
	if(t == NULL)
	{
		return;
	}
	
	if(t->rc != NULL)
	{
		transformToDoubleLink3(t->rc, head);
	}
	
	
	if(head == NULL)
	{
		head = t;
	}
	else
	{
		head->lc = t;
		t->rc = head;
		head = t;
	}
	
	if(t->lc != NULL)
	{
		transformToDoubleLink3(t->lc, head);
	}
}

BinNode* BinTree::transformToDoubleLink4(BinNode* t, BinNode* head)
{
	if(t == NULL)
	{
		return NULL;
	}
	
	if(t->rc != NULL)
	{
		head = transformToDoubleLink4(t->rc, head);
	}
		
	if(head == NULL)
	{
		head = t;
	}
	else
	{
		head->lc = t;
		t->rc = head;
		head = t;
	}
	
	if(t->lc != NULL)
	{
		head = transformToDoubleLink4(t->lc, head);
	}

	return head;
}

void BinTree::swap(BinNode* t)
{
	if(t == NULL)
	{
		return;
	}
	queue<BinNode*> que;
	que.push(t);

	while(!que.empty())
	{
		t = que.front();
		que.pop();

		BinNode* tmp = t->lc;
		t->lc = t->rc;
		t->rc = tmp;

		if(t->lc != NULL)
		{
			que.push(t->lc);
		}
		if(t->rc != NULL)
		{
			que.push(t->rc);
		}
	}
}

BinNode* BinTree::find(int val)
{
	BinNode* t = root;
	while(t != NULL)
	{
		if(val < t->value)
		{
			t = t->lc;
		}
		else if(val > t->value)
		{
			t = t->rc;
		}
		else
		{
			return t;
		}
	}
	return NULL;
}

void BinTree::insert(int val)
{
	if(root == NULL)
	{
		root = new BinNode(val);
		return;
	}
	BinNode* t = root;
	BinNode* parent = NULL;

	do 
	{
		parent = t;
		if(val <= t->value)
		{
			t = t->lc;
		}
		else if(val > t->value)
		{
			t = t->rc;
		}
	} while (t != NULL);

	if(val <= parent->value)
	{
		parent->lc = new BinNode(val, NULL, NULL, parent);
	}
	else
	{
		parent->rc = new BinNode(val, NULL, NULL, parent);
	}
}

void showTree(BinTree& ptree, char type = 0)
{
	static int count = 0;
	count++;
	cout<<endl;
	
	//Recursive
	if(type == 0)
	{
		cout<<"=====================Start ("<<count<<") Recursive====================="<<endl;
		ptree.preOrder();
		ptree.inOrder();
		ptree.postOrder();
		
	}
	//Stack
	else if(type == 1)
	{
		cout<<"=====================Start ("<<count<<") Stack========================="<<endl;
		ptree.preOrderStack();
		ptree.inOrderStack();
		ptree.postOrderStack();
	}
	ptree.levelOrder();	
	cout<<"=============================End============================="<<endl;
}

void showTree(BinNode* t, char type = 0)
{
	BinTree ptree(t);
	showTree(ptree, type);
}

void showCompareResult(BinNode* t1, BinNode* t2)
{
	if(compare(t1, t2))
	{
		cout<<"Equals"<<endl;
	}
	else
	{
		cout<<"Differs"<<endl;
	}
}

void testBinTree()
{
#if ((BIN_TREE_TEST) == 1)
	const int n = 10;
	int a[n];
	genseq(a, n);
	output(a, n);

	BinTree binTree(a, n);

	showTree(binTree);
	showTree(binTree, 1);

	int pre[]   = {5, 2, 1, 3, 4, 8, 6, 7, 9, 10};
	int in[]    = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int post[]  = {1, 4, 3, 2, 7, 6, 10, 9, 8, 5};
	int level[] = {5, 2, 8, 1, 3, 6, 9, 4, 7, 10};	
	
	showCompareResult(binTree.getRoot(), createTreeByInLevel(in, level, n));
	showCompareResult(binTree.getRoot(), createTreeByInLevel2(in, level, n));
	showCompareResult(binTree.getRoot(), createTreeByInPre(in, 0, COUNTOF(in) - 1, pre, 0, COUNTOF(in) - 1));
	showCompareResult(binTree.getRoot(), createTreeByInPreWithStack(in, 0, COUNTOF(in) - 1, pre, 0, COUNTOF(pre) - 1));
	showCompareResult(binTree.getRoot(), createTreeByInPost(in, 0, COUNTOF(in) - 1, post, 0, COUNTOF(post) - 1));
	showCompareResult(binTree.getRoot(), createTreeByInPostWithStack(in, 0, COUNTOF(in) - 1, post, 0, COUNTOF(post) - 1));
	showTree(createTreeByInLevel(in, level, n));
	showTree(createTreeByInLevel2(in, level, n));
	showTree(createTreeByInPre(in, 0, COUNTOF(in) - 1, pre, 0, COUNTOF(in) - 1));
	showTree(createTreeByInPreWithStack(in, 0, COUNTOF(in) - 1, pre, 0, COUNTOF(pre) - 1));
	showTree(createTreeByInPost(in, 0, COUNTOF(in) - 1, post, 0, COUNTOF(post) - 1));
	showTree(createTreeByInPostWithStack(in, 0, COUNTOF(in) - 1, post, 0, COUNTOF(post) - 1));
	
	int pos = 1;
	BinNode* binNode = binTree.getNodeByID(binTree.getRoot(), 6);
	if(binNode != NULL)
	{
		if(binTree.getPosOfNode(binTree.getRoot(), binNode, &pos))
		{
			cout<<"Node="<<binNode->value<<", pos="<<pos<<endl;
		}
	}

	BinNode* binNode2 = binTree.getNodeByID(binTree.getRoot(), 10);
	BinNode* parentNode = binTree.findLatestParent(binNode, binNode2);
	if(parentNode != NULL)
	{
		cout<<"Parent("<<binNode->value<<","<<binNode2->value<<")="<<parentNode->value<<endl;
	}
	else
	{
		cout<<"Can't find parent"<<endl;
	}

	parentNode = NULL;
	parentNode = binTree.findLatestParentStack(binNode, binNode2);
	if(parentNode != NULL)
	{
		cout<<"Parent("<<binNode->value<<","<<binNode2->value<<")="<<parentNode->value<<endl;
	}
	else
	{
		cout<<"Can't find parent"<<endl;
	}

	parentNode = NULL;
	parentNode = binTree.findLatestParentPtr(binNode, binNode2);
	if(parentNode != NULL)
	{
		cout<<"Parent("<<binNode->value<<","<<binNode2->value<<")="<<parentNode->value<<endl;
	}
	else
	{
		cout<<"Can't find parent"<<endl;
	}

	stack<BinNode*> stPath;
	binTree.findPath(binTree.getRoot(), 3, stPath, false);

	int b[n] = {1,4,3/*,2,7,6,10,9,8,8*/};
	cout<<checkSeqIfPostOrderOfBST(b, n)<<endl;

	BinNode* closest = NULL;
	
	for(int i = 0; i < n + 2; i++)
	{
		closest = binTree.findClosestToMid(i);
		if(closest != NULL)
		{
			cout<<i<<","<<closest->value<<endl;
		}
	}

//	binTree.swap(binTree.getRoot());

	DoubleLinkNode *tail = /*new DoubleLinkNode(-1)*/ NULL;
	binTree.transformToDoubleLink2(binTree.getRoot(), tail);

 	DoubleLinkNode *pdnode = NULL;
// 	while(pdnode != NULL)
// 	{
// 		cout<<pdnode->value<<" ";
// 		pdnode = pdnode->next;
// 
// 	}
// 	cout<<endl;

	pdnode = tail;
	while(pdnode != NULL)
	{
		cout<<pdnode->value<<" ";
		pdnode = pdnode->next;
	}
	cout<<endl;

	BinNode* bhead = NULL;
//	binTree.transformToDoubleLink3(binTree.getRoot(), bhead);
// 	bhead = binTree.transformToDoubleLink4(binTree.getRoot(), bhead);
// 
// 	BinNode* pnode = bhead, *pnode_last = bhead;
// 	while(pnode != NULL)
// 	{
// 		pnode_last = pnode;
// 		cout<<pnode->value<<" ";
// 		pnode = pnode->rc;
// 	}
// 	cout<<endl;
// 
// 	cout<<pnode_last->value<<endl;
// 
// 	pnode = pnode_last;
// 	while(pnode != NULL)
// 	{
// 		cout<<pnode->value<<" ";
// 		pnode = pnode->lc;
// 	}
// 	cout<<endl;
	printSep(__FILE__);
#endif
}