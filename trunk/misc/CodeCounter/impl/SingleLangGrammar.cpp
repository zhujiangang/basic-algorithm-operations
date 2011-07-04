#include "StdAfx.h"
#include "SingleLangGrammar.h"

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