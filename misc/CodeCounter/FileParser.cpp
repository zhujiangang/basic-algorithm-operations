#include "StdAfx.h"
#include "FileParser.h"

CFileInfo::CFileInfo() : m_nTotalLines(0), m_nCodeLines(0), m_nCommentLines(0), m_nBlankLines(0), m_nMixedLines(0)
{
}

void CFileInfo::SetFileName(LPCTSTR lpszFullFileName)
{
    TCHAR drive[MAX_PATH];
    TCHAR dir[MAX_PATH];
    TCHAR fname[MAX_PATH];
    TCHAR ext[MAX_PATH];
	
    m_sFullFileName = lpszFullFileName;
	
    _tsplitpath(m_sFullFileName, drive, dir, fname, ext);
	
    m_sFilePath = drive;
    m_sFilePath += dir;
	
	m_sFileExt = ext;
	
    m_sFileName = fname;
	m_sFileName += ext;
}

UINT CFileInfo::GetMixedLines() const
{
	UINT nResult = (m_nCodeLines + m_nCommentLines) - (m_nTotalLines - m_nBlankLines);
	ASSERT(nResult == m_nMixedLines);
	return nResult;
}
void CFileInfo::Increase(DWORD dwFlags)
{
	if(dwFlags == MASK_TOTAL_LINE)
	{
		m_nTotalLines++;
		return;
	}
	if(dwFlags & MASK_CODE_LINE)
	{
		m_nCodeLines++;
	}
	if(dwFlags & MASK_COMMENT_LINE)
	{
		m_nCommentLines++;
	}
	if(dwFlags & MASK_BLANK_LINE)
	{
		m_nBlankLines++;
	}
	if(dwFlags & MASK_MIXED_LINE)
	{
		m_nMixedLines++;
	}
}

CTotalInfo::CTotalInfo()
{
	Reset();
}
void CTotalInfo::Reset()
{
	m_nTotalCount = 0;
	m_nTotalLines = 0;
	m_nTotalCodeLines = 0;
	m_nTotalCommentLines = 0;
	m_nTotalBlankLines = 0;
	m_nTotalMixedLines = 0;
}
UINT CTotalInfo::GetTotalMixedLines() const
{
	UINT nResult = (m_nTotalCodeLines + m_nTotalCommentLines) - (m_nTotalLines - m_nTotalBlankLines);
	ASSERT(nResult == m_nTotalMixedLines);
	return nResult;
}


CPair::CPair()
{
}
CPair::CPair(LPCTSTR lpszStart, LPCTSTR lpszEnd) : m_szStart(lpszStart), m_szEnd(lpszEnd)
{
}

CLangGrammar::CLangGrammar()
{
}
CLangGrammar::~CLangGrammar()
{
	m_singleCommentArray.RemoveAll();
	m_escapeCharArray.RemoveAll();
	m_multiCommentArray.RemoveAll();
	m_strMarkArray.RemoveAll();
}