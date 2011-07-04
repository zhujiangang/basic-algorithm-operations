#include "stdafx.h"
#include "LangGrammar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int GetLength(const LG_STRING& str)
{
#if defined(STRING_USE_STL)
	return str.length();
#else
	return str.GetLength();
#endif
}

char GetAt(const LG_STRING& str, int nIndex)
{
#if defined(STRING_USE_STL)
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

BOOL ILangGrammar::IsStartsWith(const CString& sSrc, const LG_STRING& sPrefix, int nBeginIndex)
{
	int nPrefixLen = GetLength(sPrefix);
	if( (nPrefixLen <= 0) || ((nBeginIndex + nPrefixLen) > sSrc.GetLength()) )
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
int  ILangGrammar::GetStartIndex(const CTinyVector<CPair>& tinyVector, const CString& sLine, int nBeginIndex)
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
BOOL ILangGrammar::IsEnd(const CTinyVector<CPair>& tinyVector, int iIndex, const CString& sLine, int nBeginIndex)
{
	if(iIndex < 0 || iIndex >= tinyVector.GetSize())
	{
		return FALSE;
	}	
	return IsStartsWith(sLine, tinyVector.GetAt(iIndex).m_szEnd, nBeginIndex);
}

BOOL ILangGrammar::IsSingleComment(const CSingleLineComment& sComment, const CString& sLine, int nBeginIndex)
{
	if(sComment.m_szTag.IsEmpty())
	{
		return FALSE;
	}
	//SOL
	if(sComment.m_nStartCol == START_COLUMN_SOL && nBeginIndex != 0)
	{
		return FALSE;
	}
	
	//TODO: concern about the case "singleComment.m_nStartCol > 0"
	if(IsStartsWith(sLine, sComment.m_szTag, nBeginIndex))
	{
		return TRUE;
	}
	return FALSE;
}


CSingleLangGrammar::CSingleLangGrammar()
{
}

CSingleLangGrammar::~CSingleLangGrammar()
{

}

CSingleLineComment& CSingleLangGrammar::GetSingleLineComment(int nIndex /* = 0 */)
{
	return m_singleComment;
}
BOOL CSingleLangGrammar::IsSingleLineComment(const CString& sLine, int nBeginIndex)
{
	return IsSingleComment(m_singleComment, sLine, nBeginIndex);
}

LG_STRING& CSingleLangGrammar::GetEscapeStr(int nIndex /* = 0 */)
{
	return m_escapeStr;
}
int  CSingleLangGrammar::IndexOfEscStr(const CString& sLine, int nBeginIndex)
{
	if(IsStartsWith(sLine, m_escapeStr, nBeginIndex))
	{
		return 0;
	}
	return -1;
}

CMultiLineComment& CSingleLangGrammar::GetMultiLineComment(int nIndex /* = 0 */)
{
	return m_multiComment;
}
int  CSingleLangGrammar::GetMultiLineCommentStartIndex(const CString& sLine, int nBeginIndex)
{
	if(IsStartsWith(sLine, m_multiComment.m_szStart, nBeginIndex))
	{
		return 0;
	}
	return -1;
}
BOOL CSingleLangGrammar::IsMultiLineCommentEnd(int iIndexOfMultiComment, const CString& sLine, int nBeginIndex)
{
	if(IsStartsWith(sLine, m_multiComment.m_szEnd, nBeginIndex))
	{
		return TRUE;
	}
	return FALSE;
}

CPair& CSingleLangGrammar::GetStringMark(int nIndex /* = 0 */)
{
	return m_stringMark;
}
int  CSingleLangGrammar::GetStringStartIndex(const CString& sLine, int nBeginIndex)
{
	if(IsStartsWith(sLine, m_stringMark.m_szStart, nBeginIndex))
	{
		return 0;
	}
	return -1;
}
BOOL CSingleLangGrammar::IsStringEnd(int iStrIndex, const CString& sLine, int nBeginIndex)
{
	if(IsStartsWith(sLine, m_stringMark.m_szEnd, nBeginIndex))
	{
		return TRUE;
	}
	return FALSE;
}

CPair& CSingleLangGrammar::GetCharMark(int iCharIndex)
{
	return m_charMark;
}
int  CSingleLangGrammar::GetCharStartIndex(const CString& sLine, int nBeginIndex)
{
	if(IsStartsWith(sLine, m_charMark.m_szStart, nBeginIndex))
	{
		return 0;
	}
	return -1;
}
BOOL CSingleLangGrammar::IsCharEnd(int iStrIndex, const CString& sLine, int nBeginIndex)
{
	if(IsStartsWith(sLine, m_charMark.m_szEnd, nBeginIndex))
	{
		return TRUE;
	}
	return FALSE;
}

CMultiLangGrammar::CMultiLangGrammar()
{
}
CMultiLangGrammar::~CMultiLangGrammar()
{
	m_singleCommentArray.RemoveAll();
	m_multiCommentArray.RemoveAll();
	m_stringMarkArray.RemoveAll();
	m_charMarkArray.RemoveAll();
	m_escapeStrArray.RemoveAll();
}

CSingleLineComment& CMultiLangGrammar::GetSingleLineComment(int nIndex /* = 0 */)
{
	return m_singleCommentArray.GetAt(nIndex);
}
BOOL CMultiLangGrammar::IsSingleLineComment(const CString& sLine, int nBeginIndex)
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

LG_STRING& CMultiLangGrammar::GetEscapeStr(int nIndex /* = 0 */)
{
	return m_escapeStrArray.GetAt(nIndex);
}
int  CMultiLangGrammar::IndexOfEscStr(const CString& sLine, int nBeginIndex)
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

CMultiLineComment& CMultiLangGrammar::GetMultiLineComment(int nIndex /* = 0 */)
{
	return m_multiCommentArray.GetAt(nIndex);
}
int  CMultiLangGrammar::GetMultiLineCommentStartIndex(const CString& sLine, int nBeginIndex)
{
	return GetStartIndex(m_multiCommentArray, sLine, nBeginIndex);
}

BOOL CMultiLangGrammar::IsMultiLineCommentEnd(int iIndexOfMultiComment, const CString& sLine, int nBeginIndex)
{
	return IsEnd(m_multiCommentArray, iIndexOfMultiComment, sLine, nBeginIndex);
}

CPair& CMultiLangGrammar::GetStringMark(int nIndex /* = 0 */)
{
	return m_stringMarkArray.GetAt(nIndex);
}
int  CMultiLangGrammar::GetStringStartIndex(const CString& sLine, int nBeginIndex)
{
	return GetStartIndex(m_stringMarkArray, sLine, nBeginIndex);
}

BOOL CMultiLangGrammar::IsStringEnd(int iStrIndex, const CString& sLine, int nBeginIndex)
{
	return IsEnd(m_stringMarkArray, iStrIndex, sLine, nBeginIndex);
}

CPair& CMultiLangGrammar::GetCharMark(int nIndex /* = 0 */)
{
	return m_charMarkArray.GetAt(nIndex);
}
int  CMultiLangGrammar::GetCharStartIndex(const CString& sLine, int nBeginIndex)
{
	return GetStartIndex(m_charMarkArray, sLine, nBeginIndex);
}
BOOL CMultiLangGrammar::IsCharEnd(int iStrIndex, const CString& sLine, int nBeginIndex)
{
	return IsEnd(m_charMarkArray, iStrIndex, sLine, nBeginIndex);
}

CLangGrammarBuilder::CLangGrammarBuilder(int type /* = LG_TYPE_SINGLE */)
 : m_pSingleLangGrammar(NULL), m_pMultiLangGrammar(NULL)
{
	if(type == LG_TYPE_MULTI)
	{
		m_pMultiLangGrammar = new CMultiLangGrammar();
	}
	else
	{
		m_pSingleLangGrammar = new CSingleLangGrammar();
	}
}

ILangGrammar* CLangGrammarBuilder::GetResult()
{
	if(m_pSingleLangGrammar != NULL)
	{
		return m_pSingleLangGrammar;
	}
	else
	{
		return m_pMultiLangGrammar;
	}
}

void CLangGrammarBuilder::AddSingleComment(const char* lpszCommentStr, int nColumn /* = START_COLUMN_ANY */)
{
	if(m_pSingleLangGrammar != NULL)
	{
		if(GetLength(m_pSingleLangGrammar->m_singleComment.m_szTag) <= 0)
		{
			m_pSingleLangGrammar->m_singleComment.m_szTag = lpszCommentStr;
			m_pSingleLangGrammar->m_singleComment.m_nStartCol = nColumn;
			return;
		}
		//Change to multi
		delete m_pSingleLangGrammar;
		m_pSingleLangGrammar = NULL;
	}
	CSingleLineComment sComment(lpszCommentStr, nColumn);
	m_pMultiLangGrammar->m_singleCommentArray.Add(sComment);
}

void CLangGrammarBuilder::AddMultiComment(const char* lpszStart, const char* lpszEnd)
{
	if(m_pSingleLangGrammar != NULL)
	{
		if(GetLength(m_pSingleLangGrammar->m_multiComment.m_szStart) <= 0)
		{
			m_pSingleLangGrammar->m_multiComment.m_szStart = lpszStart;
			m_pSingleLangGrammar->m_multiComment.m_szEnd = lpszEnd;
			return;
		}
		//Change to multi
		delete m_pSingleLangGrammar;
		m_pSingleLangGrammar = NULL;
	}
	CMultiLineComment mComment(lpszStart, lpszEnd);
	m_pMultiLangGrammar->m_multiCommentArray.Add(mComment);
}

void CLangGrammarBuilder::AddStringMark(const char* lpszStart, const char* lpszEnd)
{
	if(m_pSingleLangGrammar != NULL)
	{
		if(GetLength(m_pSingleLangGrammar->m_stringMark.m_szStart) <= 0)
		{
			m_pSingleLangGrammar->m_stringMark.m_szStart = lpszStart;
			m_pSingleLangGrammar->m_stringMark.m_szEnd = lpszEnd;
			return;
		}
		//Change to multi
		delete m_pSingleLangGrammar;
		m_pSingleLangGrammar = NULL;
	}
	CPair stringMark(lpszStart, lpszEnd);
	m_pMultiLangGrammar->m_stringMarkArray.Add(stringMark);
}

void CLangGrammarBuilder::AddCharMark(const char* lpszStart, const char* lpszEnd)
{
	if(m_pSingleLangGrammar != NULL)
	{
		if(GetLength(m_pSingleLangGrammar->m_charMark.m_szStart) <= 0)
		{
			m_pSingleLangGrammar->m_charMark.m_szStart = lpszStart;
			m_pSingleLangGrammar->m_charMark.m_szEnd = lpszEnd;
			return;
		}
		//Change to multi
		delete m_pSingleLangGrammar;
		m_pSingleLangGrammar = NULL;
	}
	CPair charMark(lpszStart, lpszEnd);
	m_pMultiLangGrammar->m_charMarkArray.Add(charMark);
}

void CLangGrammarBuilder::AddEscapeStr(const char* lpszEscapeStr)
{
	if(m_pSingleLangGrammar != NULL)
	{
		if(GetLength(m_pSingleLangGrammar->m_escapeStr) <= 0)
		{
			m_pSingleLangGrammar->m_escapeStr = lpszEscapeStr;
			return;
		}
		//Change to multi
		delete m_pSingleLangGrammar;
		m_pSingleLangGrammar = NULL;
	}
	m_pMultiLangGrammar->m_escapeStrArray.Add(lpszEscapeStr);
}