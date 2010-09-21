// TestShellListCtrl.h : main header file for the TESTSHELLLISTCTRL application
//

#if !defined(AFX_TESTSHELLLISTCTRL_H__F2A6853E_AB25_4954_B994_519CF6E1874D__INCLUDED_)
#define AFX_TESTSHELLLISTCTRL_H__F2A6853E_AB25_4954_B994_519CF6E1874D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestShellListCtrlApp:
// See TestShellListCtrl.cpp for the implementation of this class
//

class CTestShellListCtrlApp : public CWinApp
{
public:
	CTestShellListCtrlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestShellListCtrlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestShellListCtrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTSHELLLISTCTRL_H__F2A6853E_AB25_4954_B994_519CF6E1874D__INCLUDED_)
