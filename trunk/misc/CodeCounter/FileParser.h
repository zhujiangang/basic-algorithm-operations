#ifndef _FILE_PARSER_H_
#define _FILE_PARSER_H_

#pragma once

#define MASK_TOTAL_LINE		0x00000000
#define MASK_CODE_LINE		0x00000001
#define MASK_COMMENT_LINE	0x00000002
#define MASK_BLANK_LINE		0x00000004
#define MASK_MIXED_LINE		0x00000008

class CFileInfo
{
public:
	CFileInfo();
	void SetFileName(LPCTSTR lpszFullFileName);
	UINT GetMixedLines() const;
	void Increase(DWORD dwFlags);
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

class CPair
{
public:
	CPair();
	CPair(LPCTSTR lpszStart, LPCTSTR lpszEnd);
public:
	CString m_szStart;
	CString m_szEnd;
};

class CLangGrammar
{
public:
	CLangGrammar();
	~CLangGrammar();
public:
	CStringArray m_singleCommentArray;
	CStringArray m_escapeCharArray;
	CArray<CPair, CPair&> m_multiCommentArray;
	CArray<CPair, CPair&> m_strMarkArray;
};

class IFileParser
{
public:
    IFileParser() {};
    virtual ~IFileParser() {};
	
    virtual void ParseFile() = 0;
};

#endif