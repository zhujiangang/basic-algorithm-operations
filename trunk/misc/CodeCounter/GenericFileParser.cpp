#include "StdAfx.h"
#include "GenericFileParser.h"
#include "BaseLogger.h"
#include "LangGrammar.h"

CGenericFileParser::CGenericFileParser(CFileInfo* pFileInfo, CLangGrammar* pLangGrammar, DWORD nMode, LPCTSTR lpLogFileName)
	: IFileParser(pFileInfo, nMode, lpLogFileName), m_pLangGrammar(pLangGrammar)
{
}

CGenericFileParser::~CGenericFileParser()
{
}


void CGenericFileParser::ParseFile()
{
	CStdioFile file;
	if(!file.Open(m_pFileInfo->m_sFullFileName, CFile::modeRead))
	{
		AfxTrace(_T("Failed to Open file %s\n"), m_pFileInfo->m_sFullFileName);
		return;
	}
	
	CString sLine;
	ParseState state;
	state.m_nMajorState = STATE_NORMAL;

	bool bHasCode, bHasComments;
	while(file.ReadString(sLine))
	{
		Increase(MASK_TOTAL_LINE);

		if(state.m_nMajorState == STATE_STRING)
		{
			//Multi Line String is NOT allowed
			if( (m_nMode & FP_MODE_STRING_IN_MULTI_LINE) == 0)
			{
				state.m_nMajorState = STATE_NORMAL;
				if(m_pLogger != NULL)
				{
					m_pLogger->log(1, "[Grammar Error]: Unexpected String in line(%d)\n", m_pFileInfo->m_nTotalLines);
				}
			}
		}
		
		sLine.TrimLeft();
		sLine.TrimRight();

		if(sLine.IsEmpty())
		{
			if(state.m_nMajorState == STATE_MULTI_COMMENT)
			{
				CountBlankLineInCommentBlock();
			}
			//Multi Line String must be allowed yet.
			else if(state.m_nMajorState == STATE_STRING)
			{
				CountBlankLineInMultiString();
			}
			else
			{
				Increase(MASK_BLANK_LINE);
			}
			continue;
		}

        ParseLineFsm(sLine, state, bHasCode, bHasComments);
		
		if( bHasComments && bHasCode)
		{
			CountCodeCommentInOneLine();
		}
		else
		{
			if (bHasComments)
			{
				Increase(MASK_COMMENT_LINE);
			}
			if (bHasCode)
			{
				Increase(MASK_CODE_LINE);
			}
		}
	}
	
	file.Close();
}

void CGenericFileParser::ParseLineFsm(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments)
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
				
				CPair& pair = m_pLangGrammar->m_charMarkArray.GetAt(iCharIndex);
				i += GetLength(pair.m_szStart);
				
				//This should be a grammar error. 
				//Eg. The char ' is the last char in the line
				if(i >= iLineLen - 1)
				{
					return;
				}
				
				if( (iIndex = m_pLangGrammar->IndexOfEscStr(sLine, i)) >= 0 )
				{
					LG_STRING szEsc = m_pLangGrammar->m_escapeStrArray.GetAt(iIndex);
					i += GetLength(szEsc);

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

				CPair pair = m_pLangGrammar->m_multiCommentArray.GetAt(iIndex);
				i += GetLength(pair.m_szStart) - 1;

				continue;
			}
			//4. String Check: "
			else if( (iIndex = m_pLangGrammar->GetStringStartIndex(sLine, i)) >= 0 )
			{
				bHasCode = true;

				//State Changed
				state.m_nMajorState = STATE_STRING;
				state.m_nMinorState = iIndex;

				CPair pair = m_pLangGrammar->m_stringMarkArray.GetAt(iIndex);
				i += GetLength(pair.m_szStart) - 1;

				continue;
			}
			//5. Escape Char Check: 
			else if((iIndex = m_pLangGrammar->IndexOfEscStr(sLine, i)) >= 0)
			{
				bHasCode = true;

				// escape character - so skip next char
				//++i;
				LG_STRING szEsc = m_pLangGrammar->m_escapeStrArray.GetAt(iIndex);
				i += GetLength(szEsc);	
				
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
				LG_STRING szEsc = m_pLangGrammar->m_escapeStrArray.GetAt(iIndex);
				i += GetLength(szEsc);
				continue;
			}
			//2. String End Check
			else if(m_pLangGrammar->IsStringEnd(state.m_nMinorState, sLine, i))
			{
				bHasCode = true;

				//State Changed
				state.m_nMajorState = STATE_NORMAL;

				CPair pair = m_pLangGrammar->m_stringMarkArray.GetAt(state.m_nMinorState);
				i += GetLength(pair.m_szEnd) - 1;
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

				CPair pair = m_pLangGrammar->m_multiCommentArray.GetAt(state.m_nMinorState);
				i += GetLength(pair.m_szEnd) - 1;
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

void CGenericFileParser::ParseLine(const CString *pLine,
							   /* in out */ bool& bInMultiLineComment,
							   /* out */ bool& bHasCode,
								/* out */ bool& bHasComments,
								int& iMultiLineComment)
{
	bHasCode = false;
    bHasComments = (iMultiLineComment >= 0);

	int iIndex = -1;
	int iStrIndex = -1;
	int iCharIndex = -1;
    
	const int iLineLen = pLine->GetLength();
    for (int i = 0; i < iLineLen; ++i)
    {
        unsigned char ch = (*pLine)[i];
		
		//Check the single line comment
		if((iMultiLineComment < 0) && (iStrIndex < 0) && m_pLangGrammar->IsSingleLineComment(*pLine, i))
		{
			bHasComments = true;
			return;
		}
		else if( (iMultiLineComment < 0) && (iStrIndex < 0) && (iCharIndex < 0) && ((iCharIndex = m_pLangGrammar->GetCharStartIndex(*pLine, i)) >= 0) )
		{
			bHasCode = true;
			
			int oldIndex = i;

			CPair& pair = m_pLangGrammar->m_charMarkArray.GetAt(iCharIndex);
			i += GetLength(pair.m_szStart);
			
			//This should be a grammar error. 
			//Eg. The char ' is the last char in the line
			if(i >= iLineLen - 1)
			{
				iCharIndex = -1;
				return;
			}
			
			if( (iIndex = m_pLangGrammar->IndexOfEscStr(*pLine, i)) >= 0 )
			{
				LG_STRING szEsc = m_pLangGrammar->m_escapeStrArray.GetAt(iIndex);
				i += GetLength(szEsc);
			}
			//This should be a grammar error. 
			//Eg. The char '\ is the last char in the line
			if(i >= iLineLen - 1)
			{
				iCharIndex = -1;
				return;
			}
			
			//Skip one char
			i++;
			//This should be a grammar error. We just skip this error. 
			//Eg. 'abcd, a ' is expected after the char a
			if(!m_pLangGrammar->IsCharEnd(iCharIndex, *pLine, i))
			{
				//Go back to '
				i = oldIndex;
				iCharIndex = -1;
			}
			else
			{
				i += GetLength(pair.m_szEnd) - 1;
				iCharIndex = -1;
			}
		}
		// start of /* comment
        else if ((iMultiLineComment < 0)  &&  (iStrIndex < 0) 
			&& ( ( iMultiLineComment = m_pLangGrammar->GetMultiLineCommentStartIndex(*pLine, i) ) >= 0 ) )
        {
            bInMultiLineComment = true;
            bHasComments = true;
            //++i;
			CPair pair = m_pLangGrammar->m_multiCommentArray.GetAt(iMultiLineComment);
			i += GetLength(pair.m_szStart) - 1;
        }
        // end of /* comment (
        else if ((iStrIndex < 0) && (iMultiLineComment >= 0) && m_pLangGrammar->IsMultiLineCommentEnd(iMultiLineComment, *pLine, i))
        {
			CPair pair = m_pLangGrammar->m_multiCommentArray.GetAt(iMultiLineComment);
			i += GetLength(pair.m_szEnd) - 1;

			iMultiLineComment = -1;
            bInMultiLineComment = false;
            //++i;
        }
		else if( (iMultiLineComment < 0) && ((iIndex = m_pLangGrammar->IndexOfEscStr(*pLine, i)) >= 0) )
        {
			LG_STRING szEsc = m_pLangGrammar->m_escapeStrArray.GetAt(iIndex);	
            // escape character - so skip next char
            //++i;
			i += GetLength(szEsc);
            bHasCode = true;
        }
		else if( (iMultiLineComment < 0) && (iStrIndex < 0) && ((iStrIndex = m_pLangGrammar->GetStringStartIndex(*pLine, i)) >= 0) )
		{
			bHasCode = true;
		}
		else if( (iMultiLineComment < 0) && (iStrIndex >= 0) && m_pLangGrammar->IsStringEnd(iStrIndex, *pLine, i))
		{
			iStrIndex = -1;
			bHasCode = true;
		}	
        else if (iMultiLineComment < 0)
        {
            if (!IsSpace(ch))
            {
                bHasCode = true;
            }
        }
    }
}
