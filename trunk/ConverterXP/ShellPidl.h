// ShellPidl.h: interface for the CShellPidl class.
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

#if !defined(AFX_SHELLPIDL_H__98F90381_3F3A_4DF6_948D_B9BB960E7A96__INCLUDED_)
#define AFX_SHELLPIDL_H__98F90381_3F3A_4DF6_948D_B9BB960E7A96__INCLUDED_

#include <shlobj.h>
#include "ShellWrappers.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShellPidl : public TSharedObject<ITEMIDLIST>
{
protected:
	static LPCITEMIDLIST ILGetLast(LPCITEMIDLIST pidl);
	// manage PIDLs
	static UINT ILGetLength(LPCITEMIDLIST pidl);
	static LPCITEMIDLIST ILGetNext(LPCITEMIDLIST pidl);
	static LPITEMIDLIST ILCombine(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
	static LPITEMIDLIST ILClone(LPCITEMIDLIST pidl);
	static LPITEMIDLIST ILCloneFirst(LPCITEMIDLIST pidl);
	static LPITEMIDLIST ILCloneParent(LPCITEMIDLIST pidl);
	static LPITEMIDLIST ILFromPath(LPCTSTR pszPath, HWND hOwner = NULL);

public:
	CShellPidl(LPCTSTR pszPath, HWND hOwner = NULL);
	CShellPidl(UINT nSpecialFolder, HWND hOwner = NULL);
	CShellPidl(LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidlRel);
	CShellPidl(LPCITEMIDLIST pidl);
	CShellPidl();
	virtual ~CShellPidl();

	int GetIconIndex(UINT uFlags = SHGFI_SMALLICON) const;
	BOOL IsRoot() const;

	void Combine(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
	void CloneLastParent(LPCITEMIDLIST pidl);
	void CloneLastChild(LPCITEMIDLIST pidl);
	void CloneFirstParent(LPCITEMIDLIST pidl);
	void CloneFirstChild(LPCITEMIDLIST pidl);

	LPCITEMIDLIST GetFirstChild();
	LPCITEMIDLIST GetLastChild();

	CString GetPath() const;

// debug support 
#ifdef _DEBUG
	void Dump(CDumpContext& dc) const;
#endif //_DEBUG
};

//////////////////////////////////////////////////////////////////////
// Inline Member Functions

inline CShellPidl::CShellPidl()
{
}

inline CShellPidl::~CShellPidl()
{
}

inline CShellPidl::CShellPidl(LPCITEMIDLIST pidl)
{
	m_pObj = ILClone(pidl);
}

inline CShellPidl::CShellPidl(LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidlRel)
{
	m_pObj = ILCombine(pidlParent, pidlRel);
}

inline CShellPidl::CShellPidl(UINT nSpecialFolder, HWND hOwner)
{
	SHGetSpecialFolderLocation(hOwner, nSpecialFolder, &m_pObj);
}

inline CShellPidl::CShellPidl(LPCTSTR pszPath, HWND hOwner)
{
	m_pObj = ILFromPath(pszPath, hOwner);
}

inline BOOL CShellPidl::IsRoot() const
{
	return (m_pObj != NULL) && (m_pObj->mkid.cb == 0);
}

inline CString CShellPidl::GetPath() const
{
	CString path;
	BOOL bSuccess = SHGetPathFromIDList(m_pObj, path.GetBuffer(MAX_PATH));
	path.ReleaseBuffer();
	if (!bSuccess)
		path.Empty();
	return path;
}

inline int CShellPidl::GetIconIndex(UINT uFlags) const
{
	SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	uFlags |= SHGFI_PIDL | SHGFI_SYSICONINDEX;
	SHGetFileInfo((LPCTSTR)m_pObj, 0, &sfi, sizeof(SHFILEINFO), uFlags);
	return sfi.iIcon;
}

inline void CShellPidl::Combine(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
	Destroy();
	Attach(ILCombine(pidl1, pidl2));
}

// get the first ancestor
inline void CShellPidl::CloneFirstParent(LPCITEMIDLIST pidl)
{
	Destroy();
	Attach(ILCloneFirst(pidl));
}

inline void CShellPidl::CloneFirstChild(LPCITEMIDLIST pidl)
{
	Destroy();
	Attach(ILClone(ILGetNext(pidl)));
}

inline LPCITEMIDLIST CShellPidl::GetFirstChild()
{
	return ILGetNext(m_pObj);
}

// get the immediate parent
inline void CShellPidl::CloneLastParent(LPCITEMIDLIST pidl)
{
	Destroy();
	Attach(ILCloneParent(pidl));
}

inline void CShellPidl::CloneLastChild(LPCITEMIDLIST pidl)
{
	Destroy();
	Attach(ILClone(ILGetLast(pidl)));
}

inline LPCITEMIDLIST CShellPidl::GetLastChild()
{
	return ILGetLast(m_pObj);
}

//////////////////////////////////////////////////////////////////////
// Inline Static Functions

inline LPITEMIDLIST CShellPidl::ILClone(LPCITEMIDLIST pidl)
{
	return ILCombine(NULL, pidl);
}


#endif // !defined(AFX_SHELLPIDL_H__98F90381_3F3A_4DF6_948D_B9BB960E7A96__INCLUDED_)

