// EventSyncQueue.cpp: implementation of the CEventSyncQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventSyncQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventSyncQueue::CEventSyncQueue(int nCapacity)
{
	m_nCapacity = nCapacity;
	
	::InitializeCriticalSection(&m_lock);
	m_hPutEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hGetEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CEventSyncQueue::~CEventSyncQueue()
{
	m_list.RemoveAll();
	
	::CloseHandle(m_hGetEvent);
	::CloseHandle(m_hPutEvent);
	
	::DeleteCriticalSection(&m_lock);
}

void CEventSyncQueue::Put(void* ptr)
{
	::EnterCriticalSection(&m_lock);
	
	while(m_nCapacity > 0 && m_list.GetCount() >= m_nCapacity)
	{
		::LeaveCriticalSection(&m_lock);
		
		//wait
		if(::WaitForSingleObject(m_hPutEvent, INFINITE) != WAIT_OBJECT_0)
		{
			ASSERT(FALSE);
		}
		
		::EnterCriticalSection(&m_lock);
	}
	if(m_nCapacity > 0)
	{
		ASSERT(m_list.GetCount() < m_nCapacity);
	}
	m_list.AddTail(ptr);

	::SetEvent(m_hGetEvent);	//notifyAll
	::LeaveCriticalSection(&m_lock);
}
void* CEventSyncQueue::Get()
{
	::EnterCriticalSection(&m_lock);
	
	while(m_list.IsEmpty())
	{
		::LeaveCriticalSection(&m_lock);
		
		//wait
		if(::WaitForSingleObject(m_hGetEvent, INFINITE) != WAIT_OBJECT_0)
		{
			ASSERT(FALSE);
		}
		
		::EnterCriticalSection(&m_lock);
	}
	ASSERT(!m_list.IsEmpty());
	void* ptr = m_list.RemoveHead();

	::SetEvent(m_hPutEvent);	//notifyAll
	::LeaveCriticalSection(&m_lock);
	
	return ptr;
}

int CEventSyncQueue::GetCount()
{
	::EnterCriticalSection(&m_lock);
	int nCount = m_list.GetCount();
	::LeaveCriticalSection(&m_lock);

	return nCount;
}
BOOL CEventSyncQueue::IsEmpty()
{
	::EnterCriticalSection(&m_lock);
	BOOL bEmpty = m_list.IsEmpty();
	::LeaveCriticalSection(&m_lock);

	return bEmpty;
}
