#if(_WIN32_WINNT < 0x0500)
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0510
#endif

#include <windows.h>
#include <stdio.h>
#include "multi_thread.h"


static DWORD BeginTime;
static LONG  ItemCount;
static HANDLE CompleteEvent;

int compute();
DWORD WINAPI WorkItem(LPVOID lpParameter);
void TestWorkItem(BOOL bWaitMode, DWORD Flag);

void test_sys_thread_pool()
{
	TestWorkItem(TRUE, WT_EXECUTEDEFAULT);
}


//Dependent functions
int compute()
{
	srand(BeginTime);
	
	for(int i=0; i<20 *1000 * 1000; i++)
		rand();
	
	return rand();
}

DWORD WINAPI WorkItem(LPVOID lpParameter)
{
	BOOL bWaitMode = (BOOL)lpParameter;
	
	if(bWaitMode)
	{
		printf("Thread work [%06d]: sleep 1s\n", GetCurrentThreadId());
		Sleep(1000);
	}
	else
		compute();
	
	if(InterlockedDecrement(&ItemCount) == 0)
	{
		printf("Thread work [%06d]: finished all works in %d seconds\n", GetCurrentThreadId(), GetTickCount() - BeginTime);
		SetEvent(CompleteEvent);
	}
	
	return 0;
}

void TestWorkItem(BOOL bWaitMode, DWORD Flag)
{
	printf("Thread main [%06d]: start to post works\n", GetCurrentThreadId());
	CompleteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	BeginTime = GetTickCount();
	int nCount = 20;
	ItemCount = nCount;
	
	for(int i=0; i<nCount; i++)
	{
		QueueUserWorkItem(WorkItem, (PVOID)bWaitMode, Flag);
	}
	printf("Thread main [%06d]: end   to post works\n", GetCurrentThreadId());
	
	WaitForSingleObject(CompleteEvent, INFINITE);
	CloseHandle(CompleteEvent);
	
	printf("Thread main [%06d]: end now\n", GetCurrentThreadId());
}
