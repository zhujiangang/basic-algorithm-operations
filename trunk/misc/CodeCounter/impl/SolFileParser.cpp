#include "StdAfx.h"
#include "SolFileParser.h"

CSolFileParser::CSolFileParser(const char* szLineComment, CFileInfo* pFileInfo, DWORD nMode)
 : IFileParser(pFileInfo, nMode)
{
	m_sSingleLineComment = szLineComment;
}

void CSolFileParser::ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments)
{
	bHasCode = false;
	bHasComments = false;

	int nCommentLen = m_sSingleLineComment.GetLength();
	int nLineLen = sLine.GetLength();

	if(nLineLen < nCommentLen)
	{
		bHasCode = true;
		return;
	}

	for(int i = 0; i < nCommentLen; i++)
	{
		if(m_sSingleLineComment.GetAt(i) != sLine.GetAt(i))
		{
			bHasCode = true;
			return;
		}
	}
	bHasComments = true;
}

CPlainTextFileParser::CPlainTextFileParser(CFileInfo* pFileInfo, DWORD nMode)
 : IFileParser(pFileInfo, nMode)
{
}
void CPlainTextFileParser::ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments)
{
	bHasCode = true;
	bHasComments = false;	
}

