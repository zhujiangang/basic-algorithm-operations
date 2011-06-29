// BaseLogger.cpp: implementation of the CBaseLogger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseLogger::CBaseLogger(LPCTSTR lpLogFileName) : m_pLogFile(NULL), m_nLogLevel(10)
{
	m_pLogFile = fopen(lpLogFileName, "w");
	if(m_pLogFile == NULL)
	{
		fprintf(stderr, "Failed to open log file: %s\n", lpLogFileName);
		return;
	}
}

CBaseLogger::~CBaseLogger()
{
	if(m_pLogFile != NULL)
	{
		fclose(m_pLogFile);
		m_pLogFile = NULL;
	}
}

void CBaseLogger::log(int level, const char *format, ...)
{
	if(level > m_nLogLevel)
	{
		return;
	}

	char buffer[1024];
	va_list va;

	va_start(va, format);
    _vsntprintf(buffer, sizeof(buffer), format, va);
    va_end(va);

	fprintf(m_pLogFile, "%s", buffer);
}
