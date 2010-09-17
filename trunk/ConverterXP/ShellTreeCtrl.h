#if !defined(AFX_SHELLTREECTRL_H__98BDBB7B_E2C3_4145_A5D5_693274C6B99B__INCLUDED_)
#define AFX_SHELLTREECTRL_H__98BDBB7B_E2C3_4145_A5D5_693274C6B99B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShellTreeCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000-2001 by Paolo Messina
// (http://www.geocities.com/ppescher - ppescher@yahoo.com)
//
// The contents of this file are subject to the Artistic License (the "License").
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.opensource.org/licenses/artistic-license.html
//
// If you find this code useful, credits would be nice!
//
/////////////////////////////////////////////////////////////////////////////

#include "WaitingTreeCtrl.h"
#include "ShellPidl.h"
#include "ShellString.h"
#include "ShellContextMenu.h"

/////////////////////////////////////////////////////////////////////////////
// CShellTreeCtrl window

// item flags
#define STCF_DEFAULT		0x00
#define STCF_INCLUDEFILES	0x01
#define STCF_INCLUDEHIDDEN	0x02
#define STCF_INCLUDEALL		(STCF_INCLUDEFILES|STCF_INCLUDEHIDDEN)
#define STCF_INCLUDEMASK	0x0F
#define STCF_SHOWFULLNAME	0x10
#define STCF_SHOWPATH		0x20
#define STCF_SHOWFULLPATH	(STCF_SHOWPATH|STCF_SHOWFULLNAME)
#define STCF_SHOWMASK		0xF0
//#define STCF_DEFERLINKS		0x100

class CShellTreeCtrl : public CWaitingTreeCtrl
{
private:
	struct TVITEMDATA
	{
		SShellFolderPtr pParentFolder;
		CShellPidl pidlAbs;
		UINT nFlags;

		BOOL IsValid()
		{
			return (this != NULL)
				&& pParentFolder.IsValid() && pidlAbs.IsValid();
		}
	};

	SDesktopFolderPtr m_pDesktopFolder;
	UINT m_nCallbackMask;

private:
	// generic
	void InitializeControl();

	BOOL EnumFolderItems(HTREEITEM hParent, LPCITEMIDLIST pidlParent, UINT nFlags);
	void InsertSubItem(HTREEITEM hParent, LPSHELLFOLDER pParentFolder, LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidl, UINT nFlags);
	void FillItem(TVITEM& item);

	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

protected:
	virtual BOOL PopulateItem(HTREEITEM hParent);

// Construction
public:
	CShellTreeCtrl();
	virtual ~CShellTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellTreeCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	UINT GetCallbackMask();
	void SetCallbackMask(UINT nMask);
	void AddRootItem(LPCITEMIDLIST pidl, UINT nFlags = STCF_DEFAULT);
	void AddRootFolderContent(LPCITEMIDLIST pidl, UINT nFlags = STCF_DEFAULT);
	CShellPidl GetItemIDList(HTREEITEM hItem);
	BOOL GetItemContextMenu(HTREEITEM hItem, CShellContextMenu &rCtxMenu);

	// Generated message map functions
protected:
	//{{AFX_MSG(CShellTreeCtrl)
	afx_msg void OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLTREECTRL_H__98BDBB7B_E2C3_4145_A5D5_693274C6B99B__INCLUDED_)
