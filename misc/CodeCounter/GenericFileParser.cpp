#include "StdAfx.h"
#include "GenericFileParser.h"
#include "BaseLogger.h"
#include "TinyVector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGenericFileParser::CGenericFileParser(CFileInfo* pFileInfo, CLangGrammar* pLangGrammar)
	: IFileParser(pFileInfo), m_pLangGrammar(pLangGrammar), m_pLogger(NULL)
{
}

CGenericFileParser::~CGenericFileParser()
{
	if(m_pLogger != NULL)
	{
		delete m_pLogger;
		m_pLogger = NULL;
	}	
}

void CGenericFileParser::SetLogger(LPCTSTR lpLogFileName)
{
	m_pLogger = new CBaseLogger(lpLogFileName);
}
/*
void CGenericFileParser::ParseFile()
{
	CStdioFile file;
	if(!file.Open(m_pFileInfo->m_sFullFileName, CFile::modeRead))
	{
		AfxTrace(_T("Failed to Open file %s\n"), m_pFileInfo->m_sFullFileName);
		return;
	}
	
	CString sLine;
	bool bInMultiLineComment = false, bHasCode, bHasComments;
	int iMultiLineComment = -1;
	while(file.ReadString(sLine))
	{
		Increase(MASK_TOTAL_LINE);
		
		sLine.TrimLeft();
		sLine.TrimRight();
		if(sLine.IsEmpty())
		{
			if(bInMultiLineComment)
			{
				CountBlankLineInCommentBlock();
			}
			else
			{
				Increase(MASK_BLANK_LINE);
			}
			continue;
		}
		
        ParseLine(&sLine, bInMultiLineComment, bHasCode, bHasComments, iMultiLineComment);

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
        if (!bHasCode  &&  !bHasComments)
        {
			Increase(MASK_BLANK_LINE);
        }
	}
	
	file.Close();
}
*/
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
		
		sLine.TrimLeft();
		sLine.TrimRight();
		if(sLine.IsEmpty())
		{
			if(state.m_nMajorState == STATE_MULTI_COMMENT)
			{
				CountBlankLineInCommentBlock();
			}
			else
			{
				Increase(MASK_BLANK_LINE);
			}
			continue;
		}
		if(m_pFileInfo->m_nTotalLines == 1282)
		{
			AfxTrace("Just here for Debug\n");
		}
		if(state.m_nMajorState != STATE_MULTI_COMMENT && state.m_nMajorState != STATE_NORMAL)
		{
			state.m_nMajorState = STATE_NORMAL;
// 			if(m_pLogger != NULL)
// 			{
// 				m_pLogger->log(1, "State Wrong. State=%d, line=%d\n", state.m_nMajorState, m_pFileInfo->m_nTotalLines);
// 			}		
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
        if (!bHasCode  &&  !bHasComments)
        {
			Increase(MASK_BLANK_LINE);
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
				i += pair.m_szStart.GetLength();
				
				//This should be a grammar error. 
				//Eg. The char ' is the last char in the line
				if(i >= iLineLen - 1)
				{
					return;
				}
				
				if( (iIndex = m_pLangGrammar->IndexOfEscStr(sLine, i)) >= 0 )
				{
					CString szEsc = m_pLangGrammar->m_escapeStrArray.GetAt(iIndex);
					i += szEsc.GetLength();

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
					i += pair.m_szEnd.GetLength() - 1;
				}
			}
			//3. Multi Line Comment Check: /*
			else if( (iIndex = m_pLangGrammar->GetMultiLineCommentStartIndex(sLine, i)) >= 0 )
			{
				bHasComments = true;

				//State Changed
				state.m_nMajorState = STATE_MULTI_COMMENT;
				state.m_nMinorState = iIndex;

				CPair pair = m_pLangGrammar->m_multiCommentArray.GetAt(iIndex);
				i += pair.m_szStart.GetLength() - 1;
			}
			//4. String Check: "
			else if( (iIndex = m_pLangGrammar->GetStringStartIndex(sLine, i)) >= 0 )
			{
				bHasCode = true;

				//State Changed
				state.m_nMajorState = STATE_STRING;
				state.m_nMinorState = iIndex;

				CPair pair = m_pLangGrammar->m_stringMarkArray.GetAt(iIndex);
				i += pair.m_szStart.GetLength() - 1;
			}
			//5. Escape Char Check: 
			else if((iIndex = m_pLangGrammar->IndexOfEscStr(sLine, i)) >= 0)
			{
				bHasCode = true;

				// escape character - so skip next char
				//++i;
				CString szEsc = m_pLangGrammar->m_escapeStrArray.GetAt(iIndex);
				i += szEsc.GetLength();					
			}
			//6. Other cases
			else
			{
				if(!IsSpace(sLine.GetAt(i)))
				{
					bHasCode = true;
				}
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
				CString szEsc = m_pLangGrammar->m_escapeStrArray.GetAt(iIndex);
				i += szEsc.GetLength();					
			}
			//2. String End Check
			else if(m_pLangGrammar->IsStringEnd(state.m_nMinorState, sLine, i))
			{
				bHasCode = true;

				//State Changed
				state.m_nMajorState = STATE_NORMAL;

				CPair pair = m_pLangGrammar->m_stringMarkArray.GetAt(state.m_nMinorState);
				i += pair.m_szEnd.GetLength() - 1;
			}
			//3. Other cases
			else
			{
				if(!IsSpace(sLine.GetAt(i)))
				{
					bHasCode = true;
				}
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
				i += pair.m_szEnd.GetLength() - 1;
			}
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
			i += pair.m_szStart.GetLength();
			
			//This should be a grammar error. 
			//Eg. The char ' is the last char in the line
			if(i >= iLineLen - 1)
			{
				iCharIndex = -1;
				return;
			}
			
			if( (iIndex = m_pLangGrammar->IndexOfEscStr(*pLine, i)) >= 0 )
			{
				CString szEsc = m_pLangGrammar->m_escapeStrArray.GetAt(iIndex);
				i += szEsc.GetLength();
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
				i += pair.m_szEnd.GetLength() - 1;
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
			i += pair.m_szStart.GetLength() - 1;
        }
        // end of /* comment (
        else if ((iStrIndex < 0) && (iMultiLineComment >= 0) && m_pLangGrammar->IsMultiLineCommentEnd(iMultiLineComment, *pLine, i))
        {
			CPair pair = m_pLangGrammar->m_multiCommentArray.GetAt(iMultiLineComment);
			i += pair.m_szEnd.GetLength() - 1;

			iMultiLineComment = -1;
            bInMultiLineComment = false;
            //++i;
        }
		else if( (iMultiLineComment < 0) && ((iIndex = m_pLangGrammar->IndexOfEscStr(*pLine, i)) >= 0) )
        {
			CString szEsc = m_pLangGrammar->m_escapeStrArray.GetAt(iIndex);	
            // escape character - so skip next char
            //++i;
			i += szEsc.GetLength();
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


void CGenericFileParser::CountBlankLineInCommentBlock()
{
	Increase(MASK_COMMENT_LINE);
}

void CGenericFileParser::CountCodeCommentInOneLine()
{
	Increase(MASK_CODE_LINE | MASK_COMMENT_LINE | MASK_MIXED_LINE);
}

BOOL CGenericFileParser::IsSpace(int ch)
{
	if(ch == ' ' || ch == '\t')
	{
		return TRUE;
	}
	return FALSE;
}


void CGenericFileParser::Increase(DWORD dwFlags)
{
	m_pFileInfo->Increase(dwFlags);
	if(m_pLogger == NULL)
	{
		return;
	}
	
	if(dwFlags == MASK_TOTAL_LINE)
	{
		return;
	}
	CString sLineInfo;
	if(dwFlags & MASK_CODE_LINE)
	{
		sLineInfo += _T("CODE ");
	}
	if(dwFlags & MASK_COMMENT_LINE)
	{
		sLineInfo += _T("COMMENT ");
	}
	if(dwFlags & MASK_BLANK_LINE)
	{
		sLineInfo += _T("BLANK ");
	}
	if(dwFlags & MASK_MIXED_LINE)
	{
		sLineInfo += _T("MIXED");
	}
	m_pLogger->log(1, "(%d), Type=%s, dwFlags=%x\n", m_pFileInfo->m_nTotalLines, sLineInfo, dwFlags);
}