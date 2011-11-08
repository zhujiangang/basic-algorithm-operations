// SynQueue.h: interface for the CSynQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNQUEUE_H__2B8EFD76_8322_418E_ABB9_EDB87E223F29__INCLUDED_)
#define AFX_SYNQUEUE_H__2B8EFD76_8322_418E_ABB9_EDB87E223F29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//http://www.99inf.net/SoftwareDev/VC/13218.htm
class CSynQueue  
{
public:
	CSynQueue();
	virtual ~CSynQueue();
	
	void Put(void* ptr);
	void* Get();

	int GetCount();
	void SetMaxCount(int nMax = -1, BOOL bDrop = FALSE);
private:
	CRITICAL_SECTION m_lock;
	CPtrList m_list;

	HANDLE m_hGetEvent;
	HANDLE m_hPutEvent;

	int m_nMax;
	BOOL m_bDrop;
};

#endif // !defined(AFX_SYNQUEUE_H__2B8EFD76_8322_418E_ABB9_EDB87E223F29__INCLUDED_)
