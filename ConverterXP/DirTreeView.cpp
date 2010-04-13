// DirTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "ConverterXP.h"
#include "DirTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView

IMPLEMENT_DYNCREATE(CDirTreeView, CScrollView)

CDirTreeView::CDirTreeView()
{
}

CDirTreeView::~CDirTreeView()
{
}


BEGIN_MESSAGE_MAP(CDirTreeView, CScrollView)
	//{{AFX_MSG_MAP(CDirTreeView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView drawing

void CDirTreeView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDirTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView diagnostics

#ifdef _DEBUG
void CDirTreeView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDirTreeView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView message handlers
