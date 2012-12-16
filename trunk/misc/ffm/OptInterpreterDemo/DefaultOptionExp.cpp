// DefaultOptionExp.cpp: implementation of the DefaultOptionExp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DefaultOptionExp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DefaultOptionExp::DefaultOptionExp() : m_nEvaluateMode(OPTEM_DEFAULT), m_nEvaluateFlag(OPT_HAS_CHILDREN)
{
// 	m_pOptID = (NULL);
// 	m_pOptName = (NULL);
// 	m_pOptValue = (NULL);
// 	m_pNameValueSep = (NULL);
// 	m_pSubOptionSep = (NULL);	
}

DefaultOptionExp::~DefaultOptionExp()
{
	if(m_pChildren != NULL)
	{
		m_pChildren->clear();
		delete m_pChildren;
		m_pChildren = NULL;
	}
	/*
	if(m_pOptID != NULL)
	{
		delete m_pOptID;
		m_pOptID = NULL;
	}
	if(m_pOptName != NULL)
	{
		delete m_pOptName;
		m_pOptName = NULL;
	}
	if(m_pOptValue != NULL)
	{
		delete m_pOptValue;
		m_pOptValue = NULL;
	}
	if(m_pNameValueSep != NULL)
	{
		delete m_pNameValueSep;
		m_pNameValueSep = NULL;
	}
	if(m_pSubOptionSep != NULL)
	{
		delete m_pSubOptionSep;
		m_pSubOptionSep = NULL;
	}
	*/
	if(m_pLocalContext != NULL)
	{
		std::string* ptr;
		FieldMap::iterator iter = m_pLocalContext->begin();
		for( ; iter != m_pLocalContext->end(); iter++)
		{
			ptr = iter->second;
			if(ptr != NULL)
			{
				delete ptr;
			}
		}
	}
}

bool DefaultOptionExp::Evaluate(OptionContext* pContext, std::string& szResult)
{
	std::string val;
	bool bRet = ((m_nEvaluateFlag & OPTEF_NONE) != 0);
	
	//1. child first
	if(!bRet && (m_nEvaluateFlag & OPTEF_CHILDREN))
	{
		bRet = EvaluateChildren(pContext, val);
	}
	//2. context
	if(!bRet && (m_nEvaluateFlag & OPTEF_CONTEXT))
	{
		bRet = EvaluateContext(pContext, val);
	}
	//3. self default value
	if(!bRet && (m_nEvaluateFlag & OPTEF_SELF))
	{
		bRet = EvaluateSelf(val);
	}

	szResult.erase();
	if(!bRet)
	{
		return ((m_nEvaluateFlag & OPTEF_MUST) == 0);
	}

	int nModeAction = GET_OPT_MODE_ACTION(m_nEvaluateMode);
	int nModeFrom = GET_OPT_MODE_FROM(m_nEvaluateMode);
	int nModeTo = GET_OPT_MODE_TO(m_nEvaluateMode);

	//name only. flag type
	if(nModeAction == OPTEM_NAME_ONLY)
	{
		std::string* ptr = GetField(OPT_FIELD_NAME);
		ASSERT(ptr != NULL);
		szResult.assign(*ptr);
	}
	//value only. filter anonymous parameters, input file
	else if(nModeAction == OPTEM_VALUE_ONLY)
	{
		szResult.assign(val);
	}
	//set the context name
	else if(nModeAction == OPTEM_SET_CONTEXT)
	{
		std::string *pOptID = GetField(OPT_FIELD_ID);
		std::string *pFromVal = GetField(nModeFrom);

		ASSERT(pFromVal);
		if(pFromVal == NULL)
		{
			return false;
		}
		pContext->Put(pOptID->c_str(), pFromVal->c_str());
	}
	//set the context value
	else if(nModeAction == OPTEM_SET_PARENT)
	{
		std::string *pOptFrom = GetField(nModeFrom);
		OptionExp* pParent = GetParent();

		ASSERT(pOptFrom != NULL && pParent != NULL);
		if(pOptFrom == NULL || pParent == NULL)
		{
			return false;
		}

		pParent->SetField(nModeTo, pOptFrom->c_str());
	}
	//normal case. option<sep>optionval form
	else
	{
		std::string *pOptName = GetField(OPT_FIELD_NAME);
		std::string *pNameValueSep = GetField(OPT_FIELD_NAME_VALUE_SEP);

		ASSERT(pOptName != NULL && pNameValueSep != NULL);
		if(!val.empty())
		{
			szResult.assign(*pOptName).append(*pNameValueSep).append(val);
		}
	}
	return true;
}

bool DefaultOptionExp::EvaluateChildren(OptionContext* pContext, std::string& szResult)
{
	if(m_pChildren == NULL || m_pChildren->empty())
	{
		return false;
	}

	std::string* pSubOptionSep = GetField(OPT_FIELD_SUB_OPTION_SEP);
	ASSERT(pSubOptionSep != NULL);

	std::string subOption, val;
	OptionExp* pExp = NULL;
	for(int i = 0; i < m_pChildren->size(); i++)
	{
		pExp = m_pChildren->at(i);

		if(!pExp->Evaluate(pContext, val))
		{
			return false;
		}
		
		if(val.empty())
		{
			continue;
		}

		if(!subOption.empty())
		{
			subOption.append(*pSubOptionSep);
		}
		subOption.append(val);
	}

	szResult.assign(subOption);
	return true;
}
bool DefaultOptionExp::EvaluateContext(OptionContext* pContext, std::string& szResult)
{
	std::string* pOptID = GetField(OPT_FIELD_ID);
	return pContext->Get(pOptID->c_str(), szResult);
}
bool DefaultOptionExp::EvaluateSelf(std::string& szResult)
{
	std::string* pOptValue = GetField(OPT_FIELD_VALUE);
	if(pOptValue == NULL)
	{
		return false;
	}
	szResult.assign(*pOptValue);
	return true;
}

void DefaultOptionExp::SetStringPtr(std::string* & pStr, const char* str)
{
	if(str == NULL)
	{
		return;
	}
	if(pStr == NULL)
	{
		pStr = new std::string;
	}
	pStr->assign(str);
}

DefaultOptionExp& DefaultOptionExp::SetOptionID(const char* str)
{
	OptionExp::SetField(OPT_FIELD_ID, str);
	//SetStringPtr(m_pOptID, str);
	return *this;
}
DefaultOptionExp& DefaultOptionExp::SetOptionName(const char* str)
{
	OptionExp::SetField(OPT_FIELD_NAME, str);
	//SetStringPtr(m_pOptName, str);
	return *this;
}
DefaultOptionExp& DefaultOptionExp::SetOptionValue(const char* str)
{
	OptionExp::SetField(OPT_FIELD_VALUE, str);
	//SetStringPtr(m_pOptValue, str);
	return *this;
}
DefaultOptionExp& DefaultOptionExp::SetNameValueSep(const char* sep)
{
	OptionExp::SetField(OPT_FIELD_NAME_VALUE_SEP, sep);
	//SetStringPtr(m_pNameValueSep, sep);
	return *this;
}
DefaultOptionExp& DefaultOptionExp::SetSubOptionSep(const char* sep)
{
	OptionExp::SetField(OPT_FIELD_SUB_OPTION_SEP, sep);
	//SetStringPtr(m_pSubOptionSep, sep);
	return *this;
}

DefaultOptionExp& DefaultOptionExp::SetEvaluateMode(int nMode)
{
	m_nEvaluateMode = nMode;
	return *this;
}
DefaultOptionExp& DefaultOptionExp::SetEvaluateFlag(int nFlag)
{
	m_nEvaluateFlag = nFlag;
	return *this;
}

const char* DefaultOptionExp::GetOptionID()
{
	//return (m_pOptID ? m_pOptID->c_str() : NULL);
	return GetFieldStr(OPT_FIELD_ID);
}
const char* DefaultOptionExp::GetOptionName()
{
	//return (m_pOptName ? m_pOptName->c_str() : NULL);
	return GetFieldStr(OPT_FIELD_NAME);
}
const char* DefaultOptionExp::GetOptionValue()
{
	//return (m_pOptValue ? m_pOptValue->c_str() : NULL);
	return GetFieldStr(OPT_FIELD_VALUE);
}
const char* DefaultOptionExp::GetNameValueSep()
{
	//return (m_pNameValueSep ? m_pNameValueSep->c_str() : NULL);
	return GetFieldStr(OPT_FIELD_NAME_VALUE_SEP);
}
const char* DefaultOptionExp::GetSubOptionSep()
{
	//return (m_pSubOptionSep ? m_pSubOptionSep->c_str() : NULL);
	return GetFieldStr(OPT_FIELD_SUB_OPTION_SEP);
}
int  DefaultOptionExp::GetEvaluateMode()
{
	return m_nEvaluateMode;
}
int  DefaultOptionExp::GeEvalueateFlag()
{
	return m_nEvaluateFlag;
}
