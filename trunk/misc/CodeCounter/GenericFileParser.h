#ifndef _GENERIC_FILE_PARSER_H_
#define _GENERIC_FILE_PARSER_H_

#pragma once

#include "FileParser.h"
class CBaseLogger;
class CGenericFileParser : public IFileParser
{
public:
	CGenericFileParser(CFileInfo* pFileInfo, CLangGrammar* pLangGrammar);
	virtual ~CGenericFileParser();
	virtual void ParseFile();	
	void Increase(DWORD dwFlags);
	void SetLogger(LPCTSTR lpLogFileName);
	void ParseLine(const CString *pLine, bool& bMultiLineComment, bool& bHasCode, bool& bHasComments, int& iMultiLineComment);
protected:
	BOOL IsSpace(int ch);
	void CountBlankLineInCommentBlock();
	void CountCodeCommentInOneLine();

protected:
	CLangGrammar* m_pLangGrammar;
	CBaseLogger* m_pLogger;
};

#endif