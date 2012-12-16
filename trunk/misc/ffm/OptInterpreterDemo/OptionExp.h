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

enum OptEvaluateMode
{
	OPTEM_DEFAULT = 0,
	OPTEM_NAME_ONLY,
	OPTEM_VALUE_ONLY,
// 	OPTEM_SET_NAME,
// 	OPTEM_SET_VALUE,
// 	OPTEM_SET_PARENT_NAME,
	OPTEM_SET_CONTEXT,
	OPTEM_SET_PARENT
};

enum OptEvaluateFlag
{
	OPTEF_MUST		= (1 << 0),
	OPTEF_CHILDREN	= (1 << 1),
	OPTEF_CONTEXT	= (1 << 2),
	OPTEF_SELF		= (1 << 3),
	OPTEF_NONE		= (1 << 4)		/* don't evaluate (always ok), used for set context value */
};

#define OPT_HAS_CHILDREN	(OPTEF_CHILDREN | OPTEF_CONTEXT | OPTEF_SELF)
#define OPT_CONFIGURABLE	(OPTEF_CONTEXT | OPTEF_SELF)

enum OptFieldEnum
{
	OPT_FIELD_ID = 0,
	OPT_FIELD_NAME,
	OPT_FIELD_VALUE,
	OPT_FIELD_NAME_VALUE_SEP,
	OPT_FIELD_SUB_OPTION_SEP,
	OPT_FIELD_CHOICE_OPT_ID,

	CONTEXT_VALUE
};

#define MAKE_OPT_MODE(ACTION, FROM, TO)		(((TO) << 16) | ((FROM) << 8) | (ACTION))
#define GET_OPT_MODE_ACTION(MODE)			((MODE) & 0xFF)
#define GET_OPT_MODE_FROM(MODE)				(((MODE) >> 8) & 0xFF)
#define GET_OPT_MODE_TO(MODE)				(((MODE) >> 16) & 0xFF)


/*
#define OPT_FIELD_ID		"OptFieldID"
#define OPT_FIELD_NAME		"OptFieldName"
#define OPT_FIELD_VALUE		"OptFieldValue"
#define OPT_FIELD_NAME_VALUE_SEP	"OptFieldNameValueSep"
#define OPT_FIELD_SUB_OPTION_SEP	"OptFieldSubOptionSep"
#define OPT_FIELD_CHOICE_OPT_ID		"OptFieldChoiceOptID"
*/

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
	virtual void RemoveChildren(bool bFree = false);

	/*
	virtual bool ContainsKey(const char* key);
	virtual OptionExp& SetField(const char* key, const char* val);
	virtual std::string* GetField(const char* key);
	virtual const char* GetFieldStr(const char* key);
	*/

	virtual bool ContainsKey(int nFieldID);
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

#endif // !defined(AFX_OPTIONEXP_H__90B7E96C_983F_4607_B818_945BF6137CBC__INCLUDED_)
