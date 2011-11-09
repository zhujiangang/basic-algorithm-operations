// SyncQueue.h: interface for the CSyncQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNCQUEUE_H__9AD9ADCB_68BF_4ED0_A5C7_4D484AB0B7BB__INCLUDED_)
#define AFX_SYNCQUEUE_H__9AD9ADCB_68BF_4ED0_A5C7_4D484AB0B7BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSyncQueue  
{
public:
	virtual ~CSyncQueue();

	virtual void Put(void* ptr) = 0;
	virtual void* Get() = 0;
	
	virtual int GetCount() = 0;
	virtual BOOL IsEmpty() = 0;
};

#endif // !defined(AFX_SYNCQUEUE_H__9AD9ADCB_68BF_4ED0_A5C7_4D484AB0B7BB__INCLUDED_)
