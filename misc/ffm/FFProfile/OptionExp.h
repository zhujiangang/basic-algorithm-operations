// OptionExp.h: interface for the OptionExp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONEXP_H__90B7E96C_983F_4607_B818_945BF6137CBC__INCLUDED_)
#define AFX_OPTIONEXP_H__90B7E96C_983F_4607_B818_945BF6137CBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>
#include <string>
#include "OptionContext.h"

class OptionExp  
{
public:
	OptionExp();
	virtual ~OptionExp();

	virtual bool Evaluate(OptionContext* pContext, std::string& szResult) = 0;

public:
	virtual OptionExp* GetParent();
	virtual OptionExp* GetChild(int childIndex);
	virtual int GetChildCount();

	virtual OptionExp& SetParent(OptionExp* pParent);
	virtual OptionExp& AddChild(OptionExp* pChild);

	virtual OptionExp& SetField(int nFieldID, const char* val);
	virtual std::string* GetField(int nFieldID);
	virtual const char* GetFieldStr(int nFieldID);

protected:
	typedef	std::vector< OptionExp* >	OptionExpVec;
	typedef std::map<int, std::string*>	FieldMap;
	
	OptionExp*		m_pParent;
	OptionExpVec*	m_pChildren;
	FieldMap*		m_pLocalContext;
};

class OptionExpBuilder
{
public:
	OptionExpBuilder() {}
	virtual ~OptionExpBuilder() {}
	
	virtual OptionExp* Build(OptionContext* pContext) = 0;
};

#endif // !defined(AFX_OPTIONEXP_H__90B7E96C_983F_4607_B818_945BF6137CBC__INCLUDED_)
