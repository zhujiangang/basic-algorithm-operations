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

private:
	bool EvaluateChildren(OptionContext* pContext, std::string& szResult);
	bool EvaluateContext(OptionContext* pContext, std::string& szResult);
	bool EvaluateSelf(std::string& szResult);
protected:
	int				m_nEvaluateMode;
	int				m_nEvaluateFlag;
};

#endif // !defined(AFX_DEFAULTOPTIONEXP_H__E45B6945_563E_413F_AC71_4332EA1B7044__INCLUDED_)
