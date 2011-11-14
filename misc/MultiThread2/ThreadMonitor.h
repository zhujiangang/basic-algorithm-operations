// ThreadMonitor.h: interface for the CThreadMonitor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADMONITOR_H__550C6B33_20AA_484D_BB67_1F7D0C57B98F__INCLUDED_)
#define AFX_THREADMONITOR_H__550C6B33_20AA_484D_BB67_1F7D0C57B98F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPostAction
{
public:
	virtual ~CPostAction() {}
	virtual int DoAction() = 0;
};

typedef CMap<HANDLE, HANDLE, CPostAction*, CPostAction*> CMapHandle2Action;

class CThreadMonitor  
{
public:
	CThreadMonitor();
	virtual ~CThreadMonitor();

	void StartMonitor();
	void StopMonitor(BOOL bWaitUtilEnd = FALSE);
	BOOL AddMonitee(HANDLE handle, CPostAction* pPostAction);
private:
	enum
	{
		MAX_CONTROL_EVENT_COUNT = 2,
		MAX_MONITEE_COUNT = 64,
		MAX_WAIT_COUNT = MAX_MONITEE_COUNT + MAX_CONTROL_EVENT_COUNT
	};
	static DWORD WINAPI MonitorThreadProc(LPVOID lpParameter);
	DWORD DoMonitor();
	DWORD ProcessSignaled(DWORD dwRet, int nCount);
	void  ProcessSignaledMonitees(int pSingaledIndex[], int nSingaledCount, int nFirstMonitee);
	DWORD ProcessSignaledControlEvents(int pSingaledIndex[], int nControlEventEnd);
	void RefreshWaitObjects(int* pCount);
	void GetSignaledObjects(DWORD dwRet, int nCount, int pSignaledIndex[], int* pSignaledCount);
	BOOL GetPostAction(HANDLE handle, CPostAction** pPostAction);
	BOOL RemovePostAction(HANDLE handle);
	int  FindFirstMonitee(int array[], int nLength);
	DWORD ProcessAbnormal(int nCount, DWORD dwResult);
private:
	HANDLE m_hQuitEvent;
	HANDLE m_hChangeEvent;
	HANDLE m_hWaitObjects[MAX_WAIT_COUNT];

	HANDLE m_hMonitor;

	BOOL m_bMonitoring;

	CMapHandle2Action m_mapAction;
	CCriticalSection m_criticalSection;
};

#endif // !defined(AFX_THREADMONITOR_H__550C6B33_20AA_484D_BB67_1F7D0C57B98F__INCLUDED_)
