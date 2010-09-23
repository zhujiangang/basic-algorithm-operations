// WaitingTreeCtrl.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000-2001 by Paolo Messina
// (http://www.geocities.com/ppescher - ppescher@yahoo.com)
//
// The contents of this file are subject to the Artistic License (the "License").
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.opensource.org/licenses/artistic-license.html
//
// If you find this code useful, credits would be nice!
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaitingTreeCtrl.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#include <mmsystem.h>
#pragma warning(pop)

#pragma comment(lib, "winmm.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitingTreeCtrl

CWaitingTreeCtrl::CWaitingTreeCtrl()
{
	m_bDrawSnapshot = FALSE;

	m_sWaitMsg = _T("Loading...");
	m_bShowWaitMsg = FALSE;
	m_hIconMsg = NULL;	// default: blank icon
	m_nTimerDelay = 0;	// default: no timer

	m_hRedrawEvent = NULL;
	m_hTimerEvent = NULL;
	m_hThread = NULL;
	m_bNoNotify = FALSE;
}

CWaitingTreeCtrl::~CWaitingTreeCtrl()
{
	UINT event;
	CList<HWND, HWND>* pList;
	POSITION ps = m_observersMap.GetStartPosition();
	while( ps )
	{  
		m_observersMap.GetNextAssoc(ps, event, pList);
		if(pList != NULL)
		{
			delete pList;
		}
	}
	m_observersMap.RemoveAll();
}


BEGIN_MESSAGE_MAP(CWaitingTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CWaitingTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemExpanded)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitingTreeCtrl message handlers

void CWaitingTreeCtrl::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if (pNMTreeView->action & TVE_EXPAND)
		PreExpandItem(pNMTreeView->itemNew.hItem);

	*pResult = 0;
}

void CWaitingTreeCtrl::OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if (pNMTreeView->action & TVE_EXPAND)
		ExpandItem(pNMTreeView->itemNew.hItem);
	else if (pNMTreeView->action & TVE_COLLAPSE)
	{
		if (WantsRefresh(pNMTreeView->itemNew.hItem))
		{
			// delete child items
			DeleteChildren(pNMTreeView->itemNew.hItem);
		}
	}

	*pResult = 0;
}

void CWaitingTreeCtrl::PreAnimation(HTREEITEM hItemMsg)
{
	UNREFERENCED_PARAMETER(hItemMsg);
}

void CWaitingTreeCtrl::PostAnimation()
{
}

void CWaitingTreeCtrl::DoAnimation(BOOL bTimerEvent, int iMaxSteps, int iStep)
{
	UNREFERENCED_PARAMETER(bTimerEvent);
	UNREFERENCED_PARAMETER(iMaxSteps);
	UNREFERENCED_PARAMETER(iStep);
}

int CWaitingTreeCtrl::GetPopulationCount(int *piMaxSubItems)
{
	if (piMaxSubItems != NULL)
		*piMaxSubItems = m_iItemCount;
	return m_iItemIndex;
}

void CWaitingTreeCtrl::SetPopulationCount(int iMaxSubItems, int iFirstSubItem)
{
	m_iItemCount = iMaxSubItems;
	m_iItemIndex = iFirstSubItem;

	SetEvent(m_hRedrawEvent);
}

void CWaitingTreeCtrl::UpdatePopulation(int iSubItems)
{
	m_iItemIndex = iSubItems;

	SetEvent(m_hRedrawEvent);
}

void CWaitingTreeCtrl::IncreasePopulation(int iSubItemsToAdd)
{
	m_iItemIndex += iSubItemsToAdd;

	SetEvent(m_hRedrawEvent);
}

void CWaitingTreeCtrl::SetAnimationDelay(UINT nMilliseconds)
{
	// if greater than zero, periodic DoAnimation() will be called
	m_nTimerDelay = nMilliseconds;
}

DWORD WINAPI CWaitingTreeCtrl::AnimationThreadProc(LPVOID pThis)
{
	CWaitingTreeCtrl* me = (CWaitingTreeCtrl*)pThis;

	HANDLE events[2] = { me->m_hTimerEvent, me->m_hRedrawEvent };
	
	while (!me->m_bAbortAnimation)
	{
		DWORD wait = WaitForMultipleObjects(2, events, FALSE, INFINITE);
		
		if (me->m_bAbortAnimation || wait == WAIT_FAILED)
			break;

		if (wait == WAIT_OBJECT_0)	// timer event
			me->DoAnimation(TRUE, me->m_iItemCount, me->m_iItemIndex);
		else	// redraw event
			me->DoAnimation(FALSE, me->m_iItemCount, me->m_iItemIndex);
	}

	return 0;
}

void CWaitingTreeCtrl::StartAnimation()
{
	// user-defined setup
	PreAnimation(m_hItemMsg);

	// animation can go
	m_bAbortAnimation = FALSE;
	// automatic reset events, signaled
	m_hTimerEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_hRedrawEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	// start animation thread
	DWORD dwThreadID = 0;
	m_hThread = CreateThread(NULL, 0, AnimationThreadProc, this,
		THREAD_PRIORITY_HIGHEST, &dwThreadID);
	// setup timer, if specified
	if (m_nTimerDelay > 0)
		m_nTimerID = (UINT)timeSetEvent(m_nTimerDelay, 5, (LPTIMECALLBACK)m_hTimerEvent,
			0, TIME_PERIODIC | TIME_CALLBACK_EVENT_SET);
}

void CWaitingTreeCtrl::StopAnimation()
{
	// stop and destroy timer
	timeKillEvent(m_nTimerID);
	// signal thread to terminate
	m_bAbortAnimation = TRUE;
	SetEvent(m_hRedrawEvent);	// make sure it can see the signal
	// wait thread termination
	WaitForSingleObject(m_hThread, INFINITE);
	// clean up
	CloseHandle(m_hTimerEvent);
	m_hTimerEvent = NULL;
	CloseHandle(m_hRedrawEvent);
	m_hRedrawEvent = NULL;
	CloseHandle(m_hThread);
	m_hThread = NULL;

	// user-defined cleanup
	PostAnimation();
}

void CWaitingTreeCtrl::PopulateRoot()
{
	PreExpandItem(TVI_ROOT);
	ExpandItem(TVI_ROOT);
	// force update, don't scroll
	SetRedraw(FALSE);
	SCROLLINFO si;
	GetScrollInfo(SB_HORZ, &si);
	EnsureVisible(GetChildItem(TVI_ROOT));
	SetScrollInfo(SB_HORZ, &si, FALSE);
	SetRedraw();
}

void CWaitingTreeCtrl::PreExpandItem(HTREEITEM hItem)
{
	if (!NeedsChildren(hItem))
	{
		if (WantsRefresh(hItem))
		{
			// delete child items before populating
			DeleteChildren(hItem);
		}
		else
		{
			// doesn't want new items
			m_hItemToPopulate = NULL;
			return;
		}
	}
	// if it wants new child items, go on
	m_hItemToPopulate = hItem;

	// fix redraw when expanded programatically
	UpdateWindow();
	// hide changes until it's expanded
	SetRedraw(FALSE);
	// add wait msg, to allow item expansion
	m_hItemMsg = InsertItem(m_sWaitMsg, m_hItemToPopulate);
	// zero progress
	m_iItemCount = 1;
	m_iItemIndex = 0;
}

void CWaitingTreeCtrl::ExpandItem(HTREEITEM hItem)
{
	if (m_hItemToPopulate == NULL)
		return;	// just expand, doesn't want new items

	ASSERT(hItem == m_hItemToPopulate);	// should never fail!!!

	if (m_bShowWaitMsg)
	{
		// display wait msg now, make sure it's visible
		SetRedraw();
		EnsureVisible(m_hItemMsg);
		UpdateWindow();
	}
	// setup animation thread, call PreAnimation
	StartAnimation();
	// draw icon
	if (m_bShowWaitMsg)
		DrawUserIcon();
	// delay redraw after populating
	SetRedraw(FALSE);
	// take a snapshot of the background
	TakeSnapshot();
	// del temporary item (wait msg still shown)
	DeleteItem(m_hItemMsg);
	// fill in with sub items
	BOOL bCheckChildren = PopulateItem(hItem);
	// clean up animation thread, call PostAnimation
	StopAnimation();
	// change parent to reflect current children number
	if (hItem != TVI_ROOT)
	{
		TVITEM item;
		item.hItem = hItem;
		item.mask = TVIF_HANDLE | TVIF_CHILDREN;
		item.cChildren = NeedsChildren(hItem) ? 0 : 1;
		if (bCheckChildren)
			SetItem(&item);
		else if (item.cChildren == 0)
			// restore item's plus button if no children inserted
			SetItemState(hItem, 0, TVIS_EXPANDED);
	}
	// clean up snapshot
	DestroySnapshot();
	// redraw now
	SetRedraw(TRUE);
	// scroll like in a standard expansion
	HTREEITEM hChild = GetChildItem(hItem);
	while (hChild != NULL && GetFirstVisibleItem() != hItem)
	{
//		EnsureVisible(hChild);
		hChild = GetNextSiblingItem(hChild);
	}
}

BOOL CWaitingTreeCtrl::WantsRefresh(HTREEITEM hItem)
{
	UNREFERENCED_PARAMETER(hItem);

	// default implementation, no refresh
	return FALSE;
}

BOOL CWaitingTreeCtrl::GetItemImageRect(HTREEITEM hItem, LPRECT pRect)
{
	if (GetImageList(TVSIL_NORMAL) == NULL)
		return FALSE;	// no images

	CRect rc;
	// get item rect
	if (!GetItemRect(hItem, &rc, TRUE))
		return FALSE;

	int cx = GetSystemMetrics(SM_CXSMICON);
	int cy = GetSystemMetrics(SM_CYSMICON);

	// move onto the icon space
	int margin = (rc.Height()-cy)/2;
	rc.OffsetRect(-cx-3 , margin);
	rc.right = rc.left + cx;	// make it square
	rc.bottom = rc.top + cy;	// make it square

	*pRect = rc;
	return TRUE;
}

void CWaitingTreeCtrl::DrawUserIcon()
{
	// draw user defined icon

	CRect rcIcon;
	if (!GetItemImageRect(m_hItemMsg, &rcIcon))
		return;	// no image

	// create background brush with current bg color (take rgb part only)
	HBRUSH hBrush = CreateSolidBrush(GetBkColor() & 0x00FFFFFF);

	CClientDC dc(this);

	if (m_hIconMsg != NULL)
		DrawIconEx(dc.GetSafeHdc(), rcIcon.left, rcIcon.top, m_hIconMsg,
			rcIcon.Width(), rcIcon.Height(), 0, hBrush, DI_NORMAL);
	else
		FillRect(dc.GetSafeHdc(), &rcIcon, hBrush);

	DeleteObject(hBrush);
}

void CWaitingTreeCtrl::SetWaitMessage(LPCTSTR pszText, HICON hIcon)
{
	m_sWaitMsg = pszText;
	m_hIconMsg = hIcon;
}

void CWaitingTreeCtrl::RefreshSubItems(HTREEITEM hParent)
{
	if (hParent != TVI_ROOT && !ItemHasChildren(hParent))
		return;

	SetRedraw(FALSE);
	DeleteChildren(hParent);
	if (hParent == TVI_ROOT)
		PopulateRoot();
	else
	{
		PreExpandItem(hParent);
		ExpandItem(hParent);
	}
	SetRedraw(TRUE);
}

inline BOOL CWaitingTreeCtrl::NeedsChildren(HTREEITEM hParent)
{
	return (GetChildItem(hParent) == NULL);
}

void CWaitingTreeCtrl::DeleteChildren(HTREEITEM hParent)
{
	HTREEITEM hChild = GetChildItem(hParent);
	HTREEITEM hNext;

	while (hChild != NULL)
	{
		hNext = GetNextSiblingItem(hChild);
		DeleteItem(hChild);
		hChild = hNext;
	}
}

BOOL CWaitingTreeCtrl::OnEraseBkgnd(CDC* pDC) 
{
	if (!m_bDrawSnapshot)
		return CTreeCtrl::OnEraseBkgnd(pDC);

	DrawSnapshot(pDC);
	SetEvent(m_hRedrawEvent);

	return TRUE;
}

void CWaitingTreeCtrl::TakeSnapshot()
{
	CClientDC dc(this);
	CRect rcClient;
	GetClientRect(&rcClient);
	int width = rcClient.Width(), height = rcClient.Height();

	// create the snapshot
	CDC dcSnapshot;
	dcSnapshot.CreateCompatibleDC(&dc);
	m_bmpSnapshot.CreateCompatibleBitmap(&dc, width, height);
	// copy the control's background
	CBitmap* pOldBmp = dcSnapshot.SelectObject(&m_bmpSnapshot);
	dcSnapshot.BitBlt(0, 0, width, height, &dc, 0, 0, SRCCOPY);
	dcSnapshot.SelectObject(pOldBmp);

	m_bDrawSnapshot = TRUE;
}

void CWaitingTreeCtrl::DrawSnapshot(CDC *pDC)
{
	BITMAP bm;
	m_bmpSnapshot.GetBitmap(&bm);

	// prepare the snapshot
	CDC dcSnapshot;
	dcSnapshot.CreateCompatibleDC(pDC);
	// copy to the control's background
	CBitmap* pOldBmp = dcSnapshot.SelectObject(&m_bmpSnapshot);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcSnapshot, 0, 0, SRCCOPY);
	dcSnapshot.SelectObject(pOldBmp);
}

void CWaitingTreeCtrl::DestroySnapshot()
{
	m_bmpSnapshot.DeleteObject();

	m_bDrawSnapshot = FALSE;
}

void CWaitingTreeCtrl::AddObserver(UINT event, HWND hwnd)
{
	CList<HWND, HWND>* pList = NULL;
	
	if(!m_observersMap.Lookup(event, pList))
	{
		pList = new CList<HWND, HWND>();
		m_observersMap.SetAt(event, pList);
	}
	pList->AddTail(hwnd);
}

void CWaitingTreeCtrl::RemoveObserver(UINT event, HWND hwnd)
{
	CList<HWND, HWND>* pList = NULL;
	ASSERT( m_observersMap.Lookup(event, pList) );
	POSITION ps = pList->Find(hwnd);
	ASSERT(ps != NULL);
	pList->RemoveAt(ps);
}

void CWaitingTreeCtrl::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!m_bNoNotify)
	{
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		
		NMSELCHANGED nm;
		nm.hwndFrom = m_hWnd;
		nm.idFrom = GetDlgCtrlID();
		nm.code = TC_SELECT_CHANGED;
		nm.oldItem = pNMTreeView->itemOld.hItem;
		nm.newItem = pNMTreeView->itemNew.hItem;
		
		CList<HWND, HWND>* pList = NULL;	
		if(m_observersMap.Lookup(TC_SELECT_CHANGED, pList))
		{
			POSITION ps = pList->GetHeadPosition();
			while(ps != NULL)
			{
				HWND hwnd = pList->GetNext(ps);
				::SendMessage(hwnd, WM_NOTIFY, nm.idFrom, (LPARAM)&nm);
			}
		}
	}	
	*pResult = 0;
}
