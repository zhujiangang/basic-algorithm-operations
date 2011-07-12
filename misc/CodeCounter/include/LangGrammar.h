#ifndef _LANG_GRAMMAR_H_
#define _LANG_GRAMMAR_H_

#pragma once

#if defined(STRING_USE_STL)
#include <string>
#define LG_STRING	std::string
#else
#define LG_STRING	CString
#endif


int  GetLength(const LG_STRING& str);
char GetAt(const LG_STRING& str, int nIndex);


#define START_COLUMN_ANY	(-1)	//ANY -- Any Column of Line
#define START_COLUMN_SOL	(0)		//SOL -- Start of Line

class CSingleLineComment
{
public:
	CSingleLineComment(const char* lpszCommentStr = NULL, int nColumn = START_COLUMN_ANY, BOOL bCaseSensitive = TRUE);
public:
	LG_STRING m_szTag;
	int m_nStartCol;
	BOOL m_bCaseSensitive;
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

class ILangGrammar
{
public:
	virtual ~ILangGrammar() {};
	virtual	CSingleLineComment& GetSingleLineComment(int nIndex = 0) = 0;
	virtual BOOL IsSingleLineComment(const CString& sLine, int nBeginIndex = 0) = 0;
	virtual int  GetCountOfSingleLineComment() = 0;

	virtual LG_STRING& GetEscapeStr(int nIndex = 0) = 0;
	virtual int  IndexOfEscStr(const CString& sLine, int nBeginIndex = 0) = 0;
	virtual int  GetCountOfEscStr() = 0;
	
	virtual CMultiLineComment& GetMultiLineComment(int nIndex = 0) = 0;
	virtual int  GetMultiLineCommentStartIndex(const CString& sLine, int nBeginIndex = 0) = 0;
	virtual BOOL IsMultiLineCommentEnd(int iIndexOfMultiComment, const CString& sLine, int nBeginIndex = 0) = 0;
	virtual int  GetCountOfMultiLineComment() = 0;
	
	virtual CPair& GetStringMark(int nIndex = 0) = 0;
	virtual int  GetStringStartIndex(const CString& sLine, int nBeginIndex = 0) = 0;
	virtual BOOL IsStringEnd(int iStrIndex, const CString& sLine, int nBeginIndex = 0) = 0;
	virtual int  GetCountOfStringMark() = 0;
	
	virtual CPair& GetCharMark(int iCharIndex = 0) = 0;
	virtual int  GetCharStartIndex(const CString& sLine, int nBeginIndex = 0) = 0;
	virtual BOOL IsCharEnd(int iStrIndex, const CString& sLine, int nBeginIndex = 0) = 0;
	virtual int  GetCountOfCharMark() = 0;

	static BOOL IsStartsWith(const CString& sSrc, const LG_STRING& sPrefix, int nBeginIndex = 0);
	static BOOL IsSingleComment(const CSingleLineComment& sComment, const CString& sLine, int nBeginIndex = 0);
};


#define LG_TYPE_SINGLE		0
#define LG_TYPE_MULTI		1

class CSingleLangGrammar;
class CMultiLangGrammar;
class CLangGrammarBuilder
{
public:
	CLangGrammarBuilder(int type = LG_TYPE_SINGLE);
	ILangGrammar* GetResult();
	void AddSingleComment(const char* lpszCommentStr, int nColumn = START_COLUMN_ANY, BOOL bCaseSensitive = TRUE);
	void AddMultiComment(const char* lpszStart, const char* lpszEnd);
	void AddStringMark(const char* lpszStart, const char* lpszEnd);
	void AddCharMark(const char* lpszStart, const char* lpszEnd);
	void AddEscapeStr(const char* lpszEscapeStr);
protected:
	CSingleLangGrammar* m_pSingleLangGrammar;
	CMultiLangGrammar*  m_pMultiLangGrammar;
};

#endif // _LANG_GRAMMAR_H_
