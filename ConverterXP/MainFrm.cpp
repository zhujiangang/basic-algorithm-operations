// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ConverterXP.h"

#include "MainFrm.h"
#include "DirTreeView.h"
#include "FileListView.h"
#include "ResultView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
// 	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
// 		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
// 	{
// 		TRACE0("Failed to create view window\n");
// 		return -1;
// 	}
//	CreateClient();
	
// 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
// 		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
// 		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
// 	{
// 		TRACE0("Failed to create toolbar\n");
// 		return -1;      // fail to create
// 	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
// 	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
// 	EnableDocking(CBRS_ALIGN_ANY);
// 	DockControlBar(&m_wndToolBar);

	return 0;
}

void CMainFrame::CreateClient()
{

}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
// 	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
// 	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
// 		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{	
	m_wndSplitter1.setMinSize(CSize(0, 100));
	m_wndSplitter2.setMinSize(CSize(100, 0));

	m_wndSplitter1.CreateStatic(this, 2, 1);	
	m_wndSplitter2.CreateStatic(&m_wndSplitter1, 1, 2,
		WS_CHILD | WS_VISIBLE, m_wndSplitter1.IdFromRowCol(0, 0));
	m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CDirTreeView), CSize(300, 0), NULL);
	m_wndSplitter2.CreateView(0, 1, RUNTIME_CLASS(CFileListView), CSize(0, 0), NULL);
	
	m_wndSplitter1.CreateView(1, 0, RUNTIME_CLASS(CResultView), CSize(0, 500), NULL);

	m_wndSplitter1.SetRowInfo(0, 200, 10);
	m_wndSplitter1.RecalcLayout();

	m_wndSplitter2.SetColumnInfo(0, 200, 10);
	m_wndSplitter2.RecalcLayout();

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
