// SynQueue.cpp: implementation of the CSynQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SynQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSynQueue::CSynQueue()
{
	m_nMax = -1;
	m_bDrop = FALSE;

	::InitializeCriticalSection(&m_lock);

	m_hPutEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hGetEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CSynQueue::~CSynQueue()
{
	m_list.RemoveAll();

	::CloseHandle(m_hGetEvent);
	::CloseHandle(m_hPutEvent);

	::DeleteCriticalSection(&m_lock);
}

void CSynQueue::Put(void* ptr)
{
	::EnterCriticalSection(&m_lock);

	if(m_nMax > 0 && m_list.GetCount() >= m_nMax)
	{
		::ResetEvent(m_hPutEvent);
		::LeaveCriticalSection(&m_lock);

		if(m_bDrop)
		{
			return;
		}

		if(::WaitForSingleObject(m_hPutEvent, INFINITE) != WAIT_OBJECT_0)
		{
			ASSERT(FALSE);
		}

		::EnterCriticalSection(&m_lock);
	}

	m_list.AddTail(ptr);
	::SetEvent(m_hGetEvent);
	::LeaveCriticalSection(&m_lock);
}
void* CSynQueue::Get()
{
	::EnterCriticalSection(&m_lock);

	if(m_list.IsEmpty())
	{
		::ResetEvent(m_hGetEvent);
		::LeaveCriticalSection(&m_lock);
		
		if(::WaitForSingleObject(m_hGetEvent, INFINITE) != WAIT_OBJECT_0)
		{
			ASSERT(FALSE);
		}
		
		::EnterCriticalSection(&m_lock);
	}
	ASSERT(!m_list.IsEmpty());
	void* ptr = m_list.RemoveHead();
	::SetEvent(m_hPutEvent);
	::LeaveCriticalSection(&m_lock);

	return ptr;
}

int CSynQueue::GetCount()
{
	int nCount = 0;

	::EnterCriticalSection(&m_lock);
	nCount = m_list.GetCount();
	::LeaveCriticalSection(&m_lock);

	return nCount;
}
void CSynQueue::SetMaxCount(int nMax, BOOL bDrop)
{
	::EnterCriticalSection(&m_lock);
	m_nMax = nMax;
	m_bDrop = bDrop;
	::LeaveCriticalSection(&m_lock);
}