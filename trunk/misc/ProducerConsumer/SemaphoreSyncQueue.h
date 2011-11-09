// SemaphoreSyncQueue.h: interface for the CSemaphoreSyncQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMAPHORESYNCQUEUE_H__AD968515_02FC_4766_AAEE_D27CE378D7A1__INCLUDED_)
#define AFX_SEMAPHORESYNCQUEUE_H__AD968515_02FC_4766_AAEE_D27CE378D7A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SyncQueue.h"

class CSemaphoreSyncQueue : public CSyncQueue
{
public:
	CSemaphoreSyncQueue(int nCapacity = -1);
	virtual ~CSemaphoreSyncQueue();
	virtual void Put(void* ptr);
	virtual void* Get();
	
	virtual int GetCount();
	virtual BOOL IsEmpty();
protected:
	int m_nCapacity;
	CPtrList m_list;
	
	CRITICAL_SECTION m_lock;	
	HANDLE m_hGetSemaphore;
	HANDLE m_hPutSemaphore;
};

#endif // !defined(AFX_SEMAPHORESYNCQUEUE_H__AD968515_02FC_4766_AAEE_D27CE378D7A1__INCLUDED_)
