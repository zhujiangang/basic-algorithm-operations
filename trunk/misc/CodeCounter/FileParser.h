#ifndef _FILE_PARSER_H_
#define _FILE_PARSER_H_

#pragma once

#define MASK_TOTAL_LINE		0x00000000
#define MASK_CODE_LINE		0x00000001
#define MASK_COMMENT_LINE	0x00000002
#define MASK_BLANK_LINE		0x00000004
#define MASK_MIXED_LINE		0x00000008

#include "TinyVector.h"

class CFileInfo
{
public:
	CFileInfo();
	void SetFileName(LPCTSTR lpszFullFileName);
	UINT GetMixedLines() const;
	void Increase(DWORD dwFlags);
	bool operator==(const CFileInfo& other) const;
public:
	CString m_sFullFileName;
	
    CString m_sFilePath;
    CString m_sFileName;
    CString m_sFileExt;
	
	UINT m_nTotalLines;
	UINT m_nCodeLines;
	UINT m_nCommentLines;
	UINT m_nBlankLines;
	UINT m_nMixedLines;
};

class CTotalInfo
{
public:
	CTotalInfo();
	void Reset();
	UINT GetTotalMixedLines() const;
public:
	UINT m_nTotalCount;
	UINT m_nTotalLines;
	UINT m_nTotalCodeLines;
	UINT m_nTotalCommentLines;
	UINT m_nTotalBlankLines;
	UINT m_nTotalMixedLines;
};

#define START_COLUMN_ANY	(-1)
//SOL -- Start of Line
#define START_COLUMN_SOL	(0)

class CSingleLineComment
{
public:
	CSingleLineComment(LPCTSTR lpszCommentStr = NULL, int nColumn = START_COLUMN_ANY);
public:
	CString m_szTag;
	int m_nStartCol;
};

class CPair
{
public:
	CPair();
	CPair(LPCTSTR lpszStart, LPCTSTR lpszEnd);
public:
	CString m_szStart;
	CString m_szEnd;
};

typedef CPair CMultiLineComment;

class CLangGrammar
{
public:
	CLangGrammar();
	~CLangGrammar();
	BOOL IsSingleLineComment(const CString& sLine, int nBeginIndex = 0);
	int  GetMultiLineCommentStartIndex(const CString& sLine, int nBeginIndex = 0);
	BOOL IsMultiLineCommentEnd(int iIndexOfMultiComment, const CString& sLine, int nBeginIndex = 0);
	int  IndexOfEscStr(const CString& sLine, int nBeginIndex = 0);
	int  GetStringStartIndex(const CString& sLine, int nBeginIndex = 0);
	BOOL IsStringEnd(int iStrIndex, const CString& sLine, int nBeginIndex = 0);
	int  GetCharStartIndex(const CString& sLine, int nBeginIndex = 0);
	BOOL IsCharEnd(int iStrIndex, const CString& sLine, int nBeginIndex = 0);
	static BOOL IsStartsWith(const CString& sSrc, const CString& sPrefix, int nBeginIndex = 0);
public:
// 	CArray<CSingleLineComment, CSingleLineComment&> m_singleCommentArray;
// 	CArray<CMultiLineComment, CMultiLineComment&> m_multiCommentArray;
// 	CArray<CPair, CPair&> m_stringMarkArray;	// eg. "string"
// 	CArray<CPair, CPair&> m_charMarkArray;		// eg. 'A'
// 	CStringArray m_escapeStrArray;

	CTinyVector<CSingleLineComment> m_singleCommentArray;
	CTinyVector<CMultiLineComment> m_multiCommentArray;
	CTinyVector<CPair> m_stringMarkArray;	// eg. "string"
	CTinyVector<CPair> m_charMarkArray;		// eg. 'A'
	CTinyVector<CString> m_escapeStrArray;	
};

class IFileParser
{
public:
	IFileParser(CFileInfo* pFileInfo = NULL) : m_pFileInfo(pFileInfo) {};
    virtual ~IFileParser() {};
	
    virtual void ParseFile() = 0;
protected:
	CFileInfo* m_pFileInfo;
};

#endif