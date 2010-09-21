#if !defined(AFX_SHELLLISTCTRL_H__9DE14028_0237_4B64_9014_CDA04C1BA3B2__INCLUDED_)
#define AFX_SHELLLISTCTRL_H__9DE14028_0237_4B64_9014_CDA04C1BA3B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShellListCtrl.h : header file
//

#include "MyShellManager.h"

/////////////////////////////////////////////////////////////////////////////
// CShellListCtrl window

class CShellListCtrl : public CListCtrl
{
// Construction
public:
	CShellListCtrl();

// Attributes
public:
	enum
	{
		BCGPShellList_ColumnName = 0,
		BCGPShellList_ColumnSize = 1,
		BCGPShellList_ColumnType = 2,
		BCGPShellList_ColumnModified = 3,
	}
	BCGPShellListColumns;
	const IShellFolder* GetCurrentShellFolder () const
	{
		return m_psfCurFolder;
	}
	
	LPITEMIDLIST GetCurrentItemIdList () const
	{
		return m_pidlCurFQ;
	}
	
	BOOL IsDesktop () const
	{
		return m_bIsDesktop;
	}
protected:
	IShellFolder*	m_psfCurFolder;
	LPITEMIDLIST	m_pidlCurFQ;
	BOOL			m_bContextMenu;
	HWND			m_hwndRelatedTree;
	BOOL			m_bIsDesktop;
	BOOL			m_bNoNotify;
	SHCONTF			m_nTypes;
	
	static IContextMenu2*	m_pContextMenu2;

// Operations
public:
	virtual HRESULT Refresh ();
	virtual HRESULT DisplayFolder (LPCTSTR lpszPath);
	virtual HRESULT DisplayFolder (LPMYCBITEMINFO lpItemInfo);
	virtual HRESULT DisplayParentFolder ();
// Overrides
	virtual void OnSetColumns ();
	virtual CString OnGetItemText (int iItem, int iColumn, LPMYCBITEMINFO pItem);
	virtual int OnGetItemIcon (int iItem, LPMYCBITEMINFO pItem);
	
	virtual void OnFormatFileSize (long lFileSize, CString& str);
	virtual void OnFormatFileDate (const CTime& tmFile, CString& str);
	
	virtual int OnCompareItems (LPARAM lParam1, LPARAM lParam2, int iColumn);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellListCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShellListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShellListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

		
	HIMAGELIST GetShellImageList (BOOL bLarge);
	HRESULT LockCurrentFolder (LPMYCBITEMINFO pItemInfo);
	void ReleaseCurrFolder ();
	
	virtual HRESULT EnumObjects (LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
	virtual void DoDefault (int iItem);
	
	BOOL InitList ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLLISTCTRL_H__9DE14028_0237_4B64_9014_CDA04C1BA3B2__INCLUDED_)