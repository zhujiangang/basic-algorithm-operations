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
	CFileInfo(LPCTSTR lpszFullFileName = NULL);
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

#define FP_MODE_BLANK_IN_COMMENT_BLOCK_COMMENT			0x00000001
#define FP_MODE_MIXED_LINE_CODE							0x00000002
#define FP_MODE_MIXED_LINE_COMMENT						0x00000004
#define FP_MODE_STRING_IN_MULTI_LINE					0x00000008
#define FP_MODE_BLANK_IN_MULTI_LINE_STRING_AS_BLANK		0x00000010

#define FP_MODE_DEFAULT									0x000000FF

class CBaseLogger;
class IFileParser
{
public:
	IFileParser(CFileInfo* pFileInfo = NULL, DWORD nMode = FP_MODE_DEFAULT, LPCTSTR lpLogFileName = NULL);
    virtual ~IFileParser();
	
	void SetFileInfo(CFileInfo* pFileInfo)
	{
		m_pFileInfo = pFileInfo;
	}

	virtual void ParseFile() = 0;

	void SetLogger(LPCTSTR lpLogFileName);
	DWORD GetMode() const
	{
		return m_nMode;
	}
	void SetMode(DWORD nMode)
	{
		m_nMode = nMode;
	}
protected:
	virtual void Increase(DWORD dwFlags);
	virtual void CountBlankLineInCommentBlock();
	virtual void CountBlankLineInMultiString();
	virtual void CountCodeCommentInOneLine();
protected:
	CFileInfo* m_pFileInfo;
	CBaseLogger* m_pLogger;
	DWORD	m_nMode;
public:
	static BOOL IsSpace(int ch);
};

#endif