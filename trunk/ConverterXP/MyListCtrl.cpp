// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl()
{
	m_iSortedColumn = -1;
	m_bAscending = TRUE;
	m_bMarkSortedColumn = FALSE;
	m_clrSortedColumn = (COLORREF)-1;
	m_hOldFont = NULL;
}

CMyListCtrl::~CMyListCtrl()
{
}

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STYLECHANGED, OnStyleChanged)
END_MESSAGE_MAP()
//ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)


/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers

BOOL CMyListCtrl::InitList ()
{
	InitHeader ();
	InitColors ();
	return TRUE;
}
//*********************************************************************************
void CMyListCtrl::InitHeader ()
{
	//---------------------------
	// Initialize header control:
	//---------------------------
	m_wndHeader.SubclassDlgItem (0, this);
}
//*********************************************************************************
void CMyListCtrl::PreSubclassWindow() 
{
	CListCtrl::PreSubclassWindow();

	HWND hWndControl = ::GetDlgItem(m_hWnd, 0);
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState ();
	if (pThreadState->m_pWndInit == NULL)
	{
		if (!InitList ())
		{
			ASSERT(FALSE);
		}
	}
}
//*********************************************************************************
int CMyListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	HWND hWndControl = ::GetDlgItem(m_hWnd, 0);
	if (!InitList ())
	{
		return -1;
	}

	return 0;
}
//*********************************************************************************
void CMyListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	ASSERT (pNMListView != NULL);

	int iColumn = pNMListView->iSubItem;
	BOOL bShiftIsPressed = (::GetAsyncKeyState (VK_SHIFT) & 0x8000);
	int nColumnState = GetHeaderCtrl ().GetColumnState (iColumn);
	BOOL bAscending = TRUE;
	
	if (nColumnState != 0)
	{
		bAscending = nColumnState <= 0;
	}

	Sort (iColumn, bAscending, bShiftIsPressed && IsMultipleSort ());
	*pResult = 0;
}
void CMyListCtrl::Sort(int iColumn)
{
	int nColumnState = GetHeaderCtrl ().GetColumnState (iColumn);
	BOOL bAscending = TRUE;
	
	if (nColumnState != 0)
	{
		bAscending = nColumnState <= 0;
	}

	Sort(iColumn, bAscending, FALSE);
}
//*********************************************************************************
void CMyListCtrl::Sort (int iColumn, BOOL bAscending, BOOL bAdd)
{
	CWaitCursor wait;

	GetHeaderCtrl ().SetSortColumn (iColumn, bAscending, bAdd);

	m_iSortedColumn = iColumn;
	m_bAscending = bAscending;

	SortItems (CompareProc, (LPARAM) this);

	GetHeaderCtrl ().SetSortImage( iColumn, bAscending );
}
//*********************************************************************************
void CMyListCtrl::SetSortColumn (int iColumn, BOOL bAscending, BOOL bAdd)
{
	GetHeaderCtrl ().SetSortColumn (iColumn, bAscending, bAdd);
}
//*********************************************************************************
void CMyListCtrl::RemoveSortColumn (int iColumn)
{
	GetHeaderCtrl ().RemoveSortColumn (iColumn);
}
//*********************************************************************************
void CMyListCtrl::EnableMultipleSort (BOOL bEnable)
{
	GetHeaderCtrl ().EnableMultipleSort (bEnable);
}
//*********************************************************************************
BOOL CMyListCtrl::IsMultipleSort () const
{
	return ((CMyListCtrl*) this)->GetHeaderCtrl ().IsMultipleSort ();
}
//*********************************************************************************
int CMyListCtrl::OnCompareItems (LPARAM /*lParam1*/, 
								  LPARAM /*lParam2*/, 
								  int /*iColumn*/)
{
	return 0;
}
//***************************************************************************************
int CALLBACK CMyListCtrl::CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMyListCtrl* pList = (CMyListCtrl*) lParamSort;
	ASSERT_VALID (pList);

	int nRes = pList->OnCompareItems (lParam1, lParam2, pList->m_iSortedColumn);
	if (!pList->m_bAscending)
	{
		nRes = -nRes;
	}

	return nRes;
}
//****************************************************************************************
	/*
void CMyListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{

	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

    switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
	    *pResult = CDRF_NOTIFYITEMDRAW;
	    break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{
			int iColumn = lplvcd->iSubItem;
			int iRow = (int) lplvcd->nmcd.dwItemSpec;

			lplvcd->clrTextBk = OnGetCellBkColor (iRow, iColumn);
			lplvcd->clrText = OnGetCellTextColor (iRow, iColumn);

			if (iColumn == m_iSortedColumn && m_bMarkSortedColumn &&
				lplvcd->clrTextBk == GetBkColor ())
			{
				lplvcd->clrTextBk = m_clrSortedColumn;
			}

			HFONT hFont = OnGetCellFont (	iRow, iColumn, 
											(DWORD) lplvcd->nmcd.lItemlParam);
				
			if (hFont != NULL)
			{
				m_hOldFont = (HFONT) SelectObject (lplvcd->nmcd.hdc, hFont);
				ASSERT (m_hOldFont != NULL);

				*pResult = CDRF_NEWFONT | CDRF_NOTIFYPOSTPAINT;
			}
			else
			{
				*pResult = CDRF_DODEFAULT;
			}
		}
	    break;

	case CDDS_ITEMPOSTPAINT | CDDS_SUBITEM:
		if (m_hOldFont != NULL)
		{
			SelectObject (lplvcd->nmcd.hdc, m_hOldFont);
			m_hOldFont = NULL;
		}

		*pResult = CDRF_DODEFAULT;
		break;
	}

}
	*/
//****************************************************************************************
void CMyListCtrl::EnableMarkSortedColumn (BOOL bMark/* = TRUE*/,
										   BOOL bRedraw/* = TRUE */)
{
	m_bMarkSortedColumn = bMark;

	if (GetSafeHwnd () != NULL && bRedraw)
	{
		RedrawWindow ();
	}
}
//****************************************************************************************
BOOL CMyListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	BOOL bRes = CListCtrl::OnEraseBkgnd(pDC);

	if (m_iSortedColumn >= 0 && m_bMarkSortedColumn)
	{
		CRect rectClient;
		GetClientRect (&rectClient);

		CRect rectHeader;
		GetHeaderCtrl ().GetItemRect (m_iSortedColumn, &rectHeader);
		GetHeaderCtrl ().MapWindowPoints (this, rectHeader);

		CRect rectColumn = rectClient;
		rectColumn.left = rectHeader.left;
		rectColumn.right = rectHeader.right;

		CBrush br (m_clrSortedColumn);
		pDC->FillRect (rectColumn, &br);
	}

	return bRes;
}
//*****************************************************************************************
void CMyListCtrl::OnSysColorChange() 
{
	CListCtrl::OnSysColorChange();
	
	InitColors ();
	RedrawWindow ();
}
//****************************************************************************************
void CMyListCtrl::InitColors ()
{
	//@TODO
	/*
	m_clrSortedColumn = CBCGPDrawManager::PixelAlpha (
			GetBkColor (), .97, .97, .97);

	int ipercent = 100 - percent;
	COLORREF clrFinal = RGB ( (GetRValue (srcPixel) * percent + GetRValue (dstPixel) * ipercent) / 100, 
		(GetGValue (srcPixel) * percent + GetGValue (dstPixel) * ipercent) / 100, 
		(GetBValue (srcPixel) * percent + GetBValue (dstPixel) * ipercent) / 100);
	*/
	m_clrSortedColumn = GetBkColor ();
}
//***************************************************************************************
LRESULT CMyListCtrl::OnStyleChanged(WPARAM wp, LPARAM lp)
{
	int nStyleType = (int) wp;
	LPSTYLESTRUCT lpStyleStruct = (LPSTYLESTRUCT) lp;

	CListCtrl::OnStyleChanged (nStyleType, lpStyleStruct);

	if ((lpStyleStruct->styleNew & LVS_REPORT) &&
		(lpStyleStruct->styleOld & LVS_REPORT) == 0)
	{
		if (GetHeaderCtrl ().GetSafeHwnd () == NULL)
		{
			InitHeader ();
		}
	}

	return 0;
}

void CMyListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	
	if (m_wndHeader.GetSafeHwnd () != NULL)
	{
		m_wndHeader.RedrawWindow ();
	}
}
