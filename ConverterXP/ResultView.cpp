// ResultView.cpp : implementation file
//

#include "stdafx.h"
#include "ConverterXP.h"
#include "ResultView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultView

IMPLEMENT_DYNCREATE(CResultView, CScrollView)

CResultView::CResultView()
{
}

CResultView::~CResultView()
{
}


BEGIN_MESSAGE_MAP(CResultView, CScrollView)
	//{{AFX_MSG_MAP(CResultView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultView drawing

void CResultView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CResultView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CResultView diagnostics

#ifdef _DEBUG
void CResultView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CResultView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CResultView message handlers
