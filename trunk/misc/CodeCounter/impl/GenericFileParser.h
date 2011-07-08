#ifndef _GENERIC_FILE_PARSER_H_
#define _GENERIC_FILE_PARSER_H_

#pragma once

#include "FileParser.h"
class ILangGrammar;

class CGenericFileParser : public IFileParser
{
public:
	CGenericFileParser(ILangGrammar* pLangGrammar, CFileInfo* pFileInfo = NULL, DWORD nMode = FP_MODE_DEFAULT);
	virtual ~CGenericFileParser();
	virtual void ParseFile();
	void ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments);
protected:
	ILangGrammar* m_pLangGrammar;
};

#endif