package com.e2u.fsm;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class CodeCounter2
{
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
	
	private static final int COUNT_0_CODE_1_COMMENT = 0;
	private static final int COUNT_1_CODE_0_COMMENT = 2;
	private static final int COUNT_1_CODE_1_COMMENT = 3;
	
	private int codeCount = 0;
	private int commentCount = 0;
	private int spaceCount = 0;
	private int lineCount = 0;
	
	private int curStat = STAT_NONE;
	private int codeSeg = 0;
	private int commentSeg = 0;
	private boolean bAllBlank = true; 
	
	
	private int modeCodeCommentInOneLine = COUNT_1_CODE_1_COMMENT;
	
	private int[][] successor = 
	{
		{STAT_FIRST_SLASH, STAT_FIRST_QM, STAT_FIRST_SQM, STAT_NONE},  //  STAT_NONE
		{STAT_SECOND_SLASH, STAT_STAR_AFTER_SLASH, STAT_NONE},        //  STAT_FIRST_SLASH
		{STAT_SECOND_SLASH},                  //  STAT_SECOND_SLASH
		{STAT_STAR_IN_BLOCK_COMMENT, STAT_STAR_AFTER_SLASH},             //  STAT_STAR_AFTER_SLASH
		{STAT_NONE, STAT_ESC_AFTER_SECOND_STAR, STAT_STAR_IN_BLOCK_COMMENT, STAT_STAR_AFTER_SLASH},       //  STAT_STAR_IN_BLOCK_COMMENT
		{STAT_STAR_AFTER_SLASH},                  //  STAT_ESC_AFTER_SECOND_STAR
		{STAT_ESC_AFTER_FIRST_QM, STAT_NONE, STAT_FIRST_QM},      //  STAT_FIRST_QM
		{STAT_FIRST_QM},                  //  STAT_ESC_AFTER_FIRST_QM
		{STAT_ESC_AFTER_FIRST_SQM, STAT_NONE, STAT_FIRST_SQM},       //  STAT_FIRST_SQM
		{STAT_FIRST_SQM},                  //  STAT_ESC_AFTER_FIRST_SQM
	};
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
		{""},                  //  STAT_ESC_AFTER_FIRST_SQM
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
	OnAction[][] actions = 
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
						bAllBlank = false;
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
					if(!bAllBlank)
					{
						codeSeg++;
						bAllBlank = true;					    							    
					}
					commentSeg++;
					break;
				}
				else if(theGrammar[i].isEmpty())
				{
					bAllBlank = false;
					break;
				}
			}
			
			//Last char
			if(chIndex == (line.length() - 1))
			{
				if(codeSeg > 0 && commentSeg > 0)
			    {
					countCode();
					countComment();
			    }
				else
				{
					for(int i = 0; i < theGrammar.length; i++)
					{
						if(!theGrammar[i].isEmpty() && theGrammar[i].indexOf(ch) != -1)
						{
							commentCount++;
							break;
						}
						else if(theGrammar[i].isEmpty())
						{
							codeCount++;
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
			bAllBlank = true;
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
					countCode();
				}
				commentCount++;			
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
					countComment();
				}
				codeCount++;
			}
		}
	}
	
	public void parse(BufferedReader reader)
	{
		String line = null;
		try
		{
			while( (line = reader.readLine()) != null)
			{
				codeSeg = 0;
				commentSeg = 0;
				bAllBlank = true;
								
				lineCount++;
				
				line = line.trim();
				if(line.isEmpty())
				{
					spaceCount++;
					continue;
				}				

				for(int i = 0, size = line.length(); i < size; i++)
				{
					transition(line, i, curStat);
				}				
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	private void transition(String line, int chIndex, int state)
	{
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
				curStat = successor[state][i];
				//Last char
				if(chIndex == line.length() - 1)
				{
					curStat = nextLineState[curStat];
				}
				break;    
			}
		}
		if(curStat < STAT_NONE || curStat > STAT_ESC_AFTER_FIRST_SQM)
		{
			throw new IllegalArgumentException("Fatal Error: result = " + curStat);
		}
	}
	private void countCode()
	{
		countCode(modeCodeCommentInOneLine);
	}
	private void countCode(int mode)
	{
		switch(mode)
		{
			case COUNT_0_CODE_1_COMMENT:
			{
				break;
			}
			case COUNT_1_CODE_0_COMMENT:
			{
				codeCount++;
				break;
			}
			case COUNT_1_CODE_1_COMMENT:
			{
				codeCount++;
				break;
			}
			default:
			{
				System.out.println("Unknown mode : " + mode);
				break;
			}
		}
	}
	private void countComment()
	{
		countComment(modeCodeCommentInOneLine);
	}
	private void countComment(int mode)
	{
		switch(mode)
		{
			case COUNT_0_CODE_1_COMMENT:
			{
				commentCount++;
				break;
			}
			case COUNT_1_CODE_0_COMMENT:
			{
				break;
			}
			case COUNT_1_CODE_1_COMMENT:
			{
				commentCount++;
				break;
			}
			default:
			{
				System.out.println("Unknown mode : " + mode);
				break;
			}
		}
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
		System.out.printf("LINE COUNT = %d\n", lineCount);
		System.out.printf("CODE COUNT = %d\n", codeCount);
		System.out.printf("COMMENT COUNT = %d\n", commentCount);
		System.out.printf("BLANK COUNT = %d\n", spaceCount);
	}
	
	public static void main(String[] args)
	{
		CodeCounter2 cc = new CodeCounter2();
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
