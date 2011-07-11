#ifndef _MULTI_LANG_GRAMMAR_H_
#define _MULTI_LANG_GRAMMAR_H_

#pragma once

#include "LangGrammar.h"
#include "TinyVector.h"

class CMultiLangGrammar : public ILangGrammar
{
public:
	CMultiLangGrammar();
	~CMultiLangGrammar();
	
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

	static int  GetStartIndex(const CTinyVector<CPair>& tinyVector, const CString& sLine, int nBeginIndex = 0);
	static BOOL IsEnd(const CTinyVector<CPair>& tinyVector, int iIndex, const CString& sLine, int nBeginIndex = 0);
public:
	CTinyVector<CSingleLineComment> m_singleCommentArray;
	CTinyVector<CMultiLineComment> m_multiCommentArray;
	CTinyVector<CPair> m_stringMarkArray;	// eg. "string"
	CTinyVector<CPair> m_charMarkArray;		// eg. 'A'
	CTinyVector<LG_STRING> m_escapeStrArray;	
};

#endif