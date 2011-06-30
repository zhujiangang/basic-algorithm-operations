#include "StdAfx.h"
#include "PLCFileParser.h"
#include "BaseLogger.h"

CPlcFileParser::CPlcFileParser(CFileInfo* pFileInfo) : IFileParser(pFileInfo), m_pLogger(NULL)
{
}

CPlcFileParser::~CPlcFileParser()
{
	if(m_pLogger != NULL)
	{
		delete m_pLogger;
		m_pLogger = NULL;
	}
}

BOOL cfg_bProcessComments = TRUE;
BOOL cfg_bProcessBlanks = TRUE;

void CPlcFileParser::ParseFile()
{
	CStdioFile file;
	if(!file.Open(m_pFileInfo->m_sFullFileName, CFile::modeRead))
	{
		AfxTrace(_T("Failed to Open file %s\n"), m_pFileInfo->m_sFullFileName);
		return;
	}
	
	CString sLine;
	bool bInMultiLineComment = false, bHasCode, bHasComments;

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
		
        ParseLine(&sLine, bInMultiLineComment, bHasCode, bHasComments);

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
            if (cfg_bProcessBlanks)
            {
				Increase(MASK_BLANK_LINE);
            }
            else
            {
                Increase(MASK_CODE_LINE);
            }
        }
	}
	
	file.Close();
}

#define IS_PAIR(A, B) (ch == #@A  &&  chNext == #@B)

void CPlcFileParser::ParseLine(
						 const CString *pLine,
						 /* in out */ bool& bInMultiLineComment,
						 /* out */ bool& bHasCode,
						 /* out */ bool& bHasComments)
{
    // state flags
    bool bInString = false;
	
    const int iLineLen = pLine->GetLength();
	
    bHasComments = bInMultiLineComment;
    bHasCode = false;
	
    for (int i = 0; i < iLineLen; ++i)
    {
        unsigned char ch = (*pLine)[i];
        unsigned char chNext = i < (iLineLen - 1)? (*pLine)[i + 1] : 0;
		
        // a // type comment - ignore rest of line
        if (IS_PAIR(/, /)  &&  !bInMultiLineComment  &&  !bInString  &&
            cfg_bProcessComments)
        {
            bHasComments = true;
            return;
        }
		else if(!bInMultiLineComment  &&  !bInString && ((char)ch == '\'') )
		{
			bHasCode = true;
			
			int oldIndex = i;
			//go to next char
			i++;
			
			//This should be a grammar error. 
			//Eg. The char ' is the last char in the line
			if(i >= iLineLen - 1)
			{
				return;
			}
			ch = (*pLine)[i];
			if( ch == '\\' )
			{
				i++;
			}
			//This should be a grammar error. 
			//Eg. The char '\ is the last char in the line
			if(i >= iLineLen - 1)
			{
				return;
			}
			
			//Skip one char
			i++;
			if(i >= iLineLen - 1)
			{
				return;
			}
			ch = (*pLine)[i];
			//This should be a grammar error. We just skip this error. 
			//Eg. 'abcd, ' is expected after the char a
			if( ch != '\'' ) // in eg. ch == b
			{
				//Go back to '
				i = oldIndex;
			}
		}
        // start of /* comment
        else if (IS_PAIR(/, *)  &&  !bInMultiLineComment  &&  !bInString  &&  
            cfg_bProcessComments)
        {
            bInMultiLineComment = true;
            bHasComments = true;
            ++i;
        }
		
        // end of /* comment (
        else if (IS_PAIR(*, /)  &&  bInMultiLineComment && !bInString  &&  cfg_bProcessComments)
        {
            bInMultiLineComment = false;
            ++i;
        }
		
        else if (ch == '\\'  &&  !bInMultiLineComment)
        {
            // escape character - so skip next char
            ++i;
            bHasCode = true;
        }
		
        else if (ch == '"'  &&  !bInMultiLineComment)
        {
            bInString = !bInString;
            bHasCode = true;
        }
		
        else if (!bInMultiLineComment)
        {
            if (!IsSpace(ch))
            {
                bHasCode = true;
            }
        }
    }
}

void CPlcFileParser::CountBlankLineInCommentBlock()
{
	Increase(MASK_COMMENT_LINE);
}

void CPlcFileParser::CountCodeCommentInOneLine()
{
	Increase(MASK_CODE_LINE | MASK_COMMENT_LINE | MASK_MIXED_LINE);
}

BOOL CPlcFileParser::IsSpace(int ch)
{
	if(ch == ' ' || ch == '\t')
	{
		return TRUE;
	}
	return FALSE;
}

void CPlcFileParser::Increase(DWORD dwFlags)
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

void CPlcFileParser::SetLogger(LPCTSTR lpLogFileName)
{
	m_pLogger = new CBaseLogger(lpLogFileName);
}