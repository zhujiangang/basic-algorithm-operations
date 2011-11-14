// TestAction.cpp: implementation of the CTestAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestAction.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTestAction::CTestAction(HANDLE handle, LPCTSTR lpszName) : m_hHandle(handle), m_szName(lpszName)
{
}

CTestAction::~CTestAction()
{

}

int CTestAction::DoAction()
{
	CString szLog;
	szLog.Format("The handle is closed now. handle=%08X, name=%s", m_hHandle, m_szName);
	
	AfxTrace("%s\n", szLog);

	if(m_hHandle)
	{
		if(!::CloseHandle(m_hHandle))
		{
			AfxTrace("Failed to Close Handle. %d\n", GetLastError());
		}
	}
	

	return -1;
}
