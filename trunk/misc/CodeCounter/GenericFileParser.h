#ifndef _GENERIC_FILE_PARSER_H_
#define _GENERIC_FILE_PARSER_H_

#pragma once

#include "FileParser.h"
class CBaseLogger;

#define STATE_NORMAL			0
#define STATE_STRING			1
#define STATE_MULTI_COMMENT		2



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

class CGenericFileParser : public IFileParser
{
public:
	CGenericFileParser(CFileInfo* pFileInfo, CLangGrammar* pLangGrammar);
	virtual ~CGenericFileParser();
	virtual void ParseFile();	
	void Increase(DWORD dwFlags);
	void SetLogger(LPCTSTR lpLogFileName);
	void ParseLine(const CString *pLine, bool& bMultiLineComment, bool& bHasCode, bool& bHasComments, int& iMultiLineComment);
	void ParseLineFsm(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments);
protected:
	BOOL IsSpace(int ch);
	void CountBlankLineInCommentBlock();
	void CountCodeCommentInOneLine();

protected:
	CLangGrammar* m_pLangGrammar;
	CBaseLogger* m_pLogger;
};

#endif