// OptionExpTree.cpp: implementation of the OptionExpTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionExpTree.h"
#include "DefaultOptionExp.h"
#include <stack>
#include <list>
#include "cflbase/tstring.h"
#include "OptionExpUtils.h"
#include "OptionExpDef.h"
#include "log4cplus_config.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DECLARE_THE_LOGGER_NAME(LIB_LOGGER_PREFIX _T(".OptionExpTree"))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OptionExpTree::OptionExpTree() : m_pRoot(NULL)
{

}

OptionExpTree::~OptionExpTree()
{
	Destroy();
}

bool OptionExpTree::Build(OptionContext* pContext, OptionExpBuilder* pBuilder)
{
	if(pContext == NULL || pBuilder == NULL)
	{
		return false;
	}
	Destroy();
	m_pRoot = pBuilder->Build(pContext);
	return (m_pRoot != NULL);
}

void OptionExpTree::Destroy()
{
	if(m_pRoot != NULL)
	{
		delete m_pRoot;
		m_pRoot = NULL;
	}
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
					if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
					{
						cfl::tstring szLog;
						cfl::tformat(szLog, _T("[stack push]: id=%s,name=%s,value=%s"), 
							CFL_A2T(SafeStrA(pChild->GetFieldStr(OPT_FIELD_ID))),
							CFL_A2T(SafeStrA(pChild->GetFieldStr(OPT_FIELD_NAME))), 
							CFL_A2T(SafeStrA(pChild->GetFieldStr(OPT_FIELD_VALUE))));
						LOG4CPLUS_TRACE_STR(THE_LOGGER, szLog)
					}

					//visit
					optExpStack.push(pChild);

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
		if(IS_LOG_ENABLED(THE_LOGGER, log4cplus::TRACE_LOG_LEVEL))
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("[stack  pop]: id=%s,name=%s,value=%s"), 
				CFL_A2T(SafeStrA(pExp->GetFieldStr(OPT_FIELD_ID))),
				CFL_A2T(SafeStrA(pExp->GetFieldStr(OPT_FIELD_NAME))), 
				CFL_A2T(SafeStrA(pExp->GetFieldStr(OPT_FIELD_VALUE))));
			LOG4CPLUS_TRACE_STR(THE_LOGGER, szLog)
		}
		if(!pExp->Evaluate(pContext, val))
		{
			return false;
		}
	}
	return true;
}
