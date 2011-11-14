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

#define RC_EXIT_NO_MONITEE			1
#define RC_EXIT_ALL_MONITEE_EXITED	2

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
	m_hChangeEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CThreadMonitor::~CThreadMonitor()
{
	if(m_hChangeEvent)
	{
		::CloseHandle(m_hChangeEvent);
	}
	if(m_hQuitEvent)
	{
		::CloseHandle(m_hQuitEvent);
	}
}

void CThreadMonitor::StartMonitor()
{
	DWORD dwThreadId = 0;

	m_bMonitoring = TRUE;
	m_hMonitor = ::CreateThread(NULL, 0,  CThreadMonitor::MonitorThreadProc, this, 0, &dwThreadId);
}

void CThreadMonitor::StopMonitor(BOOL bWaitUtilEnd)
{
	::SetEvent(m_hQuitEvent);

	if(bWaitUtilEnd)
	{
		::WaitForSingleObject(m_hMonitor, INFINITE);
	}

	if(m_hMonitor)
	{
		::CloseHandle(m_hMonitor);
		m_hMonitor = NULL;
	}
}

BOOL CThreadMonitor::AddMonitee(HANDLE handle, CPostAction* pPostAction)
{
	BOOL bResult = FALSE;

	m_criticalSection.Lock();

	if(m_bMonitoring)
	{
		CPostAction* pTemp;
		ASSERT(m_mapAction.Lookup(handle, pTemp) == FALSE);

		m_mapAction.SetAt(handle, pPostAction);

		::SetEvent(m_hChangeEvent);
	}
	bResult = m_bMonitoring;

	m_criticalSection.Unlock();

	return bResult;
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
		
		if(pPostAction->DoAction() < 0)
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
		m_hWaitObjects[nCount++] = m_hChangeEvent;
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
	m_criticalSection.Unlock();
	
	return bResult;
}

int  CThreadMonitor::FindFirstMonitee(int array[], int nLength)
{
	HANDLE handle;
	for(int i = 0; i < nLength; i++)
	{
		handle = m_hWaitObjects[array[i]];
		if(handle != m_hQuitEvent && handle != m_hChangeEvent)
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