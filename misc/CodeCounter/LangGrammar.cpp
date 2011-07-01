#include "stdafx.h"
#include "LangGrammar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int GetLength(const LG_STRING& str)
{
#if defined(TIXML_USE_STL)
	return str.length();
#elif defined(USE_TIXML)
	return str.length();
#else
	return str.GetLength();
#endif
}

char GetAt(const LG_STRING& str, int nIndex)
{
#if defined(TIXML_USE_STL)
	return str.at(nIndex);
#elif defined(USE_TIXML)
	return str.at(nIndex);
#else
	return str.GetAt(nIndex);
#endif
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
	m_stringMarkArray.RemoveAll();
	m_charMarkArray.RemoveAll();
	m_escapeStrArray.RemoveAll();
}
BOOL CLangGrammar::IsStartsWith(const CString& sSrc, const LG_STRING& sPrefix, int nBeginIndex)
{
	int nPrefixLen = GetLength(sPrefix);
	if(nBeginIndex + nPrefixLen > sSrc.GetLength())
	{
		return FALSE;
	}
	for(int i = 0; i < nPrefixLen; i++)
	{
		if(GetAt(sPrefix, i) != sSrc.GetAt(nBeginIndex + i))
		{
			return FALSE;
		}
	}
	return TRUE;
}
int  CLangGrammar::GetStartIndex(const CTinyVector<CPair>& tinyVector, const CString& sLine, int nBeginIndex)
{
	int nCount = tinyVector.GetSize();
	if(nCount <= 0)
	{
		return -1;
	}
	for(int i = 0; i < nCount; i++)
	{
		if(IsStartsWith(sLine, tinyVector.GetAt(i).m_szStart, nBeginIndex))
		{
			return i;
		}
	}
	return -1;
}
BOOL CLangGrammar::IsEnd(const CTinyVector<CPair>& tinyVector, int iIndex, const CString& sLine, int nBeginIndex)
{
	if(iIndex < 0 || iIndex >= tinyVector.GetSize())
	{
		return FALSE;
	}	
	return IsStartsWith(sLine, tinyVector.GetAt(iIndex).m_szEnd, nBeginIndex);
}

BOOL CLangGrammar::IsSingleLineComment(const CString& sLine, int nBeginIndex)
{
	int nCount = m_singleCommentArray.GetSize();
	if(nCount <= 0)
	{
		return FALSE;
	}

	for(int i = 0; i < nCount; i++)
	{
		CSingleLineComment& singleComment = m_singleCommentArray.GetAt(i);

		//SOL
		if(singleComment.m_nStartCol == START_COLUMN_SOL)
		{
			if(nBeginIndex != 0)
			{
				continue;
			}
		}

		//TODO: concern about the case "singleComment.m_nStartCol > 0"
		if(IsStartsWith(sLine, singleComment.m_szTag, nBeginIndex))
		{
			return TRUE;
		}
	}
	return FALSE;
}
int  CLangGrammar::IndexOfEscStr(const CString& sLine, int nBeginIndex)
{
	int nCount = m_escapeStrArray.GetSize();
	if(nCount <= 0)
	{
		return -1;
	}
	
	for(int i = 0; i < nCount; i++)
	{
		LG_STRING& szEsc = m_escapeStrArray.GetAt(i);
		
		if(IsStartsWith(sLine, szEsc, nBeginIndex))
		{
			return i;
		}
	}
	return -1;
}

int  CLangGrammar::GetMultiLineCommentStartIndex(const CString& sLine, int nBeginIndex)
{
	return GetStartIndex(m_multiCommentArray, sLine, nBeginIndex);
}

BOOL CLangGrammar::IsMultiLineCommentEnd(int iIndexOfMultiComment, const CString& sLine, int nBeginIndex)
{
	return IsEnd(m_multiCommentArray, iIndexOfMultiComment, sLine, nBeginIndex);
}

int  CLangGrammar::GetStringStartIndex(const CString& sLine, int nBeginIndex)
{
	return GetStartIndex(m_stringMarkArray, sLine, nBeginIndex);
}

BOOL CLangGrammar::IsStringEnd(int iStrIndex, const CString& sLine, int nBeginIndex)
{
	return IsEnd(m_stringMarkArray, iStrIndex, sLine, nBeginIndex);
}

int  CLangGrammar::GetCharStartIndex(const CString& sLine, int nBeginIndex)
{
	return GetStartIndex(m_charMarkArray, sLine, nBeginIndex);
}
BOOL CLangGrammar::IsCharEnd(int iStrIndex, const CString& sLine, int nBeginIndex)
{
	return IsEnd(m_charMarkArray, iStrIndex, sLine, nBeginIndex);
}