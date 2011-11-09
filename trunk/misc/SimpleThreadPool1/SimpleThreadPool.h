// SimpleThreadPool.h: interface for the CSimpleThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLETHREADPOOL_H__20AD0E42_A76D_44AB_B5BB_9D55E7031832__INCLUDED_)
#define AFX_SIMPLETHREADPOOL_H__20AD0E42_A76D_44AB_B5BB_9D55E7031832__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//http://www.cppblog.com/tx7do/archive/2006/04/14/5576.aspx

typedef VOID (*WORK_ITEM_PROC)(PVOID Param);

typedef struct _WORK_ITEM
{
	WORK_ITEM_PROC UserProc;
	PVOID UserParam;
	
}WORK_ITEM, *PWORK_ITEM;

class CSimpleThreadPool  
{
public:
	CSimpleThreadPool();
	virtual ~CSimpleThreadPool();

	void Init(int nThreadCount);
	BOOL PostWorkItem(WORK_ITEM_PROC UserProc, PVOID UserParam);
	void DestroyThreadPool();
public:
	HANDLE QuitEvent;
	HANDLE WorkItemSemaphore;
	
	LONG WorkItemCount;
	CList<PWORK_ITEM, PWORK_ITEM> WorkItemList;
	CRITICAL_SECTION WorkItemLock;
	
	LONG ThreadNum;
	HANDLE *ThreadsArray;
};

#endif // !defined(AFX_SIMPLETHREADPOOL_H__20AD0E42_A76D_44AB_B5BB_9D55E7031832__INCLUDED_)
