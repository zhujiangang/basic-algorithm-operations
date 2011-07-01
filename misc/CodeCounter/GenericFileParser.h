#ifndef _GENERIC_FILE_PARSER_H_
#define _GENERIC_FILE_PARSER_H_

#pragma once

#include "FileParser.h"
class CLangGrammar;

class CGenericFileParser : public IFileParser
{
public:
	enum
	{
		STATE_NORMAL = 0,
		STATE_STRING = 1,
		STATE_MULTI_COMMENT = 2
	};
private:
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

public:
	CGenericFileParser(CFileInfo* pFileInfo, CLangGrammar* pLangGrammar, DWORD nMode = FP_MODE_DEFAULT, LPCTSTR lpLogFileName = NULL);
	virtual ~CGenericFileParser();
	virtual void ParseFile();
	void ParseLine(const CString *pLine, bool& bMultiLineComment, bool& bHasCode, bool& bHasComments, int& iMultiLineComment);
	void ParseLineFsm(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments);
protected:
	CLangGrammar* m_pLangGrammar;
};

#endif