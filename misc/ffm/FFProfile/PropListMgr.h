// PropListMgr.h: interface for the CPropListMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPLISTMGR_H__1199350F_F024_46E0_B26F_05116A262F4F__INCLUDED_)
#define AFX_PROPLISTMGR_H__1199350F_F024_46E0_B26F_05116A262F4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tinyxml.h"

class CPropListMgr  
{
public:
	CPropListMgr(CBCGPPropList* pPropList);
	virtual ~CPropListMgr();

	BOOL Init(LPCTSTR lpszFileName);
	BOOL InitProp(CBCGPProp* pParentProp, TiXmlElement *pParentXml);

	CBCGPPropList* m_pPropList;
};

#endif // !defined(AFX_PROPLISTMGR_H__1199350F_F024_46E0_B26F_05116A262F4F__INCLUDED_)
