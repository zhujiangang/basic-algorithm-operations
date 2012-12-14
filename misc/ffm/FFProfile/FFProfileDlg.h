// FFProfileDlg.h : header file
//

#if !defined(AFX_FFPROFILEDLG_H__7CCEFC6B_69B8_4AFB_B6E9_F36560CDC9A7__INCLUDED_)
#define AFX_FFPROFILEDLG_H__7CCEFC6B_69B8_4AFB_B6E9_F36560CDC9A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFFProfileDlg dialog

#include "CmdBuilder.h"

class CFFProfileDlg : public CDialog
{
// Construction
public:
	CFFProfileDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL GetPropMap(PropMap* pPropMap);

// Dialog Data
	//{{AFX_DATA(CFFProfileDlg)
	enum { IDD = IDD_FFPROFILE_DIALOG };
	CStatic	m_wndPropListLocation;
	CBCGPPropList m_wndPropList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFFProfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	void InitPropList();
	void GetPropValue(CBCGPProp* pProp, PropMap* pPropMap);
	// Generated message map functions
	//{{AFX_MSG(CFFProfileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FFPROFILEDLG_H__7CCEFC6B_69B8_4AFB_B6E9_F36560CDC9A7__INCLUDED_)
