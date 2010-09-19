// FileListView.cpp : implementation file
//

#include "stdafx.h"
#include "ConverterXP.h"
#include "FileListView.h"
#include "MainFrm.h"
#include "DirTreeView.h"

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
	m_curPath = "";
}

CFileListView::~CFileListView()
{
	AfxTrace("CFileListView Destructor\n");
}


BEGIN_MESSAGE_MAP(CFileListView, CScrollView)
	//{{AFX_MSG_MAP(CFileListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	
	ON_NOTIFY(TC_SELECT_CHANGED, AFX_IDW_PANE_FIRST, OnDirTreeViewSelChanged)
END_MESSAGE_MAP()

//((CMainFrame*)AfxGetMainWnd())->m_wndSplitterLeftRight.IdFromRowCol(0, 0)
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
	pDC->TextOut(5, 5, m_curPath);
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
void CFileListView::OnDirTreeViewSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	TRACE0("CFileListView::OnDirTreeViewSelChanged\n");
	NMSELCHANGED* pNMMsg = (NMSELCHANGED*)pNMHDR;

	ASSERT(pNMMsg->newItem);
	
	CDirTreeView* pDirTreeView = ((CMainFrame*)AfxGetMainWnd())->GetDirTreeView();
	CShellPidl shPidl = pDirTreeView->GetItemIDList(pNMMsg->newItem);
	m_curPath.Format("%s", shPidl.GetPath());
	this->Invalidate();
	*pResult = 0;
}