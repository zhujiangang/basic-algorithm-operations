#include "stdafx.h"
#include "CPPFileParser.h"
#include "BaseLogger.h"

CCPPFileParser::CCPPFileParser(CFileInfo* pFileInfo, DWORD nMode, LPCTSTR lpLogFileName)
 : IFileParser(pFileInfo, nMode, lpLogFileName)
{

}

CCPPFileParser::~CCPPFileParser()
{

}

void CCPPFileParser::ParseFile()
{
	IFileParser::ParseFile();
}

#define IS_PAIR(A, B) (ch == #@A  &&  chNext == #@B)

void CCPPFileParser::ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments)
{
	bHasCode = false;
	bHasComments = (state.m_nMajorState == STATE_MULTI_COMMENT);

	int iCharIndex = -1;
	int iIndex = -1;
	const int iLineLen = sLine.GetLength();
    for (int i = 0; i < iLineLen; ++i)
	{
		TCHAR ch = sLine.GetAt(i);
        TCHAR chNext = i < (iLineLen - 1) ? sLine.GetAt(i + 1) : 0;

		if(state.m_nMajorState == STATE_NORMAL)
		{
			//1. Single Line Comment Check: //
			if( IS_PAIR(/, /) )
			{
				bHasComments = true;
				return;
			}
			//2. Single Char Check: '
			else if( ch == '\'' )
			{
				bHasCode = true;

				int oldIndex = i;

				i++;
				
				//This should be a grammar error. 
				//Eg. The char ' is the last char in the line
				if(i >= iLineLen - 1)
				{
					return;
				}
				
				if( sLine.GetAt(i) == '\\' )
				{
					i++;

					//This should be a grammar error. 
					//Eg. The char '\ is the last char in the line
					if(i >= iLineLen - 1)
					{
						return;
					}
				}
				//Skip one char
				i++;
				//This should be a grammar error. We just skip this error. 
				//Eg. 'abcd, a ' is expected after the char a
				if( sLine.GetAt(i) != '\'' )
				{
					//Go back to '
					i = oldIndex;
				}
				continue;
			}
			//3. Multi Line Comment Check: /*
			else if( IS_PAIR(/, *) )
			{
				bHasComments = true;

				//State Changed
				state.m_nMajorState = STATE_MULTI_COMMENT;
				state.m_nMinorState = iIndex;

				i++;

				continue;
			}
			//4. String Check: "
			else if( ch == '"' )
			{
				bHasCode = true;

				//State Changed
				state.m_nMajorState = STATE_STRING;
				state.m_nMinorState = iIndex;

				continue;
			}
			//5. Escape Char Check: 
			else if( ch == '\\' )
			{
				bHasCode = true;

				// escape character - so skip next char
				i++;
				
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
			//1. Escape Char Check
			if( ch == '\\' )
			{
				bHasCode = true;
				
				// escape character - so skip next char
				i++;
				continue;
			}
			//2. String End Check
			else if( ch == '"' )
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
		else if(state.m_nMajorState == STATE_MULTI_COMMENT)
		{
			//1. Multi Line Comment End Check
			if( IS_PAIR(*, /) )
			{
				//No need to set Marks: bHasCode, bHasComment...

				//State Changed
				state.m_nMajorState = STATE_NORMAL;

				i++;
			}
			continue;
		}
		else
		{
			AfxTrace("Fatal Error! This should not happen. state.m_nMajorState = %d\n", state.m_nMajorState);
			ASSERT(FALSE);
		}		
	}
}