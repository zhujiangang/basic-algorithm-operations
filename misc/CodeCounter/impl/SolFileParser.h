#ifndef _SOL_FILE_PARSER_H_
#define _SOL_FILE_PARSER_H_

#pragma once

#include "FileParser.h"

class CSolFileParser : public IFileParser
{
public:
	CSolFileParser(const char* szLineComment, CFileInfo* pFileInfo = NULL, DWORD nMode = FP_MODE_DEFAULT, LPCTSTR lpLogFileName = NULL);
	void ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments);
protected:
	CString m_sSingleLineComment;
};

#endif