// SimpleThreadPool1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SimpleThreadPool1.h"
#include "SimpleThreadPool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

DWORD BeginTime;
LONG  ItemCount;
HANDLE CompleteEvent;

int compute()
{
	srand(BeginTime);
	
	for(int i=0; i<20 *1000 * 1000; i++)
		rand();
	
	return rand();
}

DWORD WINAPI WorkItem(LPVOID lpParameter)
{
	static int count = 0;
	count++;
	BOOL bWaitMode = (BOOL)lpParameter;
	
	CString szLog;
	if(bWaitMode)
	{
		szLog.Format("[%02d]: Thread %06d is sleep 1s", count, GetCurrentThreadId());
		printf("%s\n", (LPCTSTR)szLog);

		Sleep(1000);
	}	
	else
	{
		szLog.Format("[%02d]: Thread %06d is computing.", count, GetCurrentThreadId());
		printf("%s\n", (LPCTSTR)szLog);

		compute();
	}
	
	if(InterlockedDecrement(&ItemCount) == 0)
	{
		printf("Time total %d second.\n", GetTickCount() - BeginTime);
		SetEvent(CompleteEvent);
	}
	
	return 0;
}

VOID UserProc1(PVOID dwParam)
{
	WorkItem(dwParam);
}
void TestSimpleThreadPool(BOOL bWaitMode, LONG ThreadNum)
{
	CSimpleThreadPool ThreadPool;    
	ThreadPool.Init(ThreadNum);
	
	CompleteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	BeginTime = GetTickCount();
	int nWorkCount = 20;
	ItemCount = nWorkCount;
	
	for(int i=0; i<nWorkCount; i++)
	{
		ThreadPool.PostWorkItem(UserProc1, (PVOID)bWaitMode);
	}
	
	WaitForSingleObject(CompleteEvent, INFINITE);
	CloseHandle(CompleteEvent);
	
	ThreadPool.DestroyThreadPool();
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		CString strHello;
		strHello.LoadString(IDS_HELLO);
		cout << (LPCTSTR)strHello << endl;
	}

	TestSimpleThreadPool(TRUE, 5);

	return nRetCode;
}


