#include "StdAfx.h"
#include "FileParser.h"
#include "CPPFileParser.h"
#include "GenericFileParser.h"
#include "CFileParser.h"
#include "PLCFileParser.h"
#include "BasicFileParser.h"
#include "SolFileParser.h"
#include "StdioExFile.h"

CFileInfo::CFileInfo(LPCTSTR lpszFullFileName)
 : m_nTotalLines(0), m_nCodeLines(0), m_nCommentLines(0), m_nBlankLines(0), m_nMixedLines(0)
{
	if(lpszFullFileName != NULL)
	{
		SetFileName(lpszFullFileName);
	}
}

bool CFileInfo::operator==(const CFileInfo& other) const
{
	if(m_nTotalLines != other.m_nTotalLines)
	{
		return false;
	}
	if(m_nCodeLines != other.m_nCodeLines)
	{
		return false;
	}
	if(m_nCommentLines != other.m_nCommentLines)
	{
		return false;
	}
	if(m_nBlankLines != other.m_nBlankLines)
	{
		return false;
	}
	if(m_nMixedLines != other.m_nMixedLines)
	{
		return false;
	}
	return true;
}

void CFileInfo::SetFileName(LPCTSTR lpszFullFileName)
{
    TCHAR drive[MAX_PATH];
    TCHAR dir[MAX_PATH];
    TCHAR fname[MAX_PATH];
    TCHAR ext[MAX_PATH];
	
    m_sFullFileName = lpszFullFileName;
	
    _tsplitpath(m_sFullFileName, drive, dir, fname, ext);
	
    m_sFilePath = drive;
    m_sFilePath += dir;
	
	m_sFileExt = ext;
	
    m_sFileName = fname;
	m_sFileName += ext;
}

UINT CFileInfo::GetMixedLines() const
{
// 	UINT nResult = (m_nCodeLines + m_nCommentLines) - (m_nTotalLines - m_nBlankLines);
// 	ASSERT(nResult == m_nMixedLines);
	//Comment above lines to make sure the mixed lines can be treated either code or comment or none lines
	UINT nResult = m_nMixedLines;
	return nResult;
}
void CFileInfo::Increase(DWORD dwFlags)
{
	if(dwFlags == MASK_TOTAL_LINE)
	{
		m_nTotalLines++;
		return;
	}
	if(dwFlags & MASK_CODE_LINE)
	{
		m_nCodeLines++;
	}
	if(dwFlags & MASK_COMMENT_LINE)
	{
		m_nCommentLines++;
	}
	if(dwFlags & MASK_BLANK_LINE)
	{
		m_nBlankLines++;
	}
	if(dwFlags & MASK_MIXED_LINE)
	{
		m_nMixedLines++;
	}
}

CTotalInfo::CTotalInfo()
{
	Reset();
}
void CTotalInfo::Reset()
{
	m_nTotalCount = 0;
	m_nTotalLines = 0;
	m_nTotalCodeLines = 0;
	m_nTotalCommentLines = 0;
	m_nTotalBlankLines = 0;
	m_nTotalMixedLines = 0;
}
UINT CTotalInfo::GetTotalMixedLines() const
{
	UINT nResult = (m_nTotalCodeLines + m_nTotalCommentLines) - (m_nTotalLines - m_nTotalBlankLines);
	ASSERT(nResult == m_nTotalMixedLines);
	return nResult;
}

static LPCTSTR lpBlank = "Blank";
static LPCTSTR lpCode  = "Code";
static LPCTSTR lpComment = "Comment";
static LPCTSTR lpMixed	= "Code Comment Mixed";

IFileParser::IFileParser(CFileInfo* pFileInfo, DWORD nMode)
 : m_pFileInfo(pFileInfo), m_nMode(nMode)
{
}

IFileParser::~IFileParser()
{
}

BOOL IFileParser::IsSpace(int ch)
{
	if(ch == ' ' || ch == '\t')
	{
		return TRUE;
	}
	return FALSE;
}

void IFileParser::Increase(DWORD dwFlags)
{
	m_pFileInfo->Increase(dwFlags);
	//Add logs
	if(dwFlags == MASK_TOTAL_LINE)
	{
		return;
	}

	if(IS_LOG_ENABLED(THE_LIB_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
	{
		LPCTSTR lpStr = "";
		if(dwFlags & MASK_BLANK_LINE)
		{
			lpStr = lpBlank;
		}
		else
		{
			if((dwFlags & MASK_CODE_LINE) && (dwFlags & MASK_COMMENT_LINE))
			{
				lpStr = lpMixed;
			}
			else
			{
				if(dwFlags & MASK_COMMENT_LINE)
				{
					lpStr = lpComment;
				}
				else if(dwFlags & MASK_CODE_LINE)
				{
					lpStr = lpCode;
				}
			}
		}
		CString sLogInfo;
		sLogInfo.Format("%s(%d): F=%X, Info=%s", m_pFileInfo->m_sFileName, m_pFileInfo->m_nTotalLines, 
			dwFlags, lpStr);
		LOG4CPLUS_DEBUG_STR(THE_LIB_LOGGER, (LPCTSTR)sLogInfo)
	}
}

void IFileParser::CountBlankLineInCommentBlock()
{
	if(m_nMode & FP_MODE_BLANK_IN_COMMENT_BLOCK_COMMENT)
	{
		Increase(MASK_COMMENT_LINE);
	}
	else
	{
		Increase(MASK_BLANK_LINE);
	}
}

void IFileParser::CountBlankLineInMultiString()
{
	ASSERT(m_nMode & FP_MODE_STRING_IN_MULTI_LINE);
	if(m_nMode & FP_MODE_BLANK_IN_MULTI_LINE_STRING_AS_BLANK)
	{
		Increase(MASK_BLANK_LINE);
	}
	else
	{
		Increase(MASK_CODE_LINE);
	}
}

void IFileParser::CountCodeCommentInOneLine()
{
	DWORD dwFlags = MASK_MIXED_LINE;
	if(m_nMode & FP_MODE_MIXED_LINE_CODE)
	{
		dwFlags |= MASK_CODE_LINE;
	}
	if(m_nMode & FP_MODE_MIXED_LINE_COMMENT)
	{
		dwFlags |= MASK_COMMENT_LINE;
	}
	Increase(dwFlags);
}

void IFileParser::ParseFile()
{
	TRY
	{
		CStdioExFile file;
		if(!file.Open(m_pFileInfo->m_sFullFileName, CFile::modeRead))
		{
			TCHAR szLogBuf[1024];
			_stprintf(szLogBuf, _T("Failed to Open file %s"), m_pFileInfo->m_sFullFileName);
			LOG4CPLUS_INFO_STR(THE_LIB_LOGGER, szLogBuf);
			//AfxTrace("Failed to Open file %s\n", m_pFileInfo->m_sFullFileName);
			return;
		}
		
		CString sLine;
		ParseState state;
		state.m_nMajorState = STATE_NORMAL;
		
		bool bHasCode, bHasComments;
		while(file.ReadLine(sLine))
		{
			Increase(MASK_TOTAL_LINE);
			
			if(state.m_nMajorState == STATE_STRING)
			{
				if(IS_LOG_ENABLED(THE_LIB_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
				{
					CString sLogInfo;
					sLogInfo.Format("%s(%d): Multi String in line", m_pFileInfo->m_sFileName, m_pFileInfo->m_nTotalLines);
					LOG4CPLUS_DEBUG_STR(THE_LIB_LOGGER, (LPCTSTR)sLogInfo)
				}
				//Multi Line String is NOT allowed
				if( (m_nMode & FP_MODE_STRING_IN_MULTI_LINE) == 0)
				{
					state.m_nMajorState = STATE_NORMAL;
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
			
			ParseLine(sLine, state, bHasCode, bHasComments);

			ASSERT( bHasComments || bHasCode );

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
	CATCH(CFileException, e)
	{
		TCHAR szCause[512];
		e->GetErrorMessage(szCause, 512);
		AfxTrace("[CFileException]-%s: %s\n", m_pFileInfo->m_sFullFileName, szCause);
		REPORT_EXCEPTION_ERROR(e);
	}
	AND_CATCH_ALL(e)
	{
		TCHAR szCause[512];
		e->GetErrorMessage(szCause, 512);
		AfxTrace("%s: %s\n", m_pFileInfo->m_sFullFileName, szCause);
		REPORT_EXCEPTION_ERROR(e);
	}
	END_CATCH_ALL
}

void IFileParser::ParseLine(const CString& sLine, ParseState& state, bool& bHasCode, bool& bHasComments)
{
}


CFileParserFactory::CFileParserFactory()
{
}

IFileParser* CFileParserFactory::GetFileParser(ELangType eLangType, CFileInfo* pFileInfo, DWORD nMode)
{
	IFileParser* pFileParser = NULL;
	switch(eLangType)
	{
	case LANG_TYPE_C:
	case LANG_TYPE_CPP:
	case LANG_TYPE_CSHARP:
	case LANG_TYPE_JAVA:
		{
			pFileParser = new CCPPFileParser(pFileInfo, nMode);
		}
		break;
	case LANG_TYPE_BASIC:
		{
			pFileParser = new CBasicFileParser(pFileInfo, nMode);
		}
		break;
	case LANG_TYPE_INI:
		{
			pFileParser = new CSolFileParser(";", pFileInfo, nMode);
		}
		break;
	case LANG_TYPE_MAK:
		{
			pFileParser = new CSolFileParser("#", pFileInfo, nMode);
		}
		break;
	case LANG_TYPE_FSM:
		{
			pFileParser = new CCFileParser(pFileInfo);
		}
		break;

	case LANG_TYPE_PLC:
		{
			pFileParser = new CPlcFileParser(pFileInfo);
		}
		break;

	default:
		{
			AfxTrace("Unsupported language type: %d\n", eLangType);
			LOG4CPLUS_INFO(THE_LIB_LOGGER, "Unsupported language type: "<<(int)eLangType)
		}
		break;

	}
	return pFileParser;
}

IFileParser* CFileParserFactory::GetGenericFileParser(ILangGrammar* pLangGrammar, CFileInfo* pFileInfo, 
													  DWORD nMode)
{
	IFileParser* pFileParser = new CGenericFileParser(pLangGrammar, pFileInfo, nMode);
	return pFileParser;
}
