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

enum
{
	STATE_NORMAL = 0,
	STATE_STRING = 1,
	STATE_MULTI_COMMENT = 2
};
class CFsmState
{
public:
	CFsmState() : m_nMajorState(STATE_NORMAL), m_nMinorState(-1)
	{
	}
public:
	int m_nMajorState;		//STATE_NORMAL, STATE_STRING, STATE_MULTI_COMMENT
	int m_nMinorState;		//Detailed information about m_nMajorState;
};
typedef CFsmState ParseState;

//The lowest 8 bits used for stat parameters
#define FP_MODE_BLANK_IN_COMMENT_BLOCK_COMMENT			0x00000001
#define FP_MODE_MIXED_LINE_CODE							0x00000002
#define FP_MODE_MIXED_LINE_COMMENT						0x00000004
#define FP_MODE_STRING_IN_MULTI_LINE					0x00000008
#define FP_MODE_BLANK_IN_MULTI_LINE_STRING_AS_BLANK		0x00000010

//The low 9-16 bits used for other parameters
#define FP_MODE_LOG_RESULT								0x00000100
#define FP_MODE_LOG_STRING_IN_MULTI_LINE				0x00000200
#define FP_MODE_LOG_ALL									0x0000FF00

#define FP_MODE_DEFAULT									0x000000FF


#if defined(_DEBUG)
#define REPORT_EXCEPTION_ERROR(e)	e->ReportError()
#else
#define REPORT_EXCEPTION_ERROR(e)
#endif

class CByteArrayEx;
class CUIntArrayEx;

class IFileParser
{
public:
	IFileParser(CFileInfo* pFileInfo = NULL, DWORD nMode = FP_MODE_DEFAULT);
    virtual ~IFileParser();
	virtual void ParseFile();
public:
	void SetFileInfo(CFileInfo* pFileInfo)
	{
		m_pFileInfo = pFileInfo;
	}
	DWORD GetMode() const
	{
		return m_nMode;
	}
	void SetMode(DWORD nMode);
protected:
	virtual void ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments);
	virtual void Increase(DWORD dwFlags);
	virtual void CountBlankLineInCommentBlock();
	virtual void CountBlankLineInMultiString();
	virtual void CountCodeCommentInOneLine();
protected:
	CFileInfo* m_pFileInfo;
	DWORD	m_nMode;
	CByteArrayEx* m_pLogResult;
	CUIntArrayEx* m_pLogStringInMultiLine;
public:
	static BOOL IsSpace(int ch);
	static TCHAR ToChar(BYTE bInt);
};

enum ELangType
{
	LANG_TYPE_START = 0,
	LANG_TYPE_ADA = 1,
	LANG_TYPE_ASSEMBLY = 2,
	LANG_TYPE_ASP = 3,
	LANG_TYPE_BASIC = 101,
	LANG_TYPE_BAT = 102,
	LANG_TYPE_C = 201,
	LANG_TYPE_COBOL = 202,
	LANG_TYPE_CPP = 203,
	LANG_TYPE_CSHARP = 204,
	LANG_TYPE_CSS = 205,
	LANG_TYPE_DELPHI = 301,
	LANG_TYPE_FORTRAN = 501,
	LANG_TYPE_HTML = 701,
	LANG_TYPE_INI = 801,
	LANG_TYPE_JAVA = 901,
	LANG_TYPE_JAVASCRIPT = 902,
	LANG_TYPE_LISP = 1101,
	LANG_TYPE_LUA = 1102,
	LANG_TYPE_MAK = 1201,
	LANG_TYPE_PASCAL = 1501,
	LANG_TYPE_PERL = 1502,
	LANG_TYPE_PHP = 1503,
	LANG_TYPE_PYTHON = 1504,
	LANG_TYPE_RUBY = 1701,
	LANG_TYPE_SCALA = 1801,
	LANG_TYPE_SQL = 1802,
	LANG_TYPE_TCL = 1901,
	LANG_TYPE_XML = 2301,

	//Test types
	LANG_TYPE_FSM = 2601,
	LANG_TYPE_PLC = 2602,

	LANG_TYPE_END = 2700
};

class ILangGrammar;
class CFileParserFactory
{
private:
	CFileParserFactory();
public:
	static IFileParser* GetFileParser(ELangType eLangType, CFileInfo* pFileInfo = NULL, DWORD nMode = FP_MODE_DEFAULT);
	static IFileParser* GetGenericFileParser(ILangGrammar* pLangGrammar, CFileInfo* pFileInfo = NULL, DWORD nMode = FP_MODE_DEFAULT);
};

#endif