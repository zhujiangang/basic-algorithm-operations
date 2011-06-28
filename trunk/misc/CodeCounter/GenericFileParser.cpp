#include "StdAfx.h"
#include "GenericFileParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGenericFileParser::CGenericFileParser(CFileInfo* pFileInfo, CLangGrammar* pLangGrammar)
	: IFileParser(), m_pFileInfo(pFileInfo), m_pLangGrammar(pLangGrammar)
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
	int nLineLength;
	while(file.ReadString(sLine))
	{
		m_pFileInfo->Increase(MASK_TOTAL_LINE);

		sLine.TrimLeft();
		sLine.TrimRight();
		if(sLine.IsEmpty())
		{
			CountBlankLineInCommentBlock();
			continue;
		}

		nLineLength = sLine.GetLength();
		for(int i = 0; i < nLineLength; i++)
		{
//			m_eCurStat = (EStat)Transition(sLine, i, m_eCurStat);
		}
	}

	file.Close();
}


void CGenericFileParser::CountBlankLineInCommentBlock()
{
	m_pFileInfo->Increase(MASK_COMMENT_LINE);
}

void CGenericFileParser::CountCodeCommentInOneLine()
{
	m_pFileInfo->Increase(MASK_CODE_LINE | MASK_COMMENT_LINE | MASK_MIXED_LINE);
}

BOOL CGenericFileParser::IsSpace(int ch)
{
	if(ch == ' ' || ch == '\t')
	{
		return TRUE;
	}
	return FALSE;
}