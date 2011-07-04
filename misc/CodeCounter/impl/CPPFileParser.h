#ifndef _CPP_FILE_PARSER_H_
#define _CPP_FILE_PARSER_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "FileParser.h"

class CCPPFileParser : public IFileParser
{
public:
	CCPPFileParser(CFileInfo* pFileInfo = NULL, DWORD nMode = FP_MODE_DEFAULT, LPCTSTR lpLogFileName = NULL);
	virtual ~CCPPFileParser();
	virtual void ParseFile();
	void ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments);
};

#endif // _CPP_FILE_PARSER_H_
