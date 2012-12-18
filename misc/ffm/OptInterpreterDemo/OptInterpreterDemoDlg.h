// OptInterpreterDemoDlg.h : header file
//

#if !defined(AFX_OPTINTERPRETERDEMODLG_H__EB3A9105_B883_4618_9E7B_CEBA69981D10__INCLUDED_)
#define AFX_OPTINTERPRETERDEMODLG_H__EB3A9105_B883_4618_9E7B_CEBA69981D10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COptInterpreterDemoDlg dialog

//#include "CompositeOptionExp.h"
//#include "DefaultOptionExp.h"
// #include "OptionContext.h"
// #include "OptionExp.h"

class COptInterpreterDemoDlg : public CDialog
{
// Construction
public:
	COptInterpreterDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(COptInterpreterDemoDlg)
	enum { IDD = IDD_OPTINTERPRETERDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptInterpreterDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	//OptionExp*	m_pOptExp;
	//DefaultOptionExp	m_optExp;
	//OptionContext m_context;
	// Generated message map functions
	//{{AFX_MSG(COptInterpreterDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitOptionExp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTINTERPRETERDEMODLG_H__EB3A9105_B883_4618_9E7B_CEBA69981D10__INCLUDED_)
