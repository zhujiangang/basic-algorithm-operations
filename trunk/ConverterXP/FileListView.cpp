// FileListView.cpp : implementation file
//

#include "stdafx.h"
#include "ConverterXP.h"
#include "FileListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileListView

IMPLEMENT_DYNCREATE(CFileListView, CScrollView)

CFileListView::CFileListView()
{
}

CFileListView::~CFileListView()
{
}


BEGIN_MESSAGE_MAP(CFileListView, CScrollView)
	//{{AFX_MSG_MAP(CFileListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileListView drawing

void CFileListView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CFileListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CFileListView diagnostics

#ifdef _DEBUG
void CFileListView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFileListView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFileListView message handlers
