#include "StdAfx.h"
#include "CFileParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CCFileParser::EStat CCFileParser::STAT_FIRST = STAT_NONE;
const CCFileParser::EStat CCFileParser::STAT_LAST  = STAT_ESC_AFTER_FIRST_SQM;

CHAR* CCFileParser::grammar[][MAX_TRANS] = 
{
	{"/", "\"", "'", ""},  //  STAT_NONE
	{"/", "*", ""},        //  STAT_FIRST_SLASH
	{""},                  //  STAT_SECOND_SLASH
	{"*", ""},             //  STAT_STAR_AFTER_SLASH
	{"/", "\\", "*", ""},  //  STAT_STAR_IN_BLOCK_COMMENT
	{""},                  //  STAT_ESC_AFTER_SECOND_STAR
	{"\\", "\"", ""},      //  STAT_FIRST_QM
	{""},                  //  STAT_ESC_AFTER_FIRST_QM
	{"\\", "'", ""},       //  STAT_FIRST_SQM
	{""}                   //  STAT_ESC_AFTER_FIRST_SQM
};

int CCFileParser::successor[][MAX_TRANS] = 
{
	{STAT_FIRST_SLASH, STAT_FIRST_QM, STAT_FIRST_SQM, STAT_NONE},  
	{STAT_SECOND_SLASH, STAT_STAR_AFTER_SLASH, STAT_NONE},
	{STAT_SECOND_SLASH}, 
	{STAT_STAR_IN_BLOCK_COMMENT, STAT_STAR_AFTER_SLASH},
	{STAT_NONE, STAT_ESC_AFTER_SECOND_STAR, STAT_STAR_IN_BLOCK_COMMENT, STAT_STAR_AFTER_SLASH},
	{STAT_STAR_AFTER_SLASH},
	{STAT_ESC_AFTER_FIRST_QM, STAT_NONE, STAT_FIRST_QM},
	{STAT_FIRST_QM},
	{STAT_ESC_AFTER_FIRST_SQM, STAT_NONE, STAT_FIRST_SQM},
	{STAT_FIRST_SQM}
};

int CCFileParser::nextLineState[] = 
{
	STAT_NONE,               //  STAT_NONE
	STAT_NONE,               //  STAT_FIRST_SLASH
	STAT_NONE,               //  STAT_SECOND_SLASH
	STAT_STAR_AFTER_SLASH,   //  STAT_STAR_AFTER_SLASH
	STAT_STAR_AFTER_SLASH,   //  STAT_STAR_IN_BLOCK_COMMENT
	STAT_STAR_AFTER_SLASH,   //  STAT_ESC_AFTER_SECOND_STAR
	STAT_FIRST_QM,           //  STAT_FIRST_QM
	STAT_FIRST_QM,           //  STAT_ESC_AFTER_FIRST_QM
	STAT_FIRST_SQM,          //  STAT_FIRST_SQM
	STAT_FIRST_SQM           //  STAT_ESC_AFTER_FIRST_SQM
};

CCFileParser::CCFileParser(CFileInfo* pFileInfo) : IFileParser(pFileInfo)
{
	m_eModeCountBlankLine = COUNT_BLANK_LINE_AS_BLANK_IN_COMMENT_BLOCK;
	FileParserActionProc temp[MAX_STAT][MAX_TRANS] = 
	{
		{CodeBlankMarkSetAction, CodeBlankMarkSetAction, CodeBlankMarkSetAction, CodeBlankMarkSetAction},
		{CommentSegIncAction, CommentSegIncAction, CommentSegIncAction},
		{IncCommentOnLastCharAction},
		{IncCommentOnLastCharAction, IncCommentOnLastCharAction},
		{CommentEndAction, IncCommentOnLastCharAction, IncCommentOnLastCharAction, IncCommentOnLastCharAction},
		{IncCommentOnLastCharAction},
		{IncCodeOnLastCharAction, IncCodeOnLastCharAction, IncCodeOnLastCharAction},
		{IncCodeOnLastCharAction},
		{IncCodeOnLastCharAction, IncCodeOnLastCharAction, IncCodeOnLastCharAction},
		{IncCodeOnLastCharAction}
	};

	int i, j;
	for(i = 0; i < MAX_STAT; i++)
	{
		for(j = 0; j < MAX_TRANS; j++)
		{
			actions[i][j] = temp[i][j];
		}
	}
	Reset();
#ifdef _DEBUG
	m_pLogFile = NULL;
#endif
}
CCFileParser::~CCFileParser()
{
#ifdef _DEBUG
	if(m_pLogFile != NULL)
	{
		fclose(m_pLogFile);
	}
#endif
}

void CCFileParser::ParseFile()
{
	//Start
	m_eCurStat = STAT_NONE;

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
		m_nCodeSegCount = 0;
		m_nCommentSegCount = 0;
		m_bAllBlank = TRUE;
		
		Increase(MASK_TOTAL_LINE);
		
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
			m_eCurStat = (EStat)Transition(sLine, i, m_eCurStat);
		}		
	}
	file.Close();
}

void CCFileParser::Reset()
{
	//Current state
	m_eCurStat = STAT_NONE;
	//Code segment count encountered in this line
	m_nCodeSegCount = 0;
	//Comment segment count encountered in this line
	m_nCommentSegCount = 0;
	//In the non-comment segment, is it all blank?
	//In the comment segment, is the previous code segment all blank?
	m_bAllBlank = TRUE; 
}

int CCFileParser::Transition(CString& line, int chIndex, int state)
{
	int nextState = -1;
	TCHAR ch = line.GetAt(chIndex);
	for(int i = 0; i < MAX_TRANS && grammar[state][i] != 0; i++)
	{
		// check if token belongs to the expected tokens list ? 
		// a empty list acts as a wild char token
		if (*grammar[state][i] == 0 || strchr(grammar[state][i], ch))
		{
			if(actions[state][i] != NULL)
			{
				(this->*actions[state][i])(line, chIndex);
			}
			nextState = successor[state][i];
			//Last char
			if(chIndex == line.GetLength() - 1)
			{
				nextState = nextLineState[nextState];
			}
			break;    
		}
	}
	if(nextState < STAT_FIRST || nextState > STAT_LAST)
	{
		AfxTrace("Fatal Error: result = %d\n", nextState);
	}
	return nextState;
}

void CCFileParser::IncCodeOnLastCharAction(CString& line, int chIndex)
{
	//Last char
	if(chIndex == (line.GetLength() - 1))
	{
		if(m_nCommentSegCount > 0)
		{
			CountCodeCommentInOneLine();
		}
		else
		{
			Increase(MASK_CODE_LINE);
		}				
	}
}

void CCFileParser::IncCommentOnLastCharAction(CString& line, int chIndex)
{
	//Last char
	if(chIndex == (line.GetLength() - 1))
	{
		if(m_nCodeSegCount > 0)
		{
			CountCodeCommentInOneLine();
		}
		else
		{
			Increase(MASK_COMMENT_LINE);
		}				
	}
}

void CCFileParser::CodeBlankMarkSetAction(CString& line, int chIndex)
{
	if(m_eCurStat != STAT_NONE)
	{
		return;
	}
	TCHAR ch = line.GetAt(chIndex);
	
	for(int i = 0; i < MAX_TRANS && grammar[m_eCurStat][i] != 0; i++)
	{
		//No need to set
		if(*grammar[m_eCurStat][i] != 0 && strchr(grammar[m_eCurStat][i], ch))
		{
			break;
		}
		else if(*grammar[m_eCurStat][i] == 0)
		{
			if(!IsSpace(ch))
			{
				m_bAllBlank = FALSE;
			}
			break;
		}
	}
	
	IncCodeOnLastCharAction(line, chIndex);
}
void CCFileParser::CommentSegIncAction(CString& line, int chIndex)
{
	if(m_eCurStat != STAT_FIRST_SLASH)
	{
		return;
	}
	TCHAR ch = line.GetAt(chIndex);
	
	int i;
	for(i = 0; i < MAX_TRANS && grammar[m_eCurStat][i] != 0; i++)
	{
		if(*grammar[m_eCurStat][i] != 0 && strchr(grammar[m_eCurStat][i], ch))
		{
			if(!m_bAllBlank)
			{
				m_nCodeSegCount++;
				m_bAllBlank = TRUE;					    							    
			}
			m_nCommentSegCount++;
			break;
		}
		else if(*grammar[m_eCurStat][i] == 0)
		{
			m_bAllBlank = FALSE;
			break;
		}
	}
	
	//Last char
	if(chIndex == (line.GetLength() - 1))
	{
		if(m_nCodeSegCount > 0 && m_nCommentSegCount > 0)
		{
			CountCodeCommentInOneLine();
		}
		else
		{
			for(i = 0; i < MAX_TRANS && grammar[m_eCurStat][i] != 0; i++)
			{
				if(*grammar[m_eCurStat][i] != 0 && strchr(grammar[m_eCurStat][i], ch))
				{
					Increase(MASK_COMMENT_LINE);
					break;
				}
				else if(*grammar[m_eCurStat][i] == 0)
				{
					Increase(MASK_CODE_LINE);
					break;
				}
			}
		}
	}
}
void CCFileParser::CommentEndAction(CString& line, int chIndex)
{
	if(m_nCommentSegCount == 0)
	{
		m_nCommentSegCount++;
	}
	m_bAllBlank = TRUE;
	
	IncCommentOnLastCharAction(line, chIndex);
}

void CCFileParser::CountBlankLineInCommentBlock()
{
	if(m_eCurStat == STAT_STAR_AFTER_SLASH)
	{
		switch(m_eModeCountBlankLine)
		{
		case COUNT_BLANK_LINE_AS_BLANK_IN_COMMENT_BLOCK:
			{
				Increase(MASK_BLANK_LINE);
				break;
			}
		case COUNT_BLANK_LINE_AS_COMMENT_IN_COMMENT_BLOCK:
			{
				Increase(MASK_COMMENT_LINE);
				break;
			}
		default:
			{
				AfxTrace("Unknown mode : %d\n", m_eModeCountBlankLine);
				break;
			}
		}
	}
	else
	{
		Increase(MASK_BLANK_LINE);
	}
}

void CCFileParser::CountCodeCommentInOneLine()
{
	Increase(MASK_CODE_LINE | MASK_COMMENT_LINE | MASK_MIXED_LINE);
}

BOOL CCFileParser::IsSpace(int ch)
{
	if(ch == ' ' || ch == '\t')
	{
		return TRUE;
	}
	return FALSE;
}

void CCFileParser::Increase(DWORD dwFlags)
{
	m_pFileInfo->Increase(dwFlags);
#ifdef _DEBUG
	if(m_pLogFile == NULL)
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
	fprintf(m_pLogFile, "(%d), Type=%s, dwFlags=%x\n",  m_pFileInfo->m_nTotalLines, sLineInfo, dwFlags);
#endif
}

#ifdef _DEBUG
void CCFileParser::SetLogFile(LPCTSTR lpLogFileName)
{
	FILE* pf = fopen(lpLogFileName, "w");
	if(pf == NULL)
	{
		fprintf(stderr, "Failed to open log file: %s\n", lpLogFileName);
		return;
	}
	m_pLogFile = pf;
}
#endif
