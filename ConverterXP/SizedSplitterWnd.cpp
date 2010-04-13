// SizedSplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ConverterXP.h"
#include "SizedSplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSizedSplitterWnd

IMPLEMENT_DYNCREATE(CSizedSplitterWnd, CSplitterWnd)

CSizedSplitterWnd::CSizedSplitterWnd()
{
	minSize.cx = 0;
	minSize.cy = 0;
}

CSizedSplitterWnd::~CSizedSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CSizedSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CSizedSplitterWnd)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSizedSplitterWnd drawing


/////////////////////////////////////////////////////////////////////////////
// CSizedSplitterWnd diagnostics

#ifdef _DEBUG
void CSizedSplitterWnd::AssertValid() const
{
	CSplitterWnd::AssertValid();
}

void CSizedSplitterWnd::Dump(CDumpContext& dc) const
{
	CSplitterWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSizedSplitterWnd message handlers
void CSizedSplitterWnd::setMinSize(CSize size)
{
	minSize = size;
}
void CSizedSplitterWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(minSize.cx > 0 && point.x <= minSize.cx)
	{
		point.x = minSize.cx;
		//return;
	}
	if(minSize.cy > 0 && point.y <= minSize.cy)
	{
		point.y = minSize.cy;
		//return;
	}
	CSplitterWnd::OnMouseMove(nFlags, point);
}
