#ifndef _BIN_TREE_H_
#define _BIN_TREE_H_

#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

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
	BinTree() : root(NULL) {}
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

	BinNode* findLatestParent(BinNode* binNode1, BinNode* binNode2);
private:
	BinNode* buildInOrder(int data[], int start, int end);
	void deleteTree(BinNode* t);
	void preOrderHelper(BinNode* t, Visitor* visitor);
	void inOrderHelper(BinNode* t, Visitor* visitor);
	void postOrderHelper(BinNode* t, Visitor* visitor);
private:
	BinNode* root;
};
#endif