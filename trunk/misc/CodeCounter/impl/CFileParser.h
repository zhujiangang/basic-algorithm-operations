#ifndef _C_FILE_PARSER_H_
#define _C_FILE_PARSER_H_

#include "FileParser.h"

class CCFileParser : public IFileParser
{
public:
	CCFileParser(CFileInfo* pFileInfo = NULL, DWORD nMode = FP_MODE_DEFAULT, LPCTSTR lpLogFileName = NULL);
	virtual ~CCFileParser();
	virtual void ParseFile();
	virtual void Reset();
protected:
	int Transition(CString& line, int chIndex, int state);

	void IncCodeOnLastCharAction(CString& line, int chIndex);
	void IncCommentOnLastCharAction(CString& line, int chIndex);
	void CommentEndAction(CString& line, int chIndex);
	void CommentSegIncAction(CString& line, int chIndex);
	void CodeBlankMarkSetAction(CString& line, int chIndex);
	void CountBlankLineInCommentBlock();
protected:
	//All the states in the FSM.
	enum EStat
	{
		STAT_NONE = 0,
		STAT_FIRST_SLASH = 1,
		STAT_SECOND_SLASH = 2,
		STAT_STAR_AFTER_SLASH = 3,
		STAT_STAR_IN_BLOCK_COMMENT = 4,
		STAT_ESC_AFTER_SECOND_STAR = 5,
		STAT_FIRST_QM = 6,
		STAT_ESC_AFTER_FIRST_QM = 7,
		STAT_FIRST_SQM = 8,
		STAT_ESC_AFTER_FIRST_SQM = 9,
	};
	//The mode to determine the blank to be which type
	enum EModeBlankInCommentBlock
	{
		COUNT_BLANK_LINE_AS_BLANK_IN_COMMENT_BLOCK = 0,
		COUNT_BLANK_LINE_AS_COMMENT_IN_COMMENT_BLOCK = 1
	};

	//The first state and the last state
	static const EStat STAT_FIRST;
	static const EStat STAT_LAST;

	//enum hack
	enum 
	{ 
		MAX_TRANS = 4,
		MAX_STAT = 10
	}; 
	/**
	* FSM
	*/
	
	//State transition chart
	static CHAR* grammar[][MAX_TRANS];
	//Next state
	static int successor[][MAX_TRANS];
	static int nextLineState[];
	typedef void (CCFileParser::*FileParserActionProc)(CString& line, int chIndex);
	FileParserActionProc actions[MAX_STAT][MAX_TRANS];

	//The current mode used to determine the blank line type
	EModeBlankInCommentBlock m_eModeCountBlankLine;

	/**
	* Internal variables
	*/
	//Current state
	EStat m_eCurStat;
	//Code segment count encountered in this line
	int m_nCodeSegCount;
	//Comment segment count encountered in this line
	int m_nCommentSegCount;
	//In the non-comment segment, is it all blank?
	//In the comment segment, is the previous code segment all blank?
	BOOL m_bAllBlank;
};

#endif