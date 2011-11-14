// TestAction.h: interface for the CTestAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTACTION_H__CD4D422C_2CD5_45C9_8B77_9731B1FD0E4A__INCLUDED_)
#define AFX_TESTACTION_H__CD4D422C_2CD5_45C9_8B77_9731B1FD0E4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadMonitor.h"

class CTestAction : public CPostAction
{
public:
	CTestAction(HANDLE handle, LPCTSTR lpszName);
	virtual ~CTestAction();
	virtual int DoAction();
private:
	HANDLE m_hHandle;
	CString m_szName;
};

#endif // !defined(AFX_TESTACTION_H__CD4D422C_2CD5_45C9_8B77_9731B1FD0E4A__INCLUDED_)
