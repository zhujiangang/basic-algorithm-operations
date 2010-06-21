#ifndef _BIN_TREE_H_
#define _BIN_TREE_H_

#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#include <stack>

using std::stack;

class BinNode
{
public:
	BinNode(int val = -1, BinNode* left = NULL, BinNode* right = NULL, BinNode* parent = NULL)
		: value(val), lc(left), rc(right), p(parent)
	{
	}
	bool isLeaf()
	{
		return lc == NULL && rc == NULL;
	}
public:
	int value;
	BinNode* lc;
	BinNode* rc;
	BinNode* p;
};

class DoubleLinkNode
{
public:
	DoubleLinkNode(int val = -1, DoubleLinkNode* pPrev = NULL, DoubleLinkNode* pNext = NULL)
		: value(val), prev(pPrev), next(pNext)
	{
	}
public:
	int value;
	DoubleLinkNode* prev;
	DoubleLinkNode* next;	
};

//The information about level-order and in-order
class LevelOrderVisitNode
{
public:
	BinNode* p; //Parent node or the current node.
	int lvl;    //Level-order index, it's the root node of this subtree
	int l, h;	//In-order sequence low and high bound
	int lr;     //left or right child?
};

//The information about pre-order or post-order and in-order
class PreOrPostOrderVisitNode
{
public:
	BinNode* p;
	int li, hi;
	int lp, hp;
};

BinNode* createTreeByInLevel(const int in[], const int level[], int n);
BinNode* createTreeByInLevel2(const int in[], const int level[], int n);

BinNode* createTreeByInPre(const int in[], int inBegin, int inEnd, const int pre[], int preBegin, int preEnd);
BinNode* createTreeByInPost(const int in[], int inBegin, int inEnd, const int post[], int postBegin, int postEnd);


BinNode* createTreeByInPreWithStack(const int in[], int inBegin, int inEnd, const int pre[], int preBegin, int preEnd);
BinNode* createTreeByInPostWithStack(const int in[], int inBegin, int inEnd, const int post[], int postBegin, int postEnd);


class Visitor
{
public:
	virtual void visit(BinNode* root) = 0;
};

class OutputVisitor : public Visitor
{
public:
	virtual void visit(BinNode* root);
};

extern OutputVisitor outputVisitor;

enum ChildType
{
	LEFT_CHILD,
	RIGHT_CHILD
};

enum VisitType
{
	PRE_ORDER,
	IN_ORDER,
	POST_ORDER,
	LEVEL_ORDER,
	PRE_ORDER_STACK,
	IN_ORDER_STACK,
	POST_ORDER_STACK,
	PRE_ORDER_STACK_B,
	IN_ORDER_STACK_B,
	POST_ORDER_STACK_B
};

class BinTree
{
public:
	BinTree(BinNode* t = NULL) : root(t) {}
	BinTree(int data[], int len);
	virtual ~BinTree();
	BinNode* getRoot()
	{
		return this->root;
	}
	void preOrder(Visitor* visitor = &outputVisitor);
	void inOrder(Visitor* visitor = &outputVisitor);
	void postOrder(Visitor* visitor = &outputVisitor);
	void levelOrder(Visitor* visitor = &outputVisitor);
	void preOrderStack(Visitor* visitor = &outputVisitor);
	void inOrderStack(Visitor* visitor = &outputVisitor);
	void postOrderStack(Visitor* visitor = &outputVisitor);

// 	void preOrderStackB(Visitor* visitor = &outputVisitor);
// 	void inOrderStackB(Visitor* visitor = &outputVisitor);
// 	void postOrderStackB(Visitor* visitor = &outputVisitor);

	virtual void preVisit(VisitType vt, Visitor* visitor = &outputVisitor);
	virtual void postVisit(Visitor* visitor = &outputVisitor, void* param = NULL);

	BinNode* getNodeByID(BinNode* t, int id);
	bool getPosOfNode(BinNode* t, BinNode* binNode, int* pos);
	int findParentPos(int pos1, int pos2);
	BinNode* getNodeByPos(int pos);

	/************************************************************************/
	/* Find the latest parent node of 2 nodes without stack                 */
	/************************************************************************/
	BinNode* findLatestParent(BinNode* binNode1, BinNode* binNode2);


	/************************************************************************/
	/* Find the latest parent node of 2 nodes without stack, the node has parent point
	/************************************************************************/
	BinNode* findLatestParentPtr(BinNode* binNode1, BinNode* binNode2);


	/************************************************************************/
	/* Find the latest parent node of 2 nodes with stack                 */
	/************************************************************************/
	BinNode* findLatestParentStack(BinNode* binNode1, BinNode* binNode2);

	bool getPosOfNode(BinNode* t, BinNode* binNode, stack<BinNode*>& st);

	/************************************************************************/
	/* Find a path that the sum of all the nodes equals to sum              
	   The result includes the paths that not start from root node.
	/************************************************************************/
	void findPath(BinNode* t, int sum, stack<BinNode*>& st, bool isParentInPath);


	/************************************************************************/
	/* Suppose f(n)=(min+max)/2, find the closest node that greater than f(n)
	/************************************************************************/
	BinNode* findClosestToMid(int x = -1);


	/************************************************************************/
	/* transform the binary tree to a double-link                           */
	/************************************************************************/
	void transformToDoubleLink(BinNode* t, DoubleLinkNode*& tail);
	void transformToDoubleLink2(BinNode* t, DoubleLinkNode*& head);

	//This implement doesn't create a new double-link, but use the original tree space,
	//take left-child as the previous pointer, right-child as the next pointer
	void transformToDoubleLink3(BinNode* t, BinNode*& head);

	BinNode* transformToDoubleLink4(BinNode* t, BinNode* head);

	/************************************************************************/
	/* Swap the left and right children of the tree and all sub-trees       */
	/************************************************************************/
	void swap(BinNode* t);

	BinNode* find(int val);
	void insert(int val);
private:
	BinNode* buildInOrder(int data[], int start, int end);
	void deleteTree(BinNode* t);
	void preOrderHelper(BinNode* t, Visitor* visitor);
	void inOrderHelper(BinNode* t, Visitor* visitor);
	void postOrderHelper(BinNode* t, Visitor* visitor);
private:
	BinNode* root;
};

bool compare(BinNode* t1, BinNode* t2);
bool compare(BinTree& t1, BinTree& t2);
#endif