#include "StdAfx.h"
#include "PLCFileParser.h"
#include "BaseLogger.h"

CPlcFileParser::CPlcFileParser(CFileInfo* pFileInfo) : IFileParser(pFileInfo)
{
}

CPlcFileParser::~CPlcFileParser()
{
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
