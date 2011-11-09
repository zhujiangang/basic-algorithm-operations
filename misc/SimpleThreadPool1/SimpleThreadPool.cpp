// SimpleThreadPool.cpp: implementation of the CSimpleThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleThreadPool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD WINAPI WorkerThread(PVOID pParam)
{
	CSimpleThreadPool* pThreadPool = (CSimpleThreadPool*)pParam;
	HANDLE Events[2];
	
	Events[0] = pThreadPool->QuitEvent;
	Events[1] = pThreadPool->WorkItemSemaphore;
	
	for(;;)
	{
		DWORD dwRet = WaitForMultipleObjects(2, Events, FALSE, INFINITE);
		
		if(dwRet == WAIT_OBJECT_0)
			break;
		
		//
		// execute user's proc.
		//
		
		else if(dwRet == WAIT_OBJECT_0 +1)
		{
			PWORK_ITEM pWorkItem;
			
			EnterCriticalSection(&pThreadPool->WorkItemLock);
			ASSERT(!(pThreadPool->WorkItemList.IsEmpty()));
			pWorkItem = pThreadPool->WorkItemList.RemoveHead();
			LeaveCriticalSection(&pThreadPool->WorkItemLock);
			
			pWorkItem->UserProc(pWorkItem->UserParam);
			
			InterlockedDecrement(&pThreadPool->WorkItemCount);
			free(pWorkItem);
		}
		
		else
		{
			ASSERT(0);
			break;
		}
	}
	
	return 0;
}

CSimpleThreadPool::CSimpleThreadPool()
{

}

CSimpleThreadPool::~CSimpleThreadPool()
{

}

void CSimpleThreadPool::Init(int nThreadCount)
{
	QuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	WorkItemSemaphore = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
	WorkItemCount = 0;


	InitializeCriticalSection(&WorkItemLock);
	ThreadNum = nThreadCount;
	ThreadsArray = (HANDLE*)malloc(sizeof(HANDLE) * ThreadNum);
	
	for(int i=0; i<ThreadNum; i++)
	{
		ThreadsArray[i] = CreateThread(NULL, 0, WorkerThread, this, 0, NULL);
	}
}


BOOL CSimpleThreadPool::PostWorkItem(WORK_ITEM_PROC UserProc, PVOID UserParam)
{
	PWORK_ITEM pWorkItem = (PWORK_ITEM)malloc(sizeof(WORK_ITEM));
	if(pWorkItem == NULL)
		return FALSE;
	
	pWorkItem->UserProc = UserProc;
	pWorkItem->UserParam = UserParam;
	
	EnterCriticalSection(&WorkItemLock);
	WorkItemList.AddTail(pWorkItem);
	LeaveCriticalSection(&WorkItemLock);
	
	InterlockedIncrement(&WorkItemCount);
	ReleaseSemaphore(WorkItemSemaphore, 1, NULL);
	
	return TRUE;
}

void CSimpleThreadPool::DestroyThreadPool()
{
	SetEvent(QuitEvent);
	
	for(int i=0; i<ThreadNum; i++)
	{
		WaitForSingleObject(ThreadsArray[i], INFINITE);
		CloseHandle(ThreadsArray[i]);
	}
	
	free(ThreadsArray);
	
	CloseHandle(QuitEvent);
	CloseHandle(WorkItemSemaphore);
	DeleteCriticalSection(&WorkItemLock);
	
	POSITION pos = WorkItemList.GetHeadPosition();
	while(pos != NULL)
	{
		PWORK_ITEM pWorkItem = (PWORK_ITEM)WorkItemList.GetNext(pos);
		ASSERT(pWorkItem != NULL);

		free(pWorkItem);
	}
	WorkItemList.RemoveAll();
}