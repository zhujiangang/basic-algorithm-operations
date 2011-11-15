// ThreadMonitor.cpp: implementation of the CThreadMonitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadMonitor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define INDEX_OF_QUIT_EVENT			0
#define INDEX_OF_CHANGE_EVENT		1


DWORD WINAPI CThreadMonitor::MonitorThreadProc(LPVOID lpParameter)
{
	CThreadMonitor* pMonitor = (CThreadMonitor*)lpParameter;
	
	return pMonitor->DoMonitor();
}

CThreadMonitor::CThreadMonitor() : m_bMonitoring(FALSE), m_hMonitor(NULL)
{
	//AutoReset event, no signal
	m_hQuitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	//AutoReset event, no signal
	m_hAddEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	m_hRemoveEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	ZeroMemory(m_hWaitObjects, sizeof(m_hWaitObjects));
}

CThreadMonitor::~CThreadMonitor()
{
	if(IsRunning())
	{
		StopMonitor(TRUE);
	}

	if(m_hRemoveEvent)
	{
		::CloseHandle(m_hRemoveEvent);
		m_hRemoveEvent = NULL;
	}
	if(m_hAddEvent)
	{
		::CloseHandle(m_hAddEvent);
		m_hAddEvent = NULL;
	}
	if(m_hQuitEvent)
	{
		::CloseHandle(m_hQuitEvent);
		m_hQuitEvent = NULL;
	}

	//Let the other thread to delete the left post-action
	m_mapAction.RemoveAll();
}

void CThreadMonitor::StartMonitor()
{
	DWORD dwThreadId = 0;

	m_bMonitoring = TRUE;
	m_hMonitor = ::CreateThread(NULL, 0,  CThreadMonitor::MonitorThreadProc, this, 0, &dwThreadId);
}

void CThreadMonitor::StopMonitor(BOOL bWait, DWORD dwTimeOut)
{
	::SetEvent(m_hQuitEvent);

	if(m_hMonitor)
	{
		//Should we wait?
		if(bWait)
		{
			DWORD dwResult = ::WaitForSingleObject(m_hMonitor, dwTimeOut);
			
			//abnormal case, here try to terminate thread
			if(dwResult == WAIT_ABANDONED || dwResult == WAIT_TIMEOUT)
			{
				BOOL bResult = TerminateThread(m_hMonitor, RC_EXIT_TERMINATED);
				AfxTrace("TerminateThread result = %d, dwResult = %d\n", bResult, dwResult);
			}
			//There's nothing to do with WAIT_OBJECT_0 and WAIT_FAILED
		}

		//Finally close the monitor thread
		::CloseHandle(m_hMonitor);
		m_hMonitor = NULL;
	}
}

BOOL CThreadMonitor::IsRunning()
{
	if(m_hMonitor)
	{
		BOOL bResult;
		DWORD dwExitCode;

		bResult = GetExitCodeThread(m_hMonitor, &dwExitCode);
		if(bResult && (dwExitCode == STILL_ACTIVE))
		{
			return TRUE;
		}
	}

	return FALSE;
}

DWORD CThreadMonitor::AddMonitee(HANDLE handle, CPostAction* pPostAction)
{
	DWORD dwResult = RC_ADD_OK;

	m_criticalSection.Lock();
	if(m_bMonitoring)
	{
		CPostAction* pTemp;
		if(m_mapAction.Lookup(handle, pTemp))
		{
			dwResult = RC_ADD_EXIST;
		}
		else
		{
			m_mapAction.SetAt(handle, pPostAction);
			::SetEvent(m_hAddEvent);
		}
	}
	else
	{
		dwResult = RC_ADD_NOT_MONITORING;
	}
	m_criticalSection.Unlock();

	return dwResult;
}

DWORD CThreadMonitor::AddMoniteeWaitForExist(HANDLE handle, CPostAction* pPostAction)
{
	DWORD dwResult;
	
	do 
	{
		dwResult = AddMonitee(handle, pPostAction);
		if(dwResult != RC_ADD_EXIST)
		{
			break;
		}
		::WaitForSingleObject(m_hRemoveEvent, INFINITE);
	} while (TRUE);

	return dwResult;
}

DWORD CThreadMonitor::DoMonitor()
{
	DWORD dwResult = 0, dwWaitResult = 0;

	int nCount = 0;
	RefreshWaitObjects(&nCount);
	ASSERT(nCount > 0);

	while(TRUE)
	{
		AfxTrace("WaitForMultipleObjects count = %d\n", nCount);
		if(nCount == 0)
		{
			ASSERT(m_bMonitoring == FALSE);

			dwResult = RC_EXIT_ALL_MONITEE_EXITED;
			AfxTrace("Stopped monitorring. Result code=%d\n", dwResult);

			return dwResult;
		}

		dwWaitResult = WaitForMultipleObjects(nCount, m_hWaitObjects, FALSE, INFINITE);

		//Objects signaled
		if(dwWaitResult >= WAIT_OBJECT_0 && dwWaitResult < (WAIT_OBJECT_0 + nCount))
		{
			dwResult = ProcessSignaled(dwWaitResult, nCount);
			if(dwResult != 0)
			{
				AfxTrace("Stopped monitorring. Result code=%d\n", dwResult);
				return dwResult;
			}

			RefreshWaitObjects(&nCount);
		}
		else
		{
			dwResult = ProcessAbnormal(nCount, dwWaitResult);
			if(dwResult != 0)
			{
				return dwResult;
			}
		}
	}

	return 0;
}

DWORD CThreadMonitor::ProcessSignaled(DWORD dwRet, int nCount)
{
	ASSERT(dwRet >= WAIT_OBJECT_0 && dwRet < (WAIT_OBJECT_0 + nCount));

	int nSingaledCount = 0;
	int pSingaledIndex[MAX_WAIT_COUNT];
	
	int nFirstMonitee, nControlEventEnd;	

	GetSignaledObjects(dwRet, nCount, pSingaledIndex, &nSingaledCount);
	ASSERT(nSingaledCount > 0 && nSingaledCount <= nCount);
	//Debug Code
	CString szLog, szTemp;
	szLog.Format("[Signaled Objects]: count=%d, ", nSingaledCount);
	for(int i = 0; i < nSingaledCount; i++)
	{
		szTemp.Format("%d ", pSingaledIndex[i]);
		szLog += szTemp;
	}
	AfxTrace("%s\n", szLog);
	
	nFirstMonitee = FindFirstMonitee(pSingaledIndex, nSingaledCount);
	if(nFirstMonitee >= 0)
	{
		ProcessSignaledMonitees(pSingaledIndex, nSingaledCount, nFirstMonitee);
		nControlEventEnd = nFirstMonitee;
	}
	else
	{
		ASSERT(nSingaledCount <= MAX_CONTROL_EVENT_COUNT);
		nControlEventEnd = nSingaledCount;
	}
	
	DWORD dwResult = ProcessSignaledControlEvents(pSingaledIndex, nControlEventEnd);
	
	return dwResult;
}

void CThreadMonitor::ProcessSignaledMonitees(int pSingaledIndex[], int nSingaledCount, int nFirstMonitee)
{
	ASSERT(nFirstMonitee >= 0 && nFirstMonitee < nSingaledCount);

	CPostAction* pPostAction = NULL;
	BOOL bResult;
	int i;
	
	for(i = nFirstMonitee; i < nSingaledCount; i++)
	{
		bResult = GetPostAction(m_hWaitObjects[pSingaledIndex[i]], &pPostAction);
		ASSERT(bResult);
		
		if(pPostAction->DoAction() == CPostAction::DELETE_BY_EXTERNAL)
		{
			delete pPostAction;
		}
		
		RemovePostAction(m_hWaitObjects[pSingaledIndex[i]]);
	}
}

DWORD CThreadMonitor::ProcessSignaledControlEvents(int pSingaledIndex[], int nControlEventEnd)
{
	//Control event check
	for(int i = 0; i < nControlEventEnd; i++)
	{
		ASSERT(pSingaledIndex[i] >= 0 && pSingaledIndex[i] < MAX_CONTROL_EVENT_COUNT);
		
		//Quit event
		if(pSingaledIndex[i] == INDEX_OF_QUIT_EVENT)
		{
			m_criticalSection.Lock();

			ASSERT(m_bMonitoring);
			m_bMonitoring = FALSE;
			if(m_mapAction.GetCount() <= 0)
			{
				//No monitored threads now, and we received Quit command
				m_criticalSection.Unlock();

				return RC_EXIT_NO_MONITEE;
			}
			else
			{
				//Wait all the left threads to quit. Remove Quit event from wait objects
				//Nothing need to do here
			}

			m_criticalSection.Unlock();
		}
		//Change event
		else if(pSingaledIndex[i] == INDEX_OF_CHANGE_EVENT)
		{
			//nothing here, this event is used to refresh wait objects...
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

void CThreadMonitor::RefreshWaitObjects(int* pCount)
{
	ASSERT(pCount != NULL);

	int nCount = 0;
	HANDLE handle;
	CPostAction* pAction;
	
	m_criticalSection.Lock();

	//Only wait for the 2 control events when it's monitoring.
	if(m_bMonitoring)
	{
		m_hWaitObjects[nCount++] = m_hQuitEvent;
		m_hWaitObjects[nCount++] = m_hAddEvent;
	}

	POSITION pos = m_mapAction.GetStartPosition();
	while (pos != NULL)
	{
		m_mapAction.GetNextAssoc(pos, handle, pAction);
		m_hWaitObjects[nCount++] = handle;
	}

	*pCount = nCount;
	
	m_criticalSection.Unlock();
}

void CThreadMonitor::GetSignaledObjects(DWORD dwRet, int nCount, int lpSignaledIndex[], int* lpSignaledCount)
{
	ASSERT(lpSignaledIndex != NULL && lpSignaledCount != NULL);
	ASSERT(dwRet >= WAIT_OBJECT_0 && dwRet < (WAIT_OBJECT_0 + nCount));
	
	int nSignaledCount = 0;
	int nIndex = (dwRet - WAIT_OBJECT_0);
	lpSignaledIndex[nSignaledCount++] = nIndex++;

	//Check if there are more than 1 handles become signaled
	while(nIndex < nCount) //nCount
	{
		dwRet = WaitForMultipleObjects(nCount - nIndex, &m_hWaitObjects[nIndex], FALSE, 0);

		//Something happens
		if(dwRet >= WAIT_OBJECT_0 && dwRet < (WAIT_OBJECT_0 + nCount - nIndex))
		{
			nIndex = nIndex + dwRet - WAIT_OBJECT_0;
			lpSignaledIndex[nSignaledCount++] = nIndex++;
		}
		//no other signaled handles
		else if(dwRet == WAIT_TIMEOUT)
		{
			nIndex = nCount;
			break;
		}
		else
		{
			ProcessAbnormal(nCount - nIndex, dwRet);
		}
	}

	ASSERT(nSignaledCount > 0 && nSignaledCount <= nCount);
	*lpSignaledCount = nSignaledCount;
}


BOOL CThreadMonitor::GetPostAction(HANDLE handle, CPostAction** ppPostAction)
{
	ASSERT(ppPostAction != NULL);

	BOOL bResult;

	m_criticalSection.Lock();
	bResult = m_mapAction.Lookup(handle, *ppPostAction);
	m_criticalSection.Unlock();

	return bResult;
}
BOOL CThreadMonitor::RemovePostAction(HANDLE handle)
{
	BOOL bResult;

	m_criticalSection.Lock();
	bResult = m_mapAction.RemoveKey(handle);
	ASSERT(bResult);

	if(bResult)
	{
		::SetEvent(m_hRemoveEvent);
	}
	m_criticalSection.Unlock();
	
	return bResult;
}

int  CThreadMonitor::FindFirstMonitee(int array[], int nLength)
{
	HANDLE handle;
	for(int i = 0; i < nLength; i++)
	{
		handle = m_hWaitObjects[array[i]];
		if(handle != m_hQuitEvent && handle != m_hAddEvent)
		{
			return i;
		}
	}
	return -1;
}

DWORD CThreadMonitor::ProcessAbnormal(int nCount, DWORD dwResult)
{	
	CString szErrorMsg;
	if(dwResult >= WAIT_ABANDONED_0 && dwResult < (WAIT_ABANDONED_0 + nCount))
	{
		szErrorMsg.Format("WAIT_ABANDONED, index=%d", dwResult - WAIT_ABANDONED_0);
	}
	else if(dwResult == WAIT_TIMEOUT)
	{
		szErrorMsg.Format("WAIT_TIMEOUT");
	}
	else if(dwResult == WAIT_FAILED)
	{
		szErrorMsg.Format("WAIT_FAILED, Last Error=%d", GetLastError());
	}
	else
	{
		szErrorMsg.Format("Unkown result (%d)", dwResult);
	}

	AfxTrace("WaitForMultipleObjects return abnormal. Details: %s\n", szErrorMsg);

	return 0;
}