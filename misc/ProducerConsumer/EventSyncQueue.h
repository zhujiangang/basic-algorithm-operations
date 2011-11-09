// EventSyncQueue.h: interface for the CEventSyncQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTSYNCQUEUE_H__D53FFDE4_5F8B_4BB8_A055_E914FB95EED3__INCLUDED_)
#define AFX_EVENTSYNCQUEUE_H__D53FFDE4_5F8B_4BB8_A055_E914FB95EED3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SyncQueue.h"

class CEventSyncQueue : public CSyncQueue
{
public:
	CEventSyncQueue(int nCapacity = -1);
	virtual ~CEventSyncQueue();
	virtual void Put(void* ptr);
	virtual void* Get();
	
	virtual int GetCount();
	virtual BOOL IsEmpty();
protected:
	int m_nCapacity;
	CPtrList m_list;
	
	CRITICAL_SECTION m_lock;	
	HANDLE m_hGetEvent;
	HANDLE m_hPutEvent;
};

#endif // !defined(AFX_EVENTSYNCQUEUE_H__D53FFDE4_5F8B_4BB8_A055_E914FB95EED3__INCLUDED_)
