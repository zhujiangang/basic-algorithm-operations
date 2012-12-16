// OptionExpBuilder.h: interface for the OptionExpBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONEXPBUILDER_H__B57E4388_32BD_4468_98CB_E247D60E4C77__INCLUDED_)
#define AFX_OPTIONEXPBUILDER_H__B57E4388_32BD_4468_98CB_E247D60E4C77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OptionExp.h"
#include "DefaultOptionExp.h"

#define SOT_CHOICE	1
#define SOT_LIST	2

class OptionParam
{
public:
	const char*		szID;
	const char*		szName;
	const char*		szValue;
	const char*		szNameValueSep;
	const char*		szSubOptionSep;
	int				nEvaluateMode;
	int				nEvaluateFlag;
	
	void*			pChildren;
// 	OptionParam*	pSubOptions;
// 	int				nSubOptionCount;
};

class BaseSubOptionParam
{
public:
	int				nType;
};

class OptionParamList
{
public:
	int					nType;
	const OptionParam*	pOptions;
	int					nOptionCount;
};

class NamedOptionGroup
{
public:
	const char*		szGroupName;
	const OptionParam*	pOptions;
	int					nOptionCount;
};

class ChoiceOptionParam
{
public:
	int				nType;
	const char*		szChoiceOptionID;
	const NamedOptionGroup*	pGroups;
	int				nGroupCount;
};

bool IsParamValid(const OptionParam* pParam);

class OptionParams
{
public:
	const OptionParam*	pSubOptions;
	int					nSubOptionCount;
};

bool SomeFunc(OptionContext* pContext, DefaultOptionExp* pExp, OptionParams* pParams);

class OptionExpBuilder  
{
public:
	OptionExpBuilder();
	virtual ~OptionExpBuilder();

	virtual OptionExp* Build(const OptionParam* pParams, int nCount);
	virtual	OptionExp* Build(const OptionParam* pParam);
};

#endif // !defined(AFX_OPTIONEXPBUILDER_H__B57E4388_32BD_4468_98CB_E247D60E4C77__INCLUDED_)
