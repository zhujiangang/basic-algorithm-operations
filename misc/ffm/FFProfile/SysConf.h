// SysConf.h: interface for the CSysConf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSCONF_H__A582EF5E_80FB_43A6_9FBA_117410506C08__INCLUDED_)
#define AFX_SYSCONF_H__A582EF5E_80FB_43A6_9FBA_117410506C08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSysConf  
{
public:
	CSysConf();
	virtual ~CSysConf();

	void Init();

	BOOL	m_bInited;
	CString	m_szWorkDir;
	CString	m_szProfileDir;
	CString m_szOutputDir;
	CString m_szBinDir;
};

#endif // !defined(AFX_SYSCONF_H__A582EF5E_80FB_43A6_9FBA_117410506C08__INCLUDED_)
