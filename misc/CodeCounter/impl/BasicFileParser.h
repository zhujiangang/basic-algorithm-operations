#ifndef _BASIC_FILE_PARSER_H_
#define _BASIC_FILE_PARSER_H_

#pragma once

#include "FileParser.h"

class CBasicFileParser : public IFileParser
{
public:
	CBasicFileParser(CFileInfo* pFileInfo = NULL, DWORD nMode = FP_MODE_DEFAULT, LPCTSTR lpLogFileName = NULL);
	void ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments);
};

#endif