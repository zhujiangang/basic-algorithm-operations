// BaseLogger.h: interface for the CBaseLogger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASELOGGER_H__983B4738_1B36_4E2A_98FE_0527471286E9__INCLUDED_)
#define AFX_BASELOGGER_H__983B4738_1B36_4E2A_98FE_0527471286E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseLogger  
{
public:
	CBaseLogger(LPCTSTR lpLogFileName);
	virtual ~CBaseLogger();
	void log(int level, const char *format, ...);
protected:
	FILE* m_pLogFile;
	int m_nLogLevel;
};

#endif // !defined(AFX_BASELOGGER_H__983B4738_1B36_4E2A_98FE_0527471286E9__INCLUDED_)
