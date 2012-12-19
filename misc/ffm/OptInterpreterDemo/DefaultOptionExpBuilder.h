#ifndef __DEFAULT_OPTION_EXP_BUILDER_H__
#define __DEFAULT_OPTION_EXP_BUILDER_H__

#pragma once

#include "OptionExp.h"

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

class DefaultOptionExpBuilder : public OptionExpBuilder
{
public:
	DefaultOptionExpBuilder();
	virtual ~DefaultOptionExpBuilder();
	
	virtual OptionExp* Build(OptionContext* pContext);
	//virtual bool BuildTree(OptionContext* pContext, OptionExpTree* pTree);
	
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

#endif
