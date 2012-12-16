// ChoiceOptionExp.h: interface for the ChoiceOptionExp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHOICEOPTIONEXP_H__4BEFF245_A6E4_4933_935B_66A8E57936F0__INCLUDED_)
#define AFX_CHOICEOPTIONEXP_H__4BEFF245_A6E4_4933_935B_66A8E57936F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OptionExp.h"
#include "DefaultOptionExp.h"
#include <map>

class ChoiceOptionExp : public DefaultOptionExp  
{
public:
	ChoiceOptionExp();
	virtual ~ChoiceOptionExp();

	virtual bool Evaluate(OptionContext* pContext, std::string& szResult);

public:
	virtual bool SelectOptionExp(OptionContext* pContext);

	ChoiceOptionExp& SetChoiceOptionID(const char* str);
	void AddChoice(const char* szChoiceVal, OptionExp* pChoiceExp);

protected:
	typedef std::vector<OptionExp*>	OptionExpVec;
	typedef std::map<std::string, OptionExpVec*> OptionGroupMap;
	OptionGroupMap*	m_pChoices;
	std::string*	m_pChoiceOptID;
//	std::string*	m_pChoiceOptValue;
};

#endif // !defined(AFX_CHOICEOPTIONEXP_H__4BEFF245_A6E4_4933_935B_66A8E57936F0__INCLUDED_)
