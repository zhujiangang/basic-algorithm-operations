// OptionExpBuilder.h: interface for the OptionExpBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONEXPBUILDER_H__B57E4388_32BD_4468_98CB_E247D60E4C77__INCLUDED_)
#define AFX_OPTIONEXPBUILDER_H__B57E4388_32BD_4468_98CB_E247D60E4C77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OptionExp.h"
#include "OptionExpTree.h"

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
};

enum SubOptionType
{
	SOT_CHOICE = 1,
	SOT_LIST
};

class OptionParamList
{
public:
	const OptionParam*	pOptions;
	int					nCount;
};

class DirectOptionParamList
{
public:
	int					nType;
	const OptionParam*	pOptions;
	int					nOptionCount;
};

class NamedOptionParamList
{
public:
	const char*			szName;
	const OptionParam*	pOptions;
	int					nOptionCount;
};

class ChoiceOptionParamList
{
public:
	int				nType;
	const char*		szChoiceOptionID;
	const NamedOptionParamList*	pGroups;
	int				nGroupCount;
};

bool IsParamValid(const OptionParam* pParam);

class OptionExpBuilder  
{
public:
	OptionExpBuilder();
	virtual ~OptionExpBuilder();

	virtual bool BuildTree(OptionContext* pContext, OptionExpTree* pTree);

	//Build the option root tree
	virtual OptionExp* BuildRoot(const OptionParam* pParams, int nCount, OptionContext* pContext);

	//Build one option node
	virtual	OptionExp* Build(const OptionParam* pParam, OptionContext* pContext);

private:
	//Parse the option parameters
	bool ParseOptionParamArray(void* ptr, OptionContext* pContext, OptionParamList* pArray);

	//Parse choice options
	bool ParseChoiceOptions(ChoiceOptionParamList* ptr, OptionContext* pContext, OptionParamList* pArray);
};

#endif // !defined(AFX_OPTIONEXPBUILDER_H__B57E4388_32BD_4468_98CB_E247D60E4C77__INCLUDED_)
