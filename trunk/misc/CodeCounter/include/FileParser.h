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
	virtual void ParseFile();
	virtual void SetLogger(LPCTSTR lpLogFileName);
public:
	void SetFileInfo(CFileInfo* pFileInfo)
	{
		m_pFileInfo = pFileInfo;
	}
	DWORD GetMode() const
	{
		return m_nMode;
	}
	void SetMode(DWORD nMode)
	{
		m_nMode = nMode;
	}
protected:
	virtual void ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments);
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

enum ELangType
{
	LANG_TYPE_ADA,
	LANG_TYPE_ASSEMBLY,
	LANG_TYPE_ASP,
	LANG_TYPE_BASIC,
	LANG_TYPE_BAT,
	LANG_TYPE_C,
	LANG_TYPE_COBOL,
	LANG_TYPE_CPP,
	LANG_TYPE_CSHARP,
	LANG_TYPE_CSS,
	LANG_TYPE_CSV,
	LANG_TYPE_DELPHI,
	LANG_TYPE_FORTRAN,
	LANG_TYPE_HTML,
	LANG_TYPE_INI,
	LANG_TYPE_JAVA,
	LANG_TYPE_LISP,
	LANG_TYPE_LUA,
	LANG_TYPE_PASCAL,
	LANG_TYPE_PERL,
	LANG_TYPE_PHP,
	LANG_TYPE_PYTHON,
	LANG_TYPE_RUBY,
	LANG_TYPE_SCALA,
	LANG_TYPE_SQL,
	LANG_TYPE_TCL,
	LANG_TYPE_XML,

	//Test types
	LANG_TYPE_FSM,
	LANG_TYPE_PLC
};

class ILangGrammar;
class CFileParserFactory
{
private:
	CFileParserFactory();
public:
	static IFileParser* GetFileParser(ELangType eLangType, CFileInfo* pFileInfo = NULL, DWORD nMode = FP_MODE_DEFAULT, 
		LPCTSTR lpLogFileName = NULL);
	static IFileParser* GetGenericFileParser(ILangGrammar* pLangGrammar, CFileInfo* pFileInfo = NULL, DWORD nMode = FP_MODE_DEFAULT, 
		LPCTSTR lpLogFileName = NULL);
};

#endif