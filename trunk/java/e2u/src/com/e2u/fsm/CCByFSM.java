package com.e2u.fsm;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

/**
 * The expected formats include: c/c++, java
 */
public class CCByFSM
{
	/**
	 * All the states in the FSM.
	 */
	private static final int STAT_NONE = 0;
	private static final int STAT_FIRST_SLASH = 1;
	private static final int STAT_SECOND_SLASH = 2;
	private static final int STAT_STAR_AFTER_SLASH = 3;
	private static final int STAT_STAR_IN_BLOCK_COMMENT = 4;
	private static final int STAT_ESC_AFTER_SECOND_STAR = 5;
	private static final int STAT_FIRST_QM = 6;
	private static final int STAT_ESC_AFTER_FIRST_QM = 7;
	private static final int STAT_FIRST_SQM = 8;
	private static final int STAT_ESC_AFTER_FIRST_SQM = 9;
	
	/**
	 * The first state and the last state
	 */
	public static final int STAT_FIRST = STAT_NONE;
	public static final int STAT_LAST = STAT_ESC_AFTER_FIRST_SQM;
	
	/**
	 * The mode to determine how to count when code and comment are in the same one line
	 */
	public static final int COUNT_0_CODE_0_COMMENT = 0;
	public static final int COUNT_0_CODE_1_COMMENT = 1;
	public static final int COUNT_1_CODE_0_COMMENT = 2;
	public static final int COUNT_1_CODE_1_COMMENT = 3;
	
	
	/**
	 * The current used mode for one line contains both code and comment
	 */
	private int modeCodeCommentInOneLine = COUNT_0_CODE_0_COMMENT;
	
	/**
	 * The mode to determine the blank to be which type
	 */
	public static final int COUNT_BLANK_LINE_AS_BLANK_IN_COMMENT_BLOCK = 0;
	public static final int COUNT_BLANK_LINE_AS_COMMENT_IN_COMMENT_BLOCK = 1;
	
	/**
	 * The current mode used to determine the blank line type
	 */
	private int modeCountBlankLine = COUNT_BLANK_LINE_AS_BLANK_IN_COMMENT_BLOCK;
	
	/**
	 * The following is the processing result
	 */
	//Total line count
	private int totalLine = 0;
	//Code line count
	private int codeLine = 0;
	//Comment line count
	private int commentLine = 0;
	//Mixed line count ( the line contains both comment and code )
	private int mixedLine = 0;
	//Blank line count
	private int blankLine = 0;
	
	/**
	 * Internal variables
	 */
	//Current state
	private int curStat = STAT_NONE;
	//Code segment count encountered in this line
	private int codeSeg = 0;
	//Comment segment count encountered in this line
	private int commentSeg = 0;
	//In the non-comment segment, is it all blank?
	//In the comment segment, is the previous code segment all blank?
	private boolean isAllBlank = true; 
	
	
	/**
	 * FSM
	 */
	
	//State transition chart
	private String[][] grammar = 
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
	
	//Next state
	private int[][] successor = 
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
	private int[] nextLineState = 
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
	
	private OnAction cbmsAction = new CodeBlankMarkSetAction();
	private OnAction csiAction = new CommentSegIncAction();
	private OnAction iCommentAction = new IncCommentOnLastCharAction();
	private OnAction iCodeAction = new IncCodeOnLastCharAction();
	private OnAction commentEndAction = new CommentEndAction();
	private OnAction[][] actions = 
	{
		{cbmsAction, cbmsAction, cbmsAction, cbmsAction},
		{csiAction, csiAction, csiAction},
		{iCommentAction},
		{iCommentAction, iCommentAction},
		{commentEndAction, iCommentAction, iCommentAction, iCommentAction},
		{iCommentAction},
		{iCodeAction, iCodeAction, iCodeAction},
		{iCodeAction},
		{iCodeAction, iCodeAction, iCodeAction},
		{iCodeAction}
	};
	private interface OnAction
	{
		public void action(String line, int chIndex);
	}
	private class CodeBlankMarkSetAction extends IncCodeOnLastCharAction
	{
		public void action(String line, int chIndex)
		{
			if(curStat != STAT_NONE)
			{
				return;
			}
			int ch = line.charAt(chIndex);
			
			String[] theGrammar = grammar[curStat];
			
			for(int i = 0; i < theGrammar.length; i++)
			{
				//No need to set
				if(!theGrammar[i].isEmpty() && theGrammar[i].indexOf(ch) != -1)
				{
					break;
				}
				else if(theGrammar[i].isEmpty())
				{
					if(!isSpace(ch))
					{
						isAllBlank = false;
					}
					break;
				}
			}
			
			super.action(line, chIndex);
		}
	}
	
	private class CommentSegIncAction extends IncCommentOnLastCharAction
	{
		public void action(String line, int chIndex)
		{
			if(curStat != STAT_FIRST_SLASH)
			{
				return;
			}
			int ch = line.charAt(chIndex);
			
			String[] theGrammar = grammar[curStat];
			
			for(int i = 0; i < theGrammar.length; i++)
			{
				if(!theGrammar[i].isEmpty() && theGrammar[i].indexOf(ch) != -1)
				{
					if(!isAllBlank)
					{
						codeSeg++;
						isAllBlank = true;					    							    
					}
					commentSeg++;
					break;
				}
				else if(theGrammar[i].isEmpty())
				{
					isAllBlank = false;
					break;
				}
			}
			
			//Last char
			if(chIndex == (line.length() - 1))
			{
				if(codeSeg > 0 && commentSeg > 0)
			    {
					countCodeCommentInOneLine();
			    }
				else
				{
					for(int i = 0; i < theGrammar.length; i++)
					{
						if(!theGrammar[i].isEmpty() && theGrammar[i].indexOf(ch) != -1)
						{
							commentLine++;
							break;
						}
						else if(theGrammar[i].isEmpty())
						{
							codeLine++;
							break;
						}
					}
				}
			}
		}
	}
	
	private class CommentEndAction extends IncCommentOnLastCharAction
	{
		public void action(String line, int chIndex)
		{
			if(commentSeg == 0)
			{
				commentSeg++;
			}
			isAllBlank = true;
			super.action(line, chIndex);
		}
	}
	private class IncCommentOnLastCharAction implements OnAction
	{
		public void action(String line, int chIndex)
		{
			//Last char
			if(chIndex == (line.length() - 1))
			{
				if(codeSeg > 0)
				{
					countCodeCommentInOneLine();
				}
				else
				{
					commentLine++;
				}							
			}
		}
	}
	private class IncCodeOnLastCharAction implements OnAction
	{
		public void action(String line, int chIndex)
		{
			//Last char
			if(chIndex == (line.length() - 1))
			{
				if(commentSeg > 0)
				{
					countCodeCommentInOneLine();
				}
				else
				{
					codeLine++;
				}				
			}
		}
	}
	
	public CCByFSM()
	{
		curStat = STAT_NONE;
		
		codeSeg = 0;
		commentSeg = 0;
		isAllBlank = true;
		
		totalLine = 0;
		codeLine = 0;
		commentLine = 0;
		mixedLine = 0;
		blankLine = 0;
	}
	
	/**
	 * Start to work, count the code lines
	 * @param reader
	 */
	public void parse(BufferedReader reader)
	{
		curStat = STAT_NONE;
		String line = null;
		try
		{
			while( (line = reader.readLine()) != null)
			{
				codeSeg = 0;
				commentSeg = 0;
				isAllBlank = true;
								
				totalLine++;
				
				line = line.trim();
				if(line.isEmpty())
				{
					countBlankLineInCommentBlock();
					continue;
				}				

				for(int i = 0, size = line.length(); i < size; i++)
				{
					curStat = transition(line, i, curStat);
				}				
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	/**
	 * State transition function
	 * @param line
	 * @param chIndex
	 * @param state
	 * @return
	 */
	private int transition(String line, int chIndex, int state)
	{
		int nextState = -1;
		String[] grammars = grammar[state];
		int ch = line.charAt(chIndex);
		for(int i = 0; i < grammars.length; i++)
		{
			if(grammars[i].isEmpty() || grammars[i].indexOf(ch) != -1)
			{
				if(actions[state][i] != null)
				{
					actions[state][i].action(line, chIndex);
				}
				nextState = successor[state][i];
				//Last char
				if(chIndex == line.length() - 1)
				{
					nextState = nextLineState[nextState];
				}
				break;    
			}
		}
		if(nextState < STAT_FIRST || nextState > STAT_LAST)
		{
			throw new IllegalArgumentException("Fatal Error: result = " + nextState);
		}
		return nextState;
	}
	private void countCodeCommentInOneLine()
	{
		countCodeCommentInOneLine(modeCodeCommentInOneLine);
	}
	private void countCodeCommentInOneLine(int mode)
	{
		mixedLine++;
		switch(mode)
		{
			case COUNT_0_CODE_0_COMMENT:
			{
				break;
			}
			case COUNT_0_CODE_1_COMMENT:
			{
				commentLine++;
				break;
			}
			case COUNT_1_CODE_0_COMMENT:
			{
				codeLine++;
				break;
			}
			case COUNT_1_CODE_1_COMMENT:
			{
				codeLine++;
				commentLine++;
				break;
			}
			default:
			{
				System.out.println("Unknown mode : " + mode);
				break;
			}
		}
	}
	private void countBlankLineInCommentBlock()
	{
		if(curStat == STAT_STAR_AFTER_SLASH)
		{
			switch(modeCountBlankLine)
			{
				case COUNT_BLANK_LINE_AS_BLANK_IN_COMMENT_BLOCK:
				{
					blankLine++;
					break;
				}
				case COUNT_BLANK_LINE_AS_COMMENT_IN_COMMENT_BLOCK:
				{
					commentLine++;
					break;
				}
				default:
				{
					System.out.println("Unknown mode : " + modeCountBlankLine);
					break;
				}
			}
		}
		else
		{
			blankLine++;
		}
	}		
	public int getModeCodeCommentInOneLine()
	{
		return modeCodeCommentInOneLine;
	}
	public void setModeCodeCommentInOneLine(
		int modeCodeCommentInOneLine)
	{
		this.modeCodeCommentInOneLine = modeCodeCommentInOneLine;
	}
	public int getModeCountBlankLine()
	{
		return modeCountBlankLine;
	}
	public void setModeCountBlankLine(int modeCountBlankLine)
	{
		this.modeCountBlankLine = modeCountBlankLine;
	}
	
	private boolean isSpace(int ch)
	{
		if(ch == ' ' || ch == '\t')
		{
			return true;
		}
		return false;
	}
	public void showResult()
	{
		System.out.printf("Total Line Count = %d\n", totalLine);
		System.out.printf("Code Line Count = %d\n", codeLine);
		System.out.printf("Comment Line Count = %d\n", commentLine);
		System.out.printf("Mixed Line Count = %d\n", mixedLine);
		System.out.printf("Blank Line Count = %d\n", blankLine);
	}
	
	public static void main(String[] args)
	{
		CCByFSM cc = new CCByFSM();
		BufferedReader reader = null;
		try
		{
			reader = new BufferedReader(new FileReader(new File("test.java")));
			cc.parse(reader);
			
			cc.showResult();
			
			reader.close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}
