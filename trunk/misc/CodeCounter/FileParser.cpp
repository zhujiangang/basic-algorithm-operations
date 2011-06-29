#include "StdAfx.h"
#include "FileParser.h"

CFileInfo::CFileInfo() : m_nTotalLines(0), m_nCodeLines(0), m_nCommentLines(0), m_nBlankLines(0), m_nMixedLines(0)
{
}

bool CFileInfo::operator==(const CFileInfo& other) const
{
	if(m_nTotalLines != other.m_nTotalLines)
	{
		return false;
	}
	if(m_nCodeLines != other.m_nCodeLines)
	{
		return false;
	}
	if(m_nCommentLines != other.m_nCommentLines)
	{
		return false;
	}
	if(m_nBlankLines != other.m_nBlankLines)
	{
		return false;
	}
	if(m_nMixedLines != other.m_nMixedLines)
	{
		return false;
	}
	return true;
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

CSingleLineComment::CSingleLineComment(LPCTSTR lpszCommentStr, int nColumn)
{
	if(lpszCommentStr != NULL)
	{
		m_szTag = lpszCommentStr;
	}
	m_nStartCol = nColumn;
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
	m_multiCommentArray.RemoveAll();
	m_strMarkArray.RemoveAll();
	m_escapeStrArray.RemoveAll();	
}

BOOL CLangGrammar::IsStartsWith(const CString& sSrc, const CString& sPrefix, int nBeginIndex)
{
	if( sSrc.GetLength() >= (nBeginIndex + sPrefix.GetLength()) 
		&& sSrc.Mid(nBeginIndex, sPrefix.GetLength()) == sPrefix )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CLangGrammar::IsSingleLineComment(const CString& sLine, int nBeginIndex)
{
	int i, nCount = m_singleCommentArray.GetSize();

	CSingleLineComment singleComment;
	for(i = 0; i < nCount; i++)
	{
		singleComment = m_singleCommentArray.GetAt(i);

		//TODO: concern about the case "singleComment.m_nStartCol > 0"
		if (sLine.GetLength() >= (nBeginIndex + singleComment.m_szTag.GetLength())  &&
			sLine.Mid(nBeginIndex, singleComment.m_szTag.GetLength()) == singleComment.m_szTag)
		{
			return TRUE;
        }
	}
	return FALSE;
}

int  CLangGrammar::GetMultiLineCommentStartIndex(const CString& sLine, int nBeginIndex)
{
	int i, nCount = m_multiCommentArray.GetSize();
	
	CMultiLineComment multiComment;
	for(i = 0; i < nCount; i++)
	{
		multiComment = m_multiCommentArray.GetAt(i);
		
		if (sLine.GetLength() >= (nBeginIndex + multiComment.m_szStart.GetLength())  &&
			sLine.Mid(nBeginIndex, multiComment.m_szStart.GetLength()) == multiComment.m_szStart)
		{
			return i;
        }
	}
	return -1;
}

BOOL CLangGrammar::IsMultiLineCommentEnd(int iIndexOfMultiComment, const CString& sLine, int nBeginIndex)
{
	ASSERT(iIndexOfMultiComment >= 0 && iIndexOfMultiComment < m_multiCommentArray.GetSize());

	CMultiLineComment multiComment = m_multiCommentArray.GetAt(iIndexOfMultiComment);

	return IsStartsWith(sLine, multiComment.m_szEnd, nBeginIndex);
}

int  CLangGrammar::IndexOfEscStr(const CString& sLine, int nBeginIndex)
{
	int i, nCount = m_escapeStrArray.GetSize();
	
	CString szEsc;
	for(i = 0; i < nCount; i++)
	{
		szEsc = m_escapeStrArray.GetAt(i);
		
		if(IsStartsWith(sLine, szEsc, nBeginIndex))
		{
			return i;
		}
	}
	return -1;
}

int  CLangGrammar::GetStringStartIndex(const CString& sLine, int nBeginIndex)
{
	int i, nCount = m_strMarkArray.GetSize();
	
	CPair szPair;
	for(i = 0; i < nCount; i++)
	{
		szPair = m_strMarkArray.GetAt(i);
		
		if(IsStartsWith(sLine, szPair.m_szStart, nBeginIndex))
		{
			return i;
		}
	}
	return -1;
}

BOOL CLangGrammar::IsStringEnd(int iStrIndex, const CString& sLine, int nBeginIndex)
{
	ASSERT(iStrIndex >= 0 && iStrIndex < m_strMarkArray.GetSize());
	
	CPair strPair = m_strMarkArray.GetAt(iStrIndex);
	
	return IsStartsWith(sLine, strPair.m_szEnd, nBeginIndex);
}