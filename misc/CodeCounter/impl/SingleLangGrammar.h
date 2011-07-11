#ifndef _SINGLE_LANG_GRAMMAR_H_
#define _SINGLE_LANG_GRAMMAR_H_

#pragma once

#include "LangGrammar.h"

class CSingleLangGrammar : public ILangGrammar
{
public:
	CSingleLangGrammar();
	~CSingleLangGrammar();
	
	CSingleLineComment& GetSingleLineComment(int nIndex = 0);
	BOOL IsSingleLineComment(const CString& sLine, int nBeginIndex = 0);
	
	LG_STRING& GetEscapeStr(int nIndex = 0);
	int  IndexOfEscStr(const CString& sLine, int nBeginIndex = 0);
	int  GetCountOfEscStr();
	
	CMultiLineComment& GetMultiLineComment(int nIndex = 0);
	int  GetMultiLineCommentStartIndex(const CString& sLine, int nBeginIndex = 0);
	BOOL IsMultiLineCommentEnd(int iIndexOfMultiComment, const CString& sLine, int nBeginIndex = 0);
	int  GetCountOfMultiLineComment();
	
	CPair& GetStringMark(int nIndex = 0);
	int  GetStringStartIndex(const CString& sLine, int nBeginIndex = 0);
	BOOL IsStringEnd(int iStrIndex, const CString& sLine, int nBeginIndex = 0);
	int  GetCountOfStringMark();
	
	CPair& GetCharMark(int nIndex = 0);
	int  GetCharStartIndex(const CString& sLine, int nBeginIndex = 0);
	BOOL IsCharEnd(int iStrIndex, const CString& sLine, int nBeginIndex = 0);
	int  GetCountOfCharMark();
public:
	CSingleLineComment m_singleComment;
	CMultiLineComment  m_multiComment;
	CPair m_stringMark;
	CPair m_charMark;
	LG_STRING m_escapeStr;
};

#endif