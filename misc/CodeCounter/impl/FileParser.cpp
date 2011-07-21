#include "StdAfx.h"
#include "FileParser.h"
#include "CPPFileParser.h"
#include "GenericFileParser.h"
#include "CFileParser.h"
#include "PLCFileParser.h"
#include "BasicFileParser.h"
#include "SolFileParser.h"
#include "StdioExFile.h"
#include "BaseTypeArrayEx.h"

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
// 	UINT nResult = (m_nTotalCodeLines + m_nTotalCommentLines) - (m_nTotalLines - m_nTotalBlankLines);
// 	ASSERT(nResult == m_nTotalMixedLines);
	//Comment above lines to make sure the mixed lines can be treated either code or comment or none lines
	return m_nTotalMixedLines;
}

static LPCTSTR lpBlank = "Blank";
static LPCTSTR lpCode  = "Code";
static LPCTSTR lpComment = "Comment";
static LPCTSTR lpMixed	= "Code Comment Mixed";

IFileParser::IFileParser(CFileInfo* pFileInfo, DWORD nMode)
 : m_pFileInfo(pFileInfo), m_nMode(nMode), m_pLogResult(NULL), m_pLogStringInMultiLine(NULL)
{
	SetMode(nMode);
}

IFileParser::~IFileParser()
{
	if(m_pLogResult != NULL)
	{
		m_pLogResult->RemoveAll();
		delete m_pLogResult;
		m_pLogResult = NULL;
	}
	if(m_pLogStringInMultiLine != NULL)
	{
		m_pLogStringInMultiLine->RemoveAll();
		delete m_pLogStringInMultiLine;
		m_pLogStringInMultiLine = NULL;
	}
}
void IFileParser::SetMode(DWORD nMode)
{
	m_nMode = nMode;
	if( (m_nMode & FP_MODE_LOG_RESULT) != 0 && m_pLogResult == NULL)
	{
		m_pLogResult = new CByteArrayEx();
	}
	if( (m_nMode & FP_MODE_LOG_STRING_IN_MULTI_LINE) != 0 && m_pLogStringInMultiLine == NULL)
	{
		m_pLogStringInMultiLine = new CUIntArrayEx();
	}
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
	
	if( (m_nMode & FP_MODE_LOG_RESULT) != 0 )
	{
		ASSERT(m_pLogResult != NULL);
		m_pLogResult->Add((BYTE)dwFlags);
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
				if( (m_nMode & FP_MODE_LOG_STRING_IN_MULTI_LINE) != 0 )
				{
					ASSERT(m_pLogStringInMultiLine != NULL);
					m_pLogStringInMultiLine->Add(m_pFileInfo->m_nTotalLines);
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

		//Log enabled and 
		if( IS_LOG_ENABLED(THE_LIB_LOGGER, log4cplus::DEBUG_LOG_LEVEL) )
		{
			//Result
			if( (m_nMode & FP_MODE_LOG_RESULT) != 0 )
			{
				ASSERT(m_pLogResult != NULL);
				CString szTemp;
				int nSize = m_pLogResult->GetSize();
				LPTSTR lpBuffer = szTemp.GetBufferSetLength(nSize);
				for(int i = 0; i < nSize; i++)
				{
					lpBuffer[i] = ToChar(m_pLogResult->GetAt(i));
				}
				CString sLogInfo;
				sLogInfo.Format("[Result]: %s(%d), [%s]", m_pFileInfo->m_sFileName, m_pFileInfo->m_nTotalLines, szTemp);
				LOG4CPLUS_DEBUG_STR(THE_LIB_LOGGER, (LPCTSTR)sLogInfo)
			}
			//String in Multi Line
			if( (m_nMode & FP_MODE_LOG_STRING_IN_MULTI_LINE) != 0 )
			{
				ASSERT(m_pLogStringInMultiLine != NULL);
				CString szTemp, szIntBuf;
				int nSize = m_pLogStringInMultiLine->GetSize();
				if(nSize > 0)
				{
					szTemp.GetBuffer(nSize * 5);
					szTemp.ReleaseBuffer();

					int i;
					for(i = 0; i < nSize - 1; i++)
					{
						szIntBuf.Format("%d,", m_pLogStringInMultiLine->GetAt(i));
						szTemp += szIntBuf;
					}
					szIntBuf.Format("%d", m_pLogStringInMultiLine->GetAt(i));
					szTemp += szIntBuf;

					CString sLogInfo;
					sLogInfo.Format("[String in Multi Line]: %s, [%s]", m_pFileInfo->m_sFileName, szTemp);
					LOG4CPLUS_DEBUG_STR(THE_LIB_LOGGER, (LPCTSTR)sLogInfo)
				}
			}
		}
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

static TCHAR digits[16] = {
	_T('0'), _T('1'), _T('2'), _T('3'), _T('4'), _T('5'), _T('6'), _T('7'), 
	_T('8'), _T('9'), _T('A'), _T('B'), _T('C'), _T('D'), _T('E'), _T('F')
};

TCHAR IFileParser::ToChar(BYTE bInt)
{
	ASSERT(bInt >= 0 && bInt < 16);
	return digits[bInt];
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
			LOG4CPLUS_DEBUG(THE_LIB_LOGGER, "Unsupported pre-defined language type: "<<(int)eLangType)
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
