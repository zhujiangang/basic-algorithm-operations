// DefaultOptionExp.h: interface for the DefaultOptionExp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFAULTOPTIONEXP_H__E45B6945_563E_413F_AC71_4332EA1B7044__INCLUDED_)
#define AFX_DEFAULTOPTIONEXP_H__E45B6945_563E_413F_AC71_4332EA1B7044__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OptionExp.h"
#include <vector>

class DefaultOptionExp : public OptionExp  
{
public:
	DefaultOptionExp();
	virtual ~DefaultOptionExp();
	
	//value evaluate by the order: children -> context value -> self value
	virtual bool Evaluate(OptionContext* pContext, std::string& szResult);

public:
	DefaultOptionExp& SetOptionID(const char* str);
	DefaultOptionExp& SetOptionName(const char* str);
	DefaultOptionExp& SetOptionValue(const char* str);
	DefaultOptionExp& SetNameValueSep(const char* sep);
	DefaultOptionExp& SetSubOptionSep(const char* sep);
	DefaultOptionExp& SetEvaluateMode(int nMode);
	DefaultOptionExp& SetEvaluateFlag(int nFlag);

	const char* GetOptionID();
	const char* GetOptionName();
	const char* GetOptionValue();
	const char* GetNameValueSep();
	const char* GetSubOptionSep();
	int  GetEvaluateMode();
	int  GeEvalueateFlag();

private:
	inline void SetStringPtr(std::string* & pStr, const char* str);
	bool EvaluateChildren(OptionContext* pContext, std::string& szResult);
	bool EvaluateContext(OptionContext* pContext, std::string& szResult);
	bool EvaluateSelf(std::string& szResult);
protected:
	typedef	std::vector< OptionExp* >	OptionExpVec;
// 	std::string*	m_pOptID;
// 	std::string*	m_pOptName;
// 	std::string*	m_pOptValue;
// 	std::string*	m_pNameValueSep;
// 	std::string*	m_pSubOptionSep;
	int				m_nEvaluateMode;
	int				m_nEvaluateFlag;
};

#endif // !defined(AFX_DEFAULTOPTIONEXP_H__E45B6945_563E_413F_AC71_4332EA1B7044__INCLUDED_)
