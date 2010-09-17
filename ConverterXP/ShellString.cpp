// ShellString.cpp: implementation of the CShellString class.
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
#include "ShellString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CShellString::Destroy()
{
	SMallocPtr pMalloc;
	if (m_str.uType == STRRET_WSTR && m_str.pOleStr != NULL)
	{
		pMalloc->Free(m_str.pOleStr);
		m_str.pOleStr = NULL;
	}
	ZeroMemory(&m_str, sizeof(STRRET));
}

STRRET* CShellString::GetPointer(LPCITEMIDLIST pidl, UINT uDesiredType)
{
	// reset the object
	Destroy();
	// init object
	m_pidl = pidl;
	ASSERT(m_pidl.IsValid());
	m_str.uType = uDesiredType;

	return &m_str;
}

CShellString::operator CString() const
{
	CString string;
	switch (m_str.uType)
	{
	case STRRET_WSTR:
		string = m_str.pOleStr;
		break;
	case STRRET_CSTR:
		string = m_str.cStr;
		break;
	case STRRET_OFFSET:
		string = ((LPBYTE)(LPCITEMIDLIST)m_pidl + m_str.uOffset);
	}
	return string;
}
