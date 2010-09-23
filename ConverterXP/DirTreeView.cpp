// DirTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "ConverterXP.h"
#include "DirTreeView.h"
#include "ShellListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView

IMPLEMENT_DYNCREATE(CDirTreeView, CTreeCtrl)

CDirTreeView::CDirTreeView()
{
}

CDirTreeView::~CDirTreeView()
{
	AfxTrace("CDirTreeView Destructor %d\n", this);
}


BEGIN_MESSAGE_MAP(CDirTreeView, CShellTreeCtrl)
	//{{AFX_MSG_MAP(CDirTreeView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(MSG_SHELL_LIST_CHANGE_CURRENT_FOLDER, OnShellListFolderChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView drawing

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView diagnostics

#ifdef _DEBUG
void CDirTreeView::AssertValid() const
{
	CShellTreeCtrl::AssertValid();
}

void CDirTreeView::Dump(CDumpContext& dc) const
{
	CShellTreeCtrl::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView message handlers

BOOL CDirTreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
//	SetShellOwner();
	
	cs.lpszName = _T("ShellTreeCtrl");
	cs.style |= TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.lpszClass = WC_TREEVIEW;
	
	return CShellTreeCtrl::PreCreateWindow(cs);	// TODO: Add your specialized code here and/or call the base class
	
//	return CShellTreeCtrl::PreCreateWindow(cs);
}

LRESULT CDirTreeView::OnShellListFolderChanged(WPARAM wParam, LPARAM lParam)
{
	AfxTrace("Received\n");

	LPITEMIDLIST lpidl = (LPITEMIDLIST)lParam;
	this->SelectPath(lpidl);

	return (LRESULT)0;
}
