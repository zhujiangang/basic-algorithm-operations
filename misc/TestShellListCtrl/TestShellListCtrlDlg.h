// TestShellListCtrlDlg.h : header file
//

#if !defined(AFX_TESTSHELLLISTCTRLDLG_H__2DF090FF_2732_497E_8279_103E067AFEAE__INCLUDED_)
#define AFX_TESTSHELLLISTCTRLDLG_H__2DF090FF_2732_497E_8279_103E067AFEAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShellListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CTestShellListCtrlDlg dialog

class CTestShellListCtrlDlg : public CDialog
{
// Construction
public:
	CTestShellListCtrlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestShellListCtrlDlg)
	enum { IDD = IDD_TESTSHELLLISTCTRL_DIALOG };
	CShellListCtrl	m_shellListCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestShellListCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestShellListCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTSHELLLISTCTRLDLG_H__2DF090FF_2732_497E_8279_103E067AFEAE__INCLUDED_)
