// ChoiceOptionExp.cpp: implementation of the ChoiceOptionExp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChoiceOptionExp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ChoiceOptionExp::ChoiceOptionExp() : m_pChoices(NULL), m_pChoiceOptID(NULL)
{

}

ChoiceOptionExp::~ChoiceOptionExp()
{
	if(m_pChoices != NULL)
	{
		delete m_pChoices;
		m_pChoices = NULL;
	}
	if(m_pChoiceOptID != NULL)
	{
		delete m_pChoiceOptID;
		m_pChoiceOptID = NULL;
	}
// 	if(m_pChoiceOptValue != NULL)
// 	{
// 		delete m_pChoiceOptValue;
// 		m_pChoiceOptValue = NULL;
// 	}
}

bool ChoiceOptionExp::SelectOptionExp(OptionContext* pContext)
{
	if(m_pChoiceOptID == NULL || m_pChoices == NULL)
	{
		return false;
	}
	std::string val;
	bool bRet = pContext->Get(m_pChoiceOptID->c_str(), val);
	if(!bRet)
	{
		return false;
	}
	OptionGroupMap::iterator iter = m_pChoices->find(val);
	if(iter == m_pChoices->end())
	{
		return false;
	}

	RemoveChildren(false);

	OptionExpVec* pOptionVec = iter->second;
	OptionExp* pExp;
	for(int i = 0; i < pOptionVec->size(); i++)
	{
		pExp = pOptionVec->at(i);
		AddChild(pExp);
	}
	return true;
}

ChoiceOptionExp& ChoiceOptionExp::SetChoiceOptionID(const char* str)
{
	if(str == NULL)
	{
		return *this;
	}
	if(m_pChoiceOptID == NULL)
	{
		m_pChoiceOptID = new std::string;
	}
	m_pChoiceOptID->assign(str);
	return *this;
}

void ChoiceOptionExp::AddChoice(const char* szChoiceVal, OptionExp* pChoiceExp)
{
	if(szChoiceVal == NULL || pChoiceExp == NULL)
	{
		return;
	}
	if(m_pChoices == NULL)
	{
		m_pChoices = new OptionGroupMap;
	}
	OptionGroupMap::iterator iter = m_pChoices->find(szChoiceVal);

	OptionExpVec* pOptionVec = NULL;
	if(iter != m_pChoices->end())
	{
		pOptionVec = iter->second;
	}
	else
	{
		pOptionVec = new OptionExpVec;
		m_pChoices->insert(std::make_pair<std::string, OptionExpVec*>(szChoiceVal, pOptionVec));
	}
	pOptionVec->push_back(pChoiceExp);
}

bool ChoiceOptionExp::Evaluate(OptionContext* pContext, std::string& szResult)
{
	bool bRet = SelectOptionExp(pContext);
	if(!bRet)
	{
		return false;
	}
	return DefaultOptionExp::Evaluate(pContext, szResult);
}
