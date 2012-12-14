// SysConf.cpp: implementation of the CSysConf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FFProfile.h"
#include "SysConf.h"
#include <Shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSysConf::CSysConf() : m_bInited(FALSE)
{

}

CSysConf::~CSysConf()
{

}

void CSysConf::Init()
{
	if(m_bInited)
	{
		return;
	}

	//work dir
	TCHAR szFileName[1024] = {0};
	if(::GetModuleFileName(::GetModuleHandle(NULL), szFileName, 1024))
	{
		m_szWorkDir = szFileName;
		::PathRemoveFileSpec(m_szWorkDir.GetBuffer(0));
		m_szWorkDir.ReleaseBuffer(-1);
	}

	//profile dir
	m_szProfileDir.Empty();
	if(!m_szWorkDir.IsEmpty())
	{
		m_szProfileDir.Format(_T("%s\\"), (LPCTSTR)m_szWorkDir);
	}
	m_szProfileDir += _T("profile");

	//output dir
	m_szOutputDir.Empty();
	if(!m_szWorkDir.IsEmpty())
	{
		m_szOutputDir.Format(_T("%s\\"), (LPCTSTR)m_szWorkDir);
	}
	m_szOutputDir += _T("output");

	//bin dir
	m_szBinDir.Empty();

	m_bInited = TRUE;
}
