// SemaphoreSyncQueue.cpp: implementation of the CSemaphoreSyncQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemaphoreSyncQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemaphoreSyncQueue::CSemaphoreSyncQueue(int nCapacity)
{
	m_nCapacity = nCapacity;
	
	::InitializeCriticalSection(&m_lock);

	int nMaxCount = (m_nCapacity <= 0) ? 0x7FFFFFFF : m_nCapacity;
	m_hPutSemaphore = ::CreateSemaphore(NULL, nMaxCount, nMaxCount, NULL);
	m_hGetSemaphore = ::CreateSemaphore(NULL, 0, nMaxCount, NULL);
}

CSemaphoreSyncQueue::~CSemaphoreSyncQueue()
{
	m_list.RemoveAll();
	
	::CloseHandle(m_hGetSemaphore);
	::CloseHandle(m_hPutSemaphore);
	
	::DeleteCriticalSection(&m_lock);
}

void CSemaphoreSyncQueue::Put(void* ptr)
{
	if(::WaitForSingleObject(m_hPutSemaphore, INFINITE) != WAIT_OBJECT_0)
	{
		ASSERT(FALSE);
	}

	::EnterCriticalSection(&m_lock);
	if(m_nCapacity > 0)
	{
		ASSERT(m_list.GetCount() < m_nCapacity);
	}
	m_list.AddTail(ptr);
	::LeaveCriticalSection(&m_lock);
	
	::ReleaseSemaphore(m_hGetSemaphore, 1, NULL);
}
void* CSemaphoreSyncQueue::Get()
{
	if(::WaitForSingleObject(m_hGetSemaphore, INFINITE) != WAIT_OBJECT_0)
	{
		ASSERT(FALSE);
	}
	::EnterCriticalSection(&m_lock);
	ASSERT(!m_list.IsEmpty());
	void* ptr = m_list.RemoveHead();
	::LeaveCriticalSection(&m_lock);
	
	::ReleaseSemaphore(m_hPutSemaphore, 1, NULL);
	
	return ptr;
}

int CSemaphoreSyncQueue::GetCount()
{
	::EnterCriticalSection(&m_lock);
	int nCount = m_list.GetCount();
	::LeaveCriticalSection(&m_lock);
	
	return nCount;
}
BOOL CSemaphoreSyncQueue::IsEmpty()
{
	::EnterCriticalSection(&m_lock);
	BOOL bEmpty = m_list.IsEmpty();
	::LeaveCriticalSection(&m_lock);
	
	return bEmpty;
}
