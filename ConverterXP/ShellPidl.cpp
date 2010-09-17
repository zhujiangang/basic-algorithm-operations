// ShellPidl.cpp: implementation of the CShellPidl class.
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

#include "stdafx.h"
#include <afxpriv.h>
#include "ShellPidl.h"

//////////////////////////////////////////////////////////////////////
// Static Functions

// does not include terminating zero
// NULL objects get a zero length
UINT CShellPidl::ILGetLength(LPCITEMIDLIST pidl)
{
	if (pidl == NULL)
		return 0;

	UINT length = 0, cb;
	do
	{
		cb = pidl->mkid.cb;
		pidl = (LPCITEMIDLIST)((LPBYTE)pidl + cb);
		length += cb;
	}
	while (cb != 0);

	return length;
}

LPCITEMIDLIST CShellPidl::ILGetNext(LPCITEMIDLIST pidl)
{
	if (pidl == NULL)
		return NULL;

	// Get the size of the specified item identifier. 
	int cb = pidl->mkid.cb; 
	
	// If the size is zero, it is the end of the list. 
	if (cb == 0) 
		return NULL; 
	
	// Add cb to pidl (casting to increment by bytes). 
	pidl = (LPCITEMIDLIST)((LPBYTE)pidl + cb); 
	
	// Return NULL if we reached the terminator, or a pidl otherwise. 
	return (pidl->mkid.cb == 0) ? NULL : pidl;
}

LPCITEMIDLIST CShellPidl::ILGetLast(LPCITEMIDLIST pidl)
{
	LPCITEMIDLIST pidlLast = pidl, tmp = ILGetNext(pidl);
	while (tmp != NULL)
	{
		pidlLast = tmp;
		tmp = ILGetNext(tmp);
	}

	return pidlLast;
}

LPITEMIDLIST CShellPidl::ILCombine(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
	// check arguments
	if (pidl1 == NULL && pidl2 == NULL)
		return NULL;

	// Get the size of the resulting item identifier list
	UINT cb1 = ILGetLength(pidl1);
	UINT cb2 = ILGetLength(pidl2);

	// Allocate a new item identifier list
	SMallocPtr pMalloc;
	LPITEMIDLIST pidlNew = (LPITEMIDLIST)pMalloc->Alloc(cb1 + cb2 + sizeof(USHORT));
	LPITEMIDLIST pidlEnd = pidlNew;
	if (pidlNew != NULL)
	{		
		// Copy the first item identifier list and terminating 0
		if (cb1 > 0)
		{
			CopyMemory(pidlEnd, pidl1, cb1);
			pidlEnd = (LPITEMIDLIST)((LPBYTE)pidlEnd + cb1);
		}

		// Copy the second item identifier list and terminating 0
		if (cb2 > 0)
		{
			CopyMemory(pidlEnd, pidl2, cb2);
			pidlEnd = (LPITEMIDLIST)((LPBYTE)pidlEnd + cb2);
		}

		// Append a terminating zero. 
		pidlEnd->mkid.cb = 0;
	}
	return pidlNew;
}

LPITEMIDLIST CShellPidl::ILCloneFirst(LPCITEMIDLIST pidl)
{
	LPITEMIDLIST pidlNew = NULL;

	if (pidl != NULL)
	{
		// Get the size of the first item identifier.
		int cb = pidl->mkid.cb; 
		
		// Allocate a new item identifier list. 
		SMallocPtr pMalloc;
		pidlNew = (LPITEMIDLIST)pMalloc->Alloc(cb + sizeof(USHORT)); 

		if (pidlNew != NULL) 
		{		
			// Copy the specified item identifier. 
			CopyMemory(pidlNew, pidl, cb); 
			
			// Append a terminating zero. 
			((LPITEMIDLIST)((LPBYTE)pidlNew + cb))->mkid.cb = 0; 
		}
	}
	return pidlNew;
}

LPITEMIDLIST CShellPidl::ILCloneParent(LPCITEMIDLIST pidl)
{
	// Get the size of the parent item identifier. 
	UINT cb = (UINT)ILGetLast(pidl) - (UINT)pidl;

	SMallocPtr pMalloc;
	LPITEMIDLIST pidlNew = (LPITEMIDLIST)pMalloc->Alloc(cb + sizeof(USHORT)); 

	if (pidlNew != NULL)
	{			
		// Copy the specified item identifier. 
		CopyMemory(pidlNew, pidl, cb); 
		
		// Append a terminating zero. 
		((LPITEMIDLIST)((LPBYTE)pidlNew + cb))->mkid.cb = 0; 
	}
	return pidlNew;
}

LPITEMIDLIST CShellPidl::ILFromPath(LPCTSTR pszPath, HWND hOwner)
{
	USES_CONVERSION;

	ULONG count = 0;
	LPITEMIDLIST pidlNew = NULL;

	SDesktopFolderPtr pDesktopFolder;
	pDesktopFolder->ParseDisplayName(hOwner, NULL,
		T2OLE(pszPath), &count, &pidlNew, NULL);

	return pidlNew;
}

//////////////////////////////////////////////////////////////////////
// Member Functions

// debug

#ifdef _DEBUG
void CShellPidl::Dump(CDumpContext& dc) const
{
	dc << "addr = " << (void*)m_pObj << "\n";
	if (m_pObj == NULL)
		return;

	dc << "[structure]\n";
	
	LPCITEMIDLIST pidl = m_pObj;
	while (pidl != NULL)
	{
		dc << " " << pidl->mkid.cb;
		pidl = ILGetNext(pidl);
	}
	dc << "\n";
}
#endif //_DEBUG

