#if !defined(AFX_DIRTREEVIEW_H__5A7A6967_9EA6_43AE_9FAC_F8FA843DF9FC__INCLUDED_)
#define AFX_DIRTREEVIEW_H__5A7A6967_9EA6_43AE_9FAC_F8FA843DF9FC__INCLUDED_

#include "ShellTreeCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirTreeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDirTreeView view

class CDirTreeView : public CShellTreeCtrl
{
protected:
	CDirTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDirTreeView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirTreeView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDirTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDirTreeView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRTREEVIEW_H__5A7A6967_9EA6_43AE_9FAC_F8FA843DF9FC__INCLUDED_)
