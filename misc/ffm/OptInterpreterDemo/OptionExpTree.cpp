// OptionExpTree.cpp: implementation of the OptionExpTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionExpTree.h"
#include <stack>
#include <list>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OptionExpTree::OptionExpTree() : m_pRoot(NULL)
{

}

OptionExpTree::~OptionExpTree()
{
	if(m_pRoot != NULL)
	{
		delete m_pRoot;
		m_pRoot = NULL;
	}
}

void OptionExpTree::SetRoot(OptionExp* pRoot)
{
	m_pRoot = pRoot;
}
OptionExp* OptionExpTree::GetRoot()
{
	return m_pRoot;
}

bool OptionExpTree::Evaluate(OptionContext* pContext, std::string& szResult)
{
	if(m_pRoot == NULL)
	{
		return false;
	}

	//pre-process
	//evaluate the child node first
	if(!PostEvaluateGroup(pContext))
	{
		return false;
	}

	//evaluate from the root node now
	return m_pRoot->Evaluate(pContext, szResult);
}

bool OptionExpTree::PostEvaluateGroup(OptionContext* pContext)
{
	//node that has children, but excludes root
	std::stack<OptionExp*> optExpStack;
	
	std::list<OptionExp*> optExpList;
	optExpList.push_back(m_pRoot);

	OptionExp *pExp, *pChild;
	while(!optExpList.empty())
	{
		pExp = optExpList.front();
		optExpList.pop_front();

		if(pExp->GetChildCount() > 0)
		{
			for(int i = pExp->GetChildCount() - 1; i >= 0; i--)
			{
				pChild = pExp->GetChild(i);
				if(pChild->GetChildCount() > 0)
				{
					//visit
					optExpStack.push(pExp);

					//update data list
					optExpList.push_back(pChild);
				}
			}
		}
	}

	std::string val;
	while(!optExpStack.empty())
	{
		pExp = optExpStack.top();
		optExpStack.pop();

		if(!pExp->Evaluate(pContext, val))
		{
			return false;
		}
	}
	return true;
}
