#ifndef _MY_SHELL_MANANGER_H_
#define _MY_SHELL_MANANGER_H_

#include <shlobj.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _MYCBITEMINFO
{
	LPSHELLFOLDER  pParentFolder;
	LPITEMIDLIST   pidlFQ;
	LPITEMIDLIST   pidlRel;
	
	_MYCBITEMINFO()
	{
		pParentFolder = NULL;
		pidlFQ = NULL;
		pidlRel = NULL;
	}
}
MYCBITEMINFO, FAR *LPMYCBITEMINFO;

class CMyShellManager : public CObject
{
	friend class CShellListCtrl;
	// Construction
public:
	CMyShellManager();
	virtual ~CMyShellManager();
	
	// Operations
public:
	BOOL BrowseForFolder (	CString& strOutFolder,
		CWnd* pWndParent = NULL,
		LPCTSTR lplszInitialFolder = NULL,
		LPCTSTR lpszTitle = NULL, 
		UINT ulFlags = BIF_RETURNONLYFSDIRS, // Same as flags in BROWSEINFO structure
		LPINT piFolderImage = NULL);
	
	LPITEMIDLIST CreateItem (UINT cbSize);
	void FreeItem (LPITEMIDLIST pidl);
	
	UINT GetItemCount (LPCITEMIDLIST pidl);
	UINT GetItemSize (LPCITEMIDLIST pidl);
	
	LPITEMIDLIST ConcatenateItem (LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
	LPITEMIDLIST CopyItem (LPCITEMIDLIST pidlSource);
	
	LPITEMIDLIST GetNextItem(LPCITEMIDLIST pidl);
	int GetParentItem (LPCITEMIDLIST lpidl, LPITEMIDLIST& lpidlParent);
	
	HRESULT ItemFromPath (LPCTSTR lpszPath, LPITEMIDLIST& pidl);
	
protected:
	static int CALLBACK BrowseCallbackProc (HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	
	// Attributes
protected:
	LPMALLOC	m_pBCGMalloc;
	LPCTSTR		m_lpszInitialPath;	// Valid in BrowseForFolder only
};

extern CMyShellManager* g_pShellManager;

extern UINT MYPM_ON_AFTER_SHELL_COMMAND;

#endif
