// CJFlatHeaderCtrl.cpp : implementation file
//
// Copyright © 1998-99 Kirk W. Stowell
//		mailto:kstowell@codejockeys.com
//		http://www.codejockeys.com/kstowell/
//
// This source code may be used in compiled form in any way you desire.
// Source file(s) may be redistributed unmodified by any means PROVIDING 
// they ARE NOT sold for profit without the authors expressed written 
// consent, and providing that this notice and the authors name and all 
// copyright notices remain intact. This software is by no means to be 
// included as part of any third party components library, or as part any
// development solution that offers MFC extensions that are sold for profit. 
// 
// If the source code is used in any commercial applications then a statement 
// along the lines of:
// 
// "Portions Copyright © 1998-99 Kirk Stowell" must be included in the startup 
// banner, "About" box or printed documentation. This software is provided 
// "as is" without express or implied warranty. Use it at your own risk! The 
// author(s) accepts no liability for any damage/loss of business that this 
// product may cause.
//
/////////////////////////////////////////////////////////////////////////////
/****************************************************************************
 *
 * $Date: 6/23/99 12:04a $
 * $Revision: 14 $
 * $Archive: /CodeJockey/CJLibrary/CJFlatHeaderCtrl.cpp $
 *
 * $History: CJFlatHeaderCtrl.cpp $
 * 
 * *****************  Version 14  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:04a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 13  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 10:00p
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 12  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:30a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 11  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:15a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 10  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:33a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 9  *****************
 * User: Kirk Stowell Date: 7/18/99    Time: 10:27p
 * Updated in $/CodeJockey/CJLibrary
 * Added vc5 compatibility.
 * 
 * *****************  Version 8  *****************
 * User: Kirk Stowell Date: 6/14/99    Time: 10:43p
 * Updated in $/CodeJockey/CJ60Lib
 * Cleaned up code to eliminate level 4 compilier warnings.
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 5/17/99    Time: 8:00p
 * Updated in $/CodeJockey/CJ60Lib
 * Added popup menu support and vc5 compatibility.
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 5/12/99    Time: 5:32a
 * Updated in $/CodeJockey/CJ60Lib
 * Added support for vc5 compatibility
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 4/15/99    Time: 4:27p
 * Updated in $/CodeJockey/CJ60Lib
 * Igor Ostriz [iostriz@usa.net] - fixed bug with drag/drop and changing
 * column order.
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 4/03/99    Time: 4:26p
 * Updated in $/CodeJockey/CJ60Lib
 * Added comments and cleaned up code.
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 2/24/99    Time: 4:26p
 * Updated in $/CodeJockey/CJ60Lib
 * Fixed repainting problem when header is moved or window is resized. The
 * class now uses log font instead of hard coded Tahoma font.
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 1/31/99    Time: 4:27p
 * Updated in $/CodeJockey/CJ60Lib
 * Fixed problem with sort arrow drawn. If hditem has an associated
 * bitmap, the arrow is not drawn.
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 1/16/99    Time: 4:26p
 * Created in $/CodeJockey/CJ60Lib
 * Initial release.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LCCFlatHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLCCFlatHeaderCtrl

CLCCFlatHeaderCtrl::CLCCFlatHeaderCtrl()
{
	m_nSortCol		= -1;
	m_bBoldFont		= FALSE;
	m_nOffset		= 6;
	m_bLBtnDown		= FALSE;
	m_popupMenuID	= 0;
	m_popToolbarID	= 0;
	m_nPos			= 0;

	m_bWaitCursor	= FALSE;


	m_bIsMousePressed = FALSE;
	m_bMultipleSort = FALSE;
	m_bAscending = TRUE;
	m_nHighlightedItem = -1;
	m_bTracked = FALSE;
	m_bIsDlgControl = FALSE;
	m_hFont = NULL;
}

CLCCFlatHeaderCtrl::~CLCCFlatHeaderCtrl()
{
}

BEGIN_MESSAGE_MAP(CLCCFlatHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CLCCFlatHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLCCFlatHeaderCtrl message handlers

void CLCCFlatHeaderCtrl::InitializeHeader(bool bBoldFont)
{
	m_Font.DeleteObject();
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	if(bBoldFont)
		ncm.lfMessageFont.lfWeight = 700;
	VERIFY(m_Font.CreateFontIndirect(&ncm.lfMessageFont));
	SetFont(&m_Font);

	m_bBoldFont = bBoldFont;
}

int CLCCFlatHeaderCtrl::SetSortImage(int nCol, BOOL bAsc)
{
	int nPrevCol = m_nSortCol;
	
	m_nSortCol = nCol;
	m_bSortAsc = bAsc;
	
	// Change the item to ownder drawn
	HD_ITEM hditem;
	
	hditem.mask = HDI_BITMAP | HDI_FORMAT;
	GetItem( nCol, &hditem );
	if (hditem.hbm == NULL)
	{
		hditem.fmt |= HDF_OWNERDRAW;
		SetItem( nCol, &hditem );

		// Invalidate header control so that it gets redrawn
		Invalidate();
	}
	
	return nPrevCol;
}

void CLCCFlatHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC dc;

	dc.Attach( lpDrawItemStruct->hDC );
	
	// Get the column rect
	CRect rcLabel( lpDrawItemStruct->rcItem );
	
	// Save DC
	int nSavedDC = dc.SaveDC();
	
	// Set clipping region to limit drawing within column
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcLabel );
	dc.SelectObject( &rgn );
	rgn.DeleteObject();
	
	// Draw the background
	COLORREF clr = ::GetSysColor( COLOR_3DFACE );
	CBrush brush( clr );
	dc.FillRect( rcLabel, &brush );
	dc.SetBkMode(TRANSPARENT);

	// Get the column text and format
	TCHAR buf[256];
	HD_ITEM hditem;
	
	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = buf;
	hditem.cchTextMax = 255;
	
	GetItem( lpDrawItemStruct->itemID, &hditem );
	
	// Determine format for drawing column label
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP 
		| DT_VCENTER | DT_END_ELLIPSIS ;
	
	if( hditem.fmt & HDF_CENTER)
		uFormat |= DT_CENTER;
	else if( hditem.fmt & HDF_RIGHT)
		uFormat |= DT_RIGHT;
	else
		uFormat |= DT_LEFT;
	
	// Adjust the rect if the mouse button is pressed on it
	if( lpDrawItemStruct->itemState == ODS_SELECTED )
	{
		rcLabel.left++;
		rcLabel.top += 2;
		rcLabel.right++;
	}
	
	// Adjust the rect further if Sort arrow is to be displayed
	if( lpDrawItemStruct->itemID == (UINT)m_nSortCol )
	{
		rcLabel.right -= 3 * m_nOffset;
	}
	
	rcLabel.left += m_nOffset;
	rcLabel.right -= m_nOffset;

	// Draw column label
	if( rcLabel.left < rcLabel.right )
	{
		if (m_bBoldFont) {
			dc.SelectObject(&m_Font);
		}
		dc.DrawText(buf,-1,rcLabel, uFormat);
	}

	// Draw the Sort arrow
	if( lpDrawItemStruct->itemID == (UINT)m_nSortCol )
	{
		CRect rcIcon( lpDrawItemStruct->rcItem );
		
		// Set up pens to use for drawing the triangle
		CPen penLite(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
		CPen penShad(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
		CPen *pOldPen = dc.SelectObject( &penLite );
		
		if( m_bSortAsc )
		{
			// Draw triangle pointing upwards
			dc.MoveTo( rcIcon.right - 2*m_nOffset, m_nOffset-1);
			dc.LineTo( rcIcon.right - 3*m_nOffset/2, rcIcon.bottom - m_nOffset );
			dc.LineTo( rcIcon.right - 5*m_nOffset/2-2, rcIcon.bottom - m_nOffset );
			dc.MoveTo( rcIcon.right - 5*m_nOffset/2-1, rcIcon.bottom - m_nOffset-1 );
			
			dc.SelectObject( &penShad );
			dc.LineTo( rcIcon.right - 2*m_nOffset, m_nOffset-2);
		}
		else
		{
			// Draw triangle pointing downwords
			dc.MoveTo( rcIcon.right - 3*m_nOffset/2, m_nOffset-1);
			dc.LineTo( rcIcon.right - 2*m_nOffset-1, rcIcon.bottom - m_nOffset + 1 );
			dc.MoveTo( rcIcon.right - 2*m_nOffset-1, rcIcon.bottom - m_nOffset );
			
			dc.SelectObject( &penShad );
			dc.LineTo( rcIcon.right - 5*m_nOffset/2-1, m_nOffset -1 );
			dc.LineTo( rcIcon.right - 3*m_nOffset/2, m_nOffset -1);
		}
		
		// Restore the pen
		dc.SelectObject( pOldPen );
	}

	// Restore dc
	dc.RestoreDC( nSavedDC );
	
	// Detach the dc before returning
	dc.Detach();
}

void CLCCFlatHeaderCtrl::OnPaint() 
{
	Default();
//	DrawFlatBorder();
}

void CLCCFlatHeaderCtrl::DrawFlatBorder()
{
	CDC* pDC = GetDC();

	CRect rci;
	GetWindowRect(&rci);
	ScreenToClient(&rci);

	// Cover up thick 3D border.
	pDC->Draw3dRect(rci, ::GetSysColor(COLOR_3DFACE),
		::GetSysColor(COLOR_3DFACE));
	rci.DeflateRect(1,1);
	pDC->Draw3dRect(rci, ::GetSysColor(COLOR_3DFACE),
		::GetSysColor(COLOR_3DFACE));

	// Draw flat style border around entire header.
	rci.InflateRect(1,1);
	pDC->Draw3dRect(rci, ::GetSysColor(COLOR_3DHILIGHT),
		::GetSysColor(COLOR_3DSHADOW));

	// Create the pens for further cover-up.
	CPen penLite(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
	CPen penShad(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
	CPen penFace(PS_SOLID, 1, ::GetSysColor(COLOR_3DFACE));
	CPen *pOldPen = pDC->SelectObject( &penLite );

	pDC->SelectObject(&penFace);
	pDC->MoveTo(rci.right-1, 2);
	pDC->LineTo(rci.right-1, rci.bottom-2);

	// Set up the header item struct.
	HD_ITEM hdi;
	memset (&hdi, 0, sizeof(HD_ITEM));
	hdi.fmt  = HDF_STRING | HDF_LEFT | HDF_OWNERDRAW;
	hdi.mask = HDI_WIDTH | HDI_TEXT | HDI_FORMAT;
	int cx = 0;

	int ic = GetItemCount();
	int *pioArray = new int[ic];
	GetOrderArray( pioArray, ic ); // get appearing order

	// must not omit second parameter: bug in 
	// WINCTRL6.CPP, mixed iCount and nCount
	
	// For each header item found, do further cover up.
	for (int i = 0; i < GetItemCount(); ++i)
	{
		GetItem(pioArray[i], &hdi);
		cx += hdi.cxy;

		pDC->SelectObject(&penShad);
		pDC->MoveTo(cx, 2);
		pDC->LineTo(cx, rci.bottom-2);

		pDC->SelectObject(&penLite);
		pDC->MoveTo(cx+1, 2);
		pDC->LineTo(cx+1, rci.bottom-2);

		pDC->SelectObject(&penFace);
		pDC->MoveTo(cx-1, 2);
		pDC->LineTo(cx-1, rci.bottom-2);

		pDC->SelectObject(&penFace);
		pDC->MoveTo(cx-2, 2);
		pDC->LineTo(cx-2, rci.bottom-2);
	} // end for loop

	delete [] pioArray; //clean up

	// Restore the pen and release device context.
	pDC->SelectObject( pOldPen );
	ReleaseDC(pDC);
}

void CLCCFlatHeaderCtrl::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->cx += 3;	// Stefan Belopotocan, 1999.5.11
	CHeaderCtrl::OnWindowPosChanging(lpwndpos);
}

void CLCCFlatHeaderCtrl::OnRButtonDown(UINT /*nFlags*/, CPoint point) 
{
	// No menu was defined...
	if (!m_popupMenuID) 
		return;

	// Since we are using the main window as the
	// parent, we need to convert the point to screen
	// coordinates...
	/*
	CPoint pt = point;
	ClientToScreen(&pt);
	
	CCraftFTPApp * pTheApp = (CCraftFTPApp *)AfxGetApp();
	pTheApp->ShowPopupMenu(m_popupMenuID, pt, this);
	*/
}

#ifdef _VC_VERSION_5
BOOL CLCCFlatHeaderCtrl::GetOrderArray(LPINT piArray, int iCount /* = -1 */)
{
	ASSERT(::IsWindow(m_hWnd));

	// if -1 was passed, find the count ourselves

	int nCount = iCount;
	if (nCount == -1)
	{
		nCount = GetItemCount();

		if (nCount == -1)
			return FALSE;
	}

	ASSERT(AfxIsValidAddress(piArray, iCount * sizeof(int)));

	return (BOOL) ::SendMessage(m_hWnd, HDM_GETORDERARRAY,
		(WPARAM) iCount, (LPARAM) piArray);
}
#endif // _VC_VERSION_5

BOOL CLCCFlatHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bWaitCursor)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		return TRUE;
	}
	else
		return CHeaderCtrl::OnSetCursor(pWnd, nHitTest, message);
}

int CLCCFlatHeaderCtrl::GetColumnState (int iColumn) const
{
	int nSortVal = 0;
	m_mapColumnsStatus.Lookup (iColumn, nSortVal);
	
	return nSortVal;
}
void CLCCFlatHeaderCtrl::SetSortColumn (int iColumn, BOOL bAscending, BOOL bAdd)
{
	ASSERT_VALID (this);

	if (iColumn < 0)
	{
		m_mapColumnsStatus.RemoveAll ();
		return;
	}

	if (bAdd)
	{
		if (!m_bMultipleSort)
		{
			ASSERT (FALSE);
			bAdd = FALSE;
		}
	}

	if (!bAdd)
	{
		m_mapColumnsStatus.RemoveAll ();
	}

	m_mapColumnsStatus.SetAt (iColumn, bAscending ? 1 : -1);
	RedrawWindow ();
}
//***************************************************************************************
void CLCCFlatHeaderCtrl::RemoveSortColumn (int iColumn)
{
	ASSERT_VALID (this);
	m_mapColumnsStatus.RemoveKey (iColumn);
	RedrawWindow ();
}

void CLCCFlatHeaderCtrl::EnableMultipleSort (BOOL bEnable)
{
	ASSERT_VALID (this);
	
	if (m_bMultipleSort == bEnable)
	{
		return;
	}
	
	m_bMultipleSort = bEnable;
	
	if (!m_bMultipleSort)
	{
		m_mapColumnsStatus.RemoveAll ();
		
		if (GetSafeHwnd () != NULL)
		{
			RedrawWindow ();
		}
	}
}