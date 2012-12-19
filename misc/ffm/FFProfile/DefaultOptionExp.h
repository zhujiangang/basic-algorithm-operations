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

enum OptEvaluateMode
{
	OPTEM_DEFAULT = 0,
	OPTEM_NAME_ONLY,
	OPTEM_VALUE_ONLY,
	OPTEM_SET_CONTEXT,
	OPTEM_SET_PARENT
};

enum OptEvaluateFlag
{
	OPTEF_MUST		= (1 << 0),
	OPTEF_CHILDREN	= (1 << 1),
	OPTEF_CONTEXT	= (1 << 2),
	OPTEF_SELF		= (1 << 3),
	OPTEF_NONE		= (1 << 4),		/* don't evaluate (always ok), used for set context value */
	OPTEF_KEEP_EMPTY= (1 << 5)
};

#define OPT_CONFIGURABLE	(OPTEF_CONTEXT | OPTEF_SELF)

enum OptFieldEnum
{
	OPT_FIELD_ID = 0,
	OPT_FIELD_NAME,
	OPT_FIELD_VALUE,
	OPT_FIELD_NAME_VALUE_SEP,
	OPT_FIELD_SUB_OPTION_SEP,
	CONTEXT_VALUE
};

#define MAKE_OPT_MODE(ACTION, FROM, TO)		(((TO) << 16) | ((FROM) << 8) | (ACTION))
#define GET_OPT_MODE_ACTION(MODE)			((MODE) & 0xFF)
#define GET_OPT_MODE_FROM(MODE)				(((MODE) >> 8) & 0xFF)
#define GET_OPT_MODE_TO(MODE)				(((MODE) >> 16) & 0xFF)

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
