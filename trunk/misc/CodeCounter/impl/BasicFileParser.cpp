#include "StdAfx.h"
#include "BasicFileParser.h"

CBasicFileParser::CBasicFileParser(CFileInfo* pFileInfo, DWORD nMode)
 : IFileParser(pFileInfo, nMode)
{
}

void CBasicFileParser::ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments)
{
	bHasCode = false;
	bHasComments = (state.m_nMajorState == STATE_MULTI_COMMENT);

	const int iLineLen = sLine.GetLength();
    for (int i = 0; i < iLineLen; ++i)
	{
		TCHAR ch = sLine.GetAt(i);

		if(state.m_nMajorState == STATE_NORMAL)
		{
			//1. Single Line Comment Check: '
			if(ch == '\'')
			{
				bHasComments = true;
				return;				
			}
			//4. String Check: "
			else if( ch == '"' )
			{
				bHasCode = true;
				
				//State Changed
				state.m_nMajorState = STATE_STRING;
				state.m_nMinorState = 0;
				
				continue;
			}
			//6. Other cases
			else
			{
				if( !IsSpace(ch) )
				{
					bHasCode = true;
				}
				continue;
			}
		}
		else if(state.m_nMajorState == STATE_STRING)
		{
// 			//1. Escape Char Check
// 			if( ch == '\\' )
// 			{
// 				bHasCode = true;
// 				
// 				// escape character - so skip next char
// 				i++;
// 				continue;
// 			}
// 			//2. String End Check
// 			else 
			if( ch == '"' )
			{
				bHasCode = true;
				
				//State Changed
				state.m_nMajorState = STATE_NORMAL;
				continue;
			}
			//3. Other cases
			else
			{
				if(!IsSpace(ch))
				{
					bHasCode = true;
				}
				continue;
			}
		}
	}
}