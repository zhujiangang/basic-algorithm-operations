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

DefaultOptionExp::DefaultOptionExp() : m_nEvaluateMode(OPTEM_DEFAULT), 
m_nEvaluateFlag(OPTEF_CHILDREN | OPTEF_CONTEXT | OPTEF_SELF), m_pFuncSet(NULL)
{
}

DefaultOptionExp::~DefaultOptionExp()
{
}

bool DefaultOptionExp::Evaluate(OptionContext* pContext, std::string& szResult)
{
	std::string val;
	bool bRet = ((m_nEvaluateFlag & OPTEF_NONE) != 0);
	
	const char* szName = GetFieldStr(OPT_FIELD_NAME);
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

	if(m_pFuncSet)
	{
		//only need to be called once, not called yet
		if( (m_nEvaluateFlag & OPTEF_FUNC_ONCE) && ((GetFieldInt(OPT_FIELD_RESULT_FLAG) & OPTRF_FUNC_CALLED) == 0) )
		{
			(m_pFuncSet)(this, pContext, (bRet ? &val : NULL));
			SetFieldInt(OPT_FIELD_RESULT_FLAG, GetFieldInt(OPT_FIELD_RESULT_FLAG) | OPTRF_FUNC_CALLED);
		}		
	}

	szResult.erase();
	if(!bRet)
	{
		//failed if the option is must
		return ((m_nEvaluateFlag & OPTEF_MUST) == 0);
	}

	int nModeAction = GET_OPT_MODE_ACTION(m_nEvaluateMode);
	int nModeFrom = GET_OPT_MODE_FROM(m_nEvaluateMode);
	int nModeTo = GET_OPT_MODE_TO(m_nEvaluateMode);

	//name only. flag type
	if(nModeAction == OPTEM_NAME_ONLY)
	{
		std::string* ptr = GetField(OPT_FIELD_NAME);
		if(ptr == NULL)
		{
			return false;
		}
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

		if(pOptID == NULL || pFromVal == NULL)
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

		if(pOptName == NULL || pNameValueSep == NULL)
		{
			return false;
		}

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
		
		//ignore empty string if it is not allowed
		if(val.empty() && (m_nEvaluateFlag & OPTEF_KEEP_EMPTY) == 0)
		{
			continue;
		}

		//(1). option string is not empty
		//(2). force add separator for non-first option, when empty string is allowed
		if(!subOption.empty() || ((i > 0) && (m_nEvaluateFlag & OPTEF_KEEP_EMPTY)))
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

DefaultOptionExp& DefaultOptionExp::SetOptionID(const char* str)
{
	OptionExp::SetField(OPT_FIELD_ID, str);
	return *this;
}
DefaultOptionExp& DefaultOptionExp::SetOptionName(const char* str)
{
	OptionExp::SetField(OPT_FIELD_NAME, str);
	return *this;
}
DefaultOptionExp& DefaultOptionExp::SetOptionValue(const char* str)
{
	OptionExp::SetField(OPT_FIELD_VALUE, str);
	return *this;
}
DefaultOptionExp& DefaultOptionExp::SetNameValueSep(const char* sep)
{
	OptionExp::SetField(OPT_FIELD_NAME_VALUE_SEP, sep);
	return *this;
}
DefaultOptionExp& DefaultOptionExp::SetSubOptionSep(const char* sep)
{
	OptionExp::SetField(OPT_FIELD_SUB_OPTION_SEP, sep);
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
DefaultOptionExp& DefaultOptionExp::SetFuncSet(FuncSet pf)
{
	m_pFuncSet = pf;
	return *this;
}
