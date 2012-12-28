// OptionExpTree.h: interface for the OptionExpTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONEXPTREE_H__E420FAAD_5CBE_4CE4_AA4C_BFE11A5F6BB1__INCLUDED_)
#define AFX_OPTIONEXPTREE_H__E420FAAD_5CBE_4CE4_AA4C_BFE11A5F6BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OptionExp.h"

class OptionExpTree  
{
public:
	OptionExpTree();
	virtual ~OptionExpTree();

	virtual bool Build(OptionContext* pContext, OptionExpBuilder* pBuilder);
	virtual void Destroy();	
	virtual bool Evaluate(OptionContext* pContext, std::string& szResult);

private:
	bool PostEvaluateGroup(OptionContext* pContext);

private:
	OptionExp*	m_pRoot;
};

#endif // !defined(AFX_OPTIONEXPTREE_H__E420FAAD_5CBE_4CE4_AA4C_BFE11A5F6BB1__INCLUDED_)
