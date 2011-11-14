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
	
	return pMonitor->Monitor();
}

CThreadMonitor::CThreadMonitor() : m_bAllowed(FALSE)
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

	HANDLE hMonitor = ::CreateThread(NULL, 0,  CThreadMonitor::MonitorThreadProc, this, 0, &dwThreadId);

	::CloseHandle(hMonitor);
}

void CThreadMonitor::EndMonitor()
{
	::SetEvent(m_hQuitEvent);
}

void CThreadMonitor::AddThread(HANDLE handle, CPostAction* pPostAction)
{
	m_criticalSection.Lock();
	CPostAction* pTemp;
	ASSERT(m_mapAction.Lookup(handle, pTemp) == FALSE);
	m_mapAction.SetAt(handle, pPostAction);
	if(m_bAllowed)
	{
		::SetEvent(m_hChangeEvent);
	}
	m_criticalSection.Unlock();	
}

DWORD CThreadMonitor::Monitor()
{
	int nCount = 0;
	
	m_criticalSection.Lock();
	m_bAllowed = TRUE;
	m_criticalSection.Unlock();

	RefreshWaitObjects(&nCount);

	int nSingaledCount = 0;
	int pSingaledIndex[MAX_WAIT_COUNT];

	int i, nStartIndex;
	CPostAction* pPostAction = NULL;
	BOOL bResult = FALSE;

	DWORD dwRet = 0;
	while(TRUE)
	{
		ASSERT(nCount >= 1);

		if(nCount == 1)
		{
			ASSERT(m_bAllowed == FALSE);
			ASSERT(m_hWaitObjects[0] == m_hChangeEvent);
			AfxTrace("Monitor Thread Exit 1\n");
			return 1;
		}

		dwRet = WaitForMultipleObjects(nCount, m_hWaitObjects, FALSE, INFINITE);

		//Something happens
		if(dwRet >= WAIT_OBJECT_0 && dwRet < (WAIT_OBJECT_0 + nCount))
		{
			GetSignaledObjects(dwRet, nCount, pSingaledIndex, &nSingaledCount);
			if(nSingaledCount > 0 && nSingaledCount <= nCount)
			{

			}
			else
			{
				AfxTrace("%d\n", nSingaledCount);
				ASSERT(nSingaledCount > 0 && nSingaledCount <= nCount);
			}

			nStartIndex = FindFirstMonitee(pSingaledIndex, nSingaledCount);

			if(nStartIndex >= 0)
			{
				for(i = nStartIndex; i < nSingaledCount; i++)
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
			else
			{
				ASSERT(nSingaledCount <= MAX_CONTROL_EVENT_COUNT);
				nStartIndex = nSingaledCount;
			}

			//Control event check
			for(i = 0; i < nStartIndex; i++)
			{
				ASSERT(pSingaledIndex[i] >=0 && pSingaledIndex[i] < MAX_CONTROL_EVENT_COUNT);
				
				//Quit event
				if(pSingaledIndex[i] == INDEX_OF_QUIT_EVENT)
				{
					m_criticalSection.Lock();
					ASSERT(m_bAllowed);
					m_bAllowed = FALSE;

					if(m_mapAction.GetCount() <= 0)
					{
						AfxTrace("Monitor Thread Exit 2\n");
						//TODO: quit
						m_criticalSection.Unlock();
						return 2;
					}
					else
					{
						//TODO: Wait all the left threads to quit. Remove Quit event from wait objects
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

			RefreshWaitObjects(&nCount);
			AfxTrace("Wait Count = %d\n", nCount);
		}
		else
		{
			AfxTrace("Error happens. dwRet = %d\n", dwRet);
		}
	}

	return 0;
}

BOOL CThreadMonitor::RefreshWaitObjects(int* pCount)
{
	ASSERT(pCount != NULL);

	int nCount = 0;

	HANDLE handle;
	CPostAction* pAction;
	
	m_criticalSection.Lock();

	if(m_bAllowed)
	{
		m_hWaitObjects[nCount++] = m_hQuitEvent;
	}
	m_hWaitObjects[nCount++] = m_hChangeEvent;


	POSITION pos = m_mapAction.GetStartPosition();
	while (pos != NULL)
	{
		m_mapAction.GetNextAssoc(pos, handle, pAction);

		m_hWaitObjects[nCount++] = handle;
	}
	*pCount = nCount;
	
	m_criticalSection.Unlock();

	return TRUE;
}

BOOL CThreadMonitor::GetSignaledObjects(DWORD dwRet, int nCount, int* pSignaledIndex, int* pSignaledCount)
{
	ASSERT(pSignaledIndex != NULL && pSignaledCount != NULL);
	ASSERT(dwRet >= WAIT_OBJECT_0 && dwRet < (WAIT_OBJECT_0 + nCount));
	
	int nSignaledCount = 0;
	int nIndex = (dwRet - WAIT_OBJECT_0);
	pSignaledIndex[nSignaledCount++] = nIndex++;


	while(nIndex < nCount) //nCount
	{
		dwRet = WaitForMultipleObjects(nCount - nIndex, &m_hWaitObjects[nIndex], FALSE, 0);

		//Something happens
		if(dwRet >= WAIT_OBJECT_0 && dwRet < (WAIT_OBJECT_0 + nCount - nIndex))
		{
			nIndex = nIndex + dwRet - WAIT_OBJECT_0;
			pSignaledIndex[nSignaledCount++] = nIndex++;
		}
		//no other signaled handles
		else if(dwRet == WAIT_TIMEOUT)
		{
			nIndex = nCount;
		}
		else
		{
			AfxTrace("GetSignaledObjects::Error happens. dwRet = %d\n", dwRet);
		}
	}

	ASSERT(nSignaledCount > 0 && nSignaledCount <= nCount);

	*pSignaledCount = nSignaledCount;

	return TRUE;
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