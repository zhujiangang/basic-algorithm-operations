// ShellString.h: interface for the CShellString class.
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

#if !defined(AFX_SHELLSTRING_H__1E9F6E1A_5076_4264_8747_713C53BAD6FE__INCLUDED_)
#define AFX_SHELLSTRING_H__1E9F6E1A_5076_4264_8747_713C53BAD6FE__INCLUDED_

#include <shlobj.h>
#include "ShellWrappers.h"
#include "ShellPidl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShellString  
{
protected:
	CShellPidl m_pidl;
	STRRET m_str;

public:
	CShellString();
	virtual ~CShellString();

	// obtain an empty object by pointer
	STRRET* GetPointer(LPCITEMIDLIST pidl, UINT uDesiredType = STRRET_WSTR);

	operator CString() const;

protected:

	void Destroy();
};

// inline functions

inline CShellString::CShellString()
{
	ZeroMemory(&m_str, sizeof(STRRET));
}

inline CShellString::~CShellString()
{
	Destroy();
}

#endif // !defined(AFX_SHELLSTRING_H__1E9F6E1A_5076_4264_8747_713C53BAD6FE__INCLUDED_)
