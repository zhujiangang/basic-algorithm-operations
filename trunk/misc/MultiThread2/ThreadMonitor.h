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
	enum
	{
		DELETE_BY_EXTERNAL = 0x80000000
	};
	virtual ~CPostAction() {}
	virtual int DoAction() = 0;
};

typedef CMap<HANDLE, HANDLE, CPostAction*, CPostAction*> CMapHandle2Action;

/*Best case. When ThreadMonitor receive quit command, there's no 
available handles in the waiting queue, quit right away.*/
#define RC_EXIT_NO_MONITEE			1

/*When ThreadMonitor receive quit command, there's still 
available handles in the waiting queue. ThreadMonitor wait for
all the threads to end, then exit*/
#define RC_EXIT_ALL_MONITEE_EXITED	2

/*StopMonitor met WAIT_TIMEOUT or WAIT_ABANDONED cases, ThreadMonitor thread was
terminated by force*/
#define RC_EXIT_TERMINATED			3


#define RC_ADD_OK					0
#define RC_ADD_EXIST				1
#define RC_ADD_NOT_MONITORING		2

class CThreadMonitor  
{
public:
	CThreadMonitor();
	virtual ~CThreadMonitor();

	//Start monitor thread
	void StartMonitor();

	//Stop monitor thread
	void StopMonitor(BOOL bWait = FALSE, DWORD dwTimeOut = INFINITE);

	//Add handle to monitor
	DWORD AddMonitee(HANDLE handle, CPostAction* pPostAction);

	//Special case: when the same handle already exists in the monitor thread,
	//this function will wait until the previous handle was removed
	DWORD AddMoniteeWaitForExist(HANDLE handle, CPostAction* pPostAction);

	//Check if the monitor thread is running at now
	BOOL IsRunning();
private:
	enum
	{
		MAX_CONTROL_EVENT_COUNT = 2,
		MAX_MONITEE_COUNT = 64,
		MAX_WAIT_COUNT = MAX_MONITEE_COUNT + MAX_CONTROL_EVENT_COUNT
	};
	//Monitor thread proc
	static DWORD WINAPI MonitorThreadProc(LPVOID lpParameter);

	//Actual monitor function
	DWORD DoMonitor();
	
	//Refresh the current wait objects
	void RefreshWaitObjects(int* pCount);

	//Process when there are waited objects become signaled
	DWORD ProcessSignaled(DWORD dwRet, int nCount);

	//Process when wait function return abnormal value
	DWORD ProcessAbnormal(int nCount, DWORD dwResult);

	//for signaled monitees
	void  ProcessSignaledMonitees(int pSingaledIndex[], int nSingaledCount, int nFirstMonitee);

	//for control events
	DWORD ProcessSignaledControlEvents(int pSingaledIndex[], int nControlEventEnd);
	
	//When there are multiple objects become signaled, find all the signaled objects
	void GetSignaledObjects(DWORD dwRet, int nCount, int pSignaledIndex[], int* pSignaledCount);

	//Find the first monitee in the array
	int  FindFirstMonitee(int array[], int nLength);

	//Get related post-action for the handle, thread-safe
	BOOL GetPostAction(HANDLE handle, CPostAction** pPostAction);
	//Remove related post-action for the handle, thread-safe
	BOOL RemovePostAction(HANDLE handle);

private:
	HANDLE m_hQuitEvent;
	HANDLE m_hAddEvent;
	HANDLE m_hWaitObjects[MAX_WAIT_COUNT];

	HANDLE m_hRemoveEvent;

	//Is still accept new handle to monitor
	BOOL m_bMonitoring;

	//Monitor thread handle
	HANDLE m_hMonitor;

	CMapHandle2Action m_mapAction;
	CCriticalSection m_criticalSection;
};

#endif // !defined(AFX_THREADMONITOR_H__550C6B33_20AA_484D_BB67_1F7D0C57B98F__INCLUDED_)
