#ifndef _PLC_FILE_PARSER_H_
#define _PLC_FILE_PARSER_H_

#pragma once

#include "FileParser.h"

class CBaseLogger;
class CPlcFileParser : public IFileParser
{
public:
	CPlcFileParser(CFileInfo* pFileInfo);
	virtual ~CPlcFileParser();
	virtual void ParseFile();
	void Increase(DWORD dwFlag);
	void SetLogger(LPCTSTR lpLogFileName);
protected:
	BOOL IsSpace(int ch);
	void CountBlankLineInCommentBlock();
	void CountCodeCommentInOneLine();
	virtual void ParseLine(
        const CString *pLine,
        /* in out */ bool& bMultiLineComment,
        /* out */ bool& bHasCode,
        /* out */ bool& bHasComments);
	CBaseLogger* m_pLogger;
};

#endif