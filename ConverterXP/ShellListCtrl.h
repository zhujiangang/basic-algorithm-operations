#if !defined(AFX_SHELLLISTCTRL_H__9DE14028_0237_4B64_9014_CDA04C1BA3B2__INCLUDED_)
#define AFX_SHELLLISTCTRL_H__9DE14028_0237_4B64_9014_CDA04C1BA3B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShellListCtrl.h : header file
//

#include "MyShellManager.h"
#include "MyListCtrl.h"
#include "Sorter.h"

/////////////////////////////////////////////////////////////////////////////
// CShellListCtrl window
//extern UINT MSG_SHELL_LIST_CHANGE_CURRENT_FOLDER;
#define MSG_SHELL_LIST_CHANGE_CURRENT_FOLDER (WM_USER + 100)

#ifdef _USE_MY_LIST_CTRL_
#define SHELL_LIST_CTRL_BASE_CLASS CMyListCtrl
#else
#define SHELL_LIST_CTRL_BASE_CLASS CListCtrl
#define SHELL_LIST_CTRL_BASE_SORTER_CLASS CSorter
#endif

class CShellListCtrl : public SHELL_LIST_CTRL_BASE_CLASS, CSorter
{
	DECLARE_DYNAMIC(CShellListCtrl)
// Construction
public:
	CShellListCtrl();

// Attributes
public:
	enum
	{
		SHELL_LIST_COL_NAME = 0,
		SHELL_LIST_COL_SIZE = 1,
		SHELL_LIST_COL_TYPE = 2,
		SHELL_LIST_COL_DATE = 3,
	}
	ShellListColumn;

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
	CList<HWND, HWND> m_folderChangeObservers;
	
	static IContextMenu2*	m_pContextMenu2;

// Operations
public:
	BOOL GetItemPath (CString& strPath, int iItem) const;
	BOOL GetCurrentFolder (CString& strPath) const;
	BOOL GetCurrentFolderName (CString& strName) const;
	void AddObserver(HWND hwnd);
	void RemoveObserver(HWND hwnd);

	virtual HRESULT Refresh ();
	virtual HRESULT DisplayFolder (LPCTSTR lpszPath);
	virtual HRESULT DisplayFolder (LPMYCBITEMINFO lpItemInfo, BOOL sendNotification = TRUE);
	virtual HRESULT DisplayFolder (LPCITEMIDLIST lpidlFQ, BOOL sendNotification = TRUE);
	virtual HRESULT DisplayParentFolder ();
// Overrides
	virtual void OnSetColumns ();
	virtual CString OnGetItemText (int iItem, int iColumn, LPMYCBITEMINFO pItem);
	virtual int OnGetItemIcon (int iItem, LPMYCBITEMINFO pItem);
	
	virtual void OnFormatFileSize (long lFileSize, CString& str);
	virtual void OnFormatFileDate (const CTime& tmFile, CString& str);
	
	virtual int OnCompareItems (LPARAM lParam1, LPARAM lParam2, int iColumn);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
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
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewLargeIcon();
	afx_msg void OnViewSmallIcon();
	afx_msg void OnViewList();
	afx_msg void OnViewDetail();
	afx_msg void OnUpdateViewLargeIcon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewSmallIcon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDetail(CCmdUI* pCmdUI);
	afx_msg void OnArrangeByDate();
	afx_msg void OnArrangeByName();
	afx_msg void OnArrangeBySize();
	afx_msg void OnArrangeByType();
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
