#ifndef _LANG_GRAMMAR_H_
#define _LANG_GRAMMAR_H_


#if _MSC_VER > 1000
#pragma once
#endif

#include "TinyVector.h"

#define START_COLUMN_ANY	(-1)
//SOL -- Start of Line
#define START_COLUMN_SOL	(0)

//#define USE_TIXML

#if defined(TIXML_USE_STL)
#include <string>
#define LG_STRING	std::string
#elif defined(USE_TIXML)
#include "tinystr.h"
#define LG_STRING	TiXmlString
#else
#define LG_STRING CString
#endif

int GetLength(const LG_STRING& str);
char GetAt(const LG_STRING& str, int nIndex);

class CSingleLineComment
{
public:
	CSingleLineComment(const char* lpszCommentStr = NULL, int nColumn = START_COLUMN_ANY);
public:
	LG_STRING m_szTag;
	int m_nStartCol;
};

class CPair
{
public:
	CPair();
	CPair(const char* lpszStart, const char* lpszEnd);
public:
	LG_STRING m_szStart;
	LG_STRING m_szEnd;
};

typedef CPair CMultiLineComment;

class CLangGrammar
{
public:
	CLangGrammar();
	~CLangGrammar();
	
	BOOL IsSingleLineComment(const CString& sLine, int nBeginIndex = 0);
	int  IndexOfEscStr(const CString& sLine, int nBeginIndex = 0);
	
	int  GetMultiLineCommentStartIndex(const CString& sLine, int nBeginIndex = 0);
	BOOL IsMultiLineCommentEnd(int iIndexOfMultiComment, const CString& sLine, int nBeginIndex = 0);
	
	int  GetStringStartIndex(const CString& sLine, int nBeginIndex = 0);
	BOOL IsStringEnd(int iStrIndex, const CString& sLine, int nBeginIndex = 0);
	
	int  GetCharStartIndex(const CString& sLine, int nBeginIndex = 0);
	BOOL IsCharEnd(int iStrIndex, const CString& sLine, int nBeginIndex = 0);
public:
	static int  GetStartIndex(const CTinyVector<CPair>& tinyVector, const CString& sLine, int nBeginIndex = 0);
	static BOOL IsEnd(const CTinyVector<CPair>& tinyVector, int iIndex, const CString& sLine, int nBeginIndex = 0);
	static BOOL IsStartsWith(const CString& sSrc, const LG_STRING& sPrefix, int nBeginIndex = 0);

// 	CArray<CSingleLineComment, CSingleLineComment&> m_singleCommentArray;
// 	CArray<CMultiLineComment, CMultiLineComment&> m_multiCommentArray;
// 	CArray<CPair, CPair&> m_stringMarkArray;	// eg. "string"
// 	CArray<CPair, CPair&> m_charMarkArray;		// eg. 'A'
// 	CArray<LG_STRING, LG_STRING&> m_escapeStrArray;	

	CTinyVector<CSingleLineComment> m_singleCommentArray;
	CTinyVector<CMultiLineComment> m_multiCommentArray;
	CTinyVector<CPair> m_stringMarkArray;	// eg. "string"
	CTinyVector<CPair> m_charMarkArray;		// eg. 'A'
	CTinyVector<LG_STRING> m_escapeStrArray;	
};

#endif // _LANG_GRAMMAR_H_
