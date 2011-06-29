#include "StdAfx.h"
#include "GenericFileParser.h"
#include "BaseLogger.h"

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
		// start of /* comment
        else if ((iMultiLineComment < 0)  &&  (iStrIndex < 0) 
			&& ( ( iMultiLineComment = m_pLangGrammar->GetMultiLineCommentStartIndex(*pLine, i) ) >= 0 ) )
        {
            bInMultiLineComment = true;
            bHasComments = true;
            ++i;
        }
        // end of /* comment (
        else if ((iStrIndex < 0) && (iMultiLineComment >= 0) && m_pLangGrammar->IsMultiLineCommentEnd(iMultiLineComment, *pLine, i))
        {
			iMultiLineComment = -1;
            bInMultiLineComment = false;
            ++i;
        }
		else if( (iMultiLineComment < 0) && ((iIndex = m_pLangGrammar->IndexOfEscStr(*pLine, i)) >= 0) )
        {
			CString szEsc = m_pLangGrammar->m_escapeStrArray.GetAt(iIndex);
			i += szEsc.GetLength();
            // escape character - so skip next char
            //++i;
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