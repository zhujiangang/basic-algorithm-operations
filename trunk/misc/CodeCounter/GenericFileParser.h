#ifndef _GENERIC_FILE_PARSER_H_
#define _GENERIC_FILE_PARSER_H_

#pragma once

#include "FileParser.h"

class CGenericFileParser : public IFileParser
{
public:
	CGenericFileParser(CFileInfo* pFileInfo, CLangGrammar* pLangGrammar);
	virtual ~CGenericFileParser();
	virtual void ParseFile();

protected:
	BOOL IsSpace(int ch);
	void CountBlankLineInCommentBlock();
	void CountCodeCommentInOneLine();

protected:
	CFileInfo* m_pFileInfo;
	CLangGrammar* m_pLangGrammar;
};

#endif