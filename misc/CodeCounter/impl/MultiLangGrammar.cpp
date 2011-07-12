#include "StdAfx.h"
#include "MultiLangGrammar.h"

int  CMultiLangGrammar::GetStartIndex(const CTinyVector<CPair>& tinyVector, const CString& sLine, int nBeginIndex)
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
BOOL CMultiLangGrammar::IsEnd(const CTinyVector<CPair>& tinyVector, int iIndex, const CString& sLine, int nBeginIndex)
{
	if(iIndex < 0 || iIndex >= (int)tinyVector.GetSize())
	{
		return FALSE;
	}	
	return IsStartsWith(sLine, tinyVector.GetAt(iIndex).m_szEnd, nBeginIndex);
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

int  CMultiLangGrammar::GetCountOfSingleLineComment()
{
	return m_singleCommentArray.GetSize();
}
int  CMultiLangGrammar::GetCountOfEscStr()
{
	return m_escapeStrArray.GetSize();
}
int  CMultiLangGrammar::GetCountOfMultiLineComment()
{
	return m_multiCommentArray.GetSize();
}
int  CMultiLangGrammar::GetCountOfStringMark()
{
	return m_stringMarkArray.GetSize();
}
int  CMultiLangGrammar::GetCountOfCharMark()
{
	return m_charMarkArray.GetSize();
}