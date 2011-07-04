#ifndef _PLC_FILE_PARSER_H_
#define _PLC_FILE_PARSER_H_

#pragma once

#include "FileParser.h"

class CPlcFileParser : public IFileParser
{
public:
	CPlcFileParser(CFileInfo* pFileInfo);
	virtual ~CPlcFileParser();
	virtual void ParseFile();
protected:
	virtual void ParseLine(
        const CString *pLine,
        /* in out */ bool& bMultiLineComment,
        /* out */ bool& bHasCode,
        /* out */ bool& bHasComments);
};

#endif