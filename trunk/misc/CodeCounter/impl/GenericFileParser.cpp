#include "StdAfx.h"
#include "GenericFileParser.h"
#include "BaseLogger.h"
#include "LangGrammar.h"

CGenericFileParser::CGenericFileParser(ILangGrammar* pLangGrammar, CFileInfo* pFileInfo, DWORD nMode, LPCTSTR lpLogFileName)
	: IFileParser(pFileInfo, nMode, lpLogFileName), m_pLangGrammar(pLangGrammar)
{
}

CGenericFileParser::~CGenericFileParser()
{
}


void CGenericFileParser::ParseFile()
{
	IFileParser::ParseFile();
}

void CGenericFileParser::ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments)
{
	bHasCode = false;
	bHasComments = (state.m_nMajorState == STATE_MULTI_COMMENT);

	int iCharIndex = -1;
	int iIndex = -1;
	const int iLineLen = sLine.GetLength();
    for (int i = 0; i < iLineLen; ++i)
	{
		TCHAR ch = sLine.GetAt(i);
		if(state.m_nMajorState == STATE_NORMAL)
		{
			//1. Single Line Comment Check: //
			if(m_pLangGrammar->IsSingleLineComment(sLine, i))
			{
				bHasComments = true;
				return;
			}
			//2. Single Char Check: '
			else if( (iCharIndex = m_pLangGrammar->GetCharStartIndex(sLine, i)) >= 0 )
			{
				bHasCode = true;

				int oldIndex = i;
				
				CPair& pair = m_pLangGrammar->GetCharMark(iCharIndex);
				i += GetLength(pair.m_szStart);
				
				//This should be a grammar error. 
				//Eg. The char ' is the last char in the line
				if(i >= iLineLen - 1)
				{
					return;
				}
				
				if( (iIndex = m_pLangGrammar->IndexOfEscStr(sLine, i)) >= 0 )
				{
					i += GetLength(m_pLangGrammar->GetEscapeStr(iIndex));

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
				if(!m_pLangGrammar->IsCharEnd(iCharIndex, sLine, i))
				{
					//Go back to '
					i = oldIndex;
				}
				else
				{
					i += GetLength(pair.m_szEnd) - 1;
				}
				continue;
			}
			//3. Multi Line Comment Check: /*
			else if( (iIndex = m_pLangGrammar->GetMultiLineCommentStartIndex(sLine, i)) >= 0 )
			{
				bHasComments = true;

				//State Changed
				state.m_nMajorState = STATE_MULTI_COMMENT;
				state.m_nMinorState = iIndex;

				i += GetLength(m_pLangGrammar->GetMultiLineComment(iIndex).m_szStart) - 1;

				continue;
			}
			//4. String Check: "
			else if( (iIndex = m_pLangGrammar->GetStringStartIndex(sLine, i)) >= 0 )
			{
				bHasCode = true;

				//State Changed
				state.m_nMajorState = STATE_STRING;
				state.m_nMinorState = iIndex;

				i += GetLength(m_pLangGrammar->GetStringMark(iIndex).m_szStart) - 1;

				continue;
			}
			//5. Escape Char Check: 
			else if((iIndex = m_pLangGrammar->IndexOfEscStr(sLine, i)) >= 0)
			{
				bHasCode = true;

				// escape character - so skip next char
				//++i;
				i += GetLength(m_pLangGrammar->GetEscapeStr(iIndex));	
				
				continue;
			}
			//6. Other cases
			else
			{
				if(!IsSpace(sLine.GetAt(i)))
				{
					bHasCode = true;
				}
				continue;
			}
		}
		else if(state.m_nMajorState == STATE_STRING)
		{
			//1. Escape Char Check
			if((iIndex = m_pLangGrammar->IndexOfEscStr(sLine, i)) >= 0)
			{
				bHasCode = true;
				
				// escape character - so skip next char
				//++i;
				i += GetLength(m_pLangGrammar->GetEscapeStr(iIndex));
				continue;
			}
			//2. String End Check
			else if(m_pLangGrammar->IsStringEnd(state.m_nMinorState, sLine, i))
			{
				bHasCode = true;

				//State Changed
				state.m_nMajorState = STATE_NORMAL;

				i += GetLength(m_pLangGrammar->GetStringMark(state.m_nMinorState).m_szEnd) - 1;
				continue;
			}
			//3. Other cases
			else
			{
				if(!IsSpace(sLine.GetAt(i)))
				{
					bHasCode = true;
				}
				continue;
			}
		}
		else if(state.m_nMajorState == STATE_MULTI_COMMENT)
		{
			//1. Multi Line Comment End Check
			if(m_pLangGrammar->IsMultiLineCommentEnd(state.m_nMinorState, sLine, i))
			{
				//No need to set Marks: bHasCode, bHasComment...

				//State Changed
				state.m_nMajorState = STATE_NORMAL;

				i += GetLength(m_pLangGrammar->GetMultiLineComment(state.m_nMinorState).m_szEnd) - 1;
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