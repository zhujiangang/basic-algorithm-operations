// ConverterXP.h : main header file for the CONVERTERXP application
//

#if !defined(AFX_CONVERTERXP_H__98F922FD_3F24_470C_B55F_5A90D40DF9F0__INCLUDED_)
#define AFX_CONVERTERXP_H__98F922FD_3F24_470C_B55F_5A90D40DF9F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CConverterXPApp:
// See ConverterXP.cpp for the implementation of this class
//

class CConverterXPApp : public CWinApp
{
public:
	CConverterXPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConverterXPApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CConverterXPApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONVERTERXP_H__98F922FD_3F24_470C_B55F_5A90D40DF9F0__INCLUDED_)
