package com.e2u.fsm;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class CodeCounter
{
	private static final int STAT_NONE = 0;
	private static final int STAT_FIRST_SLASH = 1;
	private static final int STAT_SECOND_SLASH = 2;
	private static final int STAT_STAR_AFTER_SLASH = 3;
	private static final int STAT_STAR_IN_BLOCK_COMMENT = 4;
	private static final int STAT_ESC_AFTER_SECOND_STAR = 5;
	private static final int STAT_FIRST_QM = 6;
	private static final int STAT_ESC_AFTER_FIRST_QM = 7;
	private static final int STAT_CODE = 8;
	private static final int STAT_BLOCK_COMMENT_END = 9;
	private static final int STAT_FIRST_SQM = 10;
	private static final int STAT_ESC_AFTER_FIRST_SQM = 11;
	
	private int codeCount = 0;
	private int commentCount = 0;
	private int spaceCount = 0;
	
	private boolean isSpace(int ch)
	{
		if(ch == ' ' || ch == '\t')
		{
			return true;
		}
		return false;
	}
	private boolean isNewLine(int ch)
	{
		if(ch == '\n')
		{
			return true;
		}
		return false;
	}

	public void parse(BufferedReader reader)
	{
		int fromStat = STAT_NONE;
		int curStat = STAT_NONE;
		int ch = -1;
		try
		{
			while( (ch = reader.read()) != -1)
			{
				switch(curStat)
				{
					case STAT_NONE:
					{
						fromStat = STAT_NONE;
						if(ch == '/')
						{
							curStat = STAT_FIRST_SLASH;
						}
						else if(ch == '"')
						{
							curStat = STAT_FIRST_QM;
						}
						else if(isSpace(ch))
						{
							
						}
						else if(ch == '\r')
						{
							
						}
						else if(isNewLine(ch))
						{
							spaceCount++;
						}
						//Any other keys
						else
						{
							curStat = STAT_CODE;
						}
						break;
					}
					case STAT_FIRST_SLASH:
					{
						if(ch == '/')
						{
							curStat = STAT_SECOND_SLASH;
						}
						else if(ch == '*')
						{
							curStat = STAT_STAR_AFTER_SLASH;
						}
						else
						{
							curStat = STAT_CODE;
							if(isNewLine(ch))
							{								
								codeCount++;
							}
						}
						break;
					}
					case STAT_SECOND_SLASH:
					{
						if(isNewLine(ch))
						{
							curStat = STAT_NONE;
							
							commentCount++;
							if(fromStat == STAT_CODE)
							{
								codeCount++;
							}
						}
						//Any other keys
						else
						{
							//stub
						}
						break;
					}
					case STAT_STAR_AFTER_SLASH:
					{
						if(ch == '*')
						{
							curStat = STAT_STAR_IN_BLOCK_COMMENT;
						}
						else if(isNewLine(ch))
						{
							commentCount++;
						}
						//Any other keys
						else
						{
							//stub
						}
						break;
					}
					case STAT_STAR_IN_BLOCK_COMMENT:
					{
						if(ch == '\\')
						{
							curStat = STAT_ESC_AFTER_SECOND_STAR;
						}
						else if(ch == '/')
						{
							curStat = STAT_BLOCK_COMMENT_END;
							commentCount++;
						}
						else if(ch == '\r')
						{
							
						}
						//Any other keys
						else
						{
							if(isNewLine(ch))
							{
								commentCount++;
							}
							curStat = STAT_STAR_AFTER_SLASH;
						}
						break;
					}
					case STAT_ESC_AFTER_SECOND_STAR:
					{						
						if(isNewLine(ch))
						{
							commentCount++;
						}
						else if(ch == '\r')
						{
							
						}
						curStat = STAT_STAR_AFTER_SLASH;
						break;
					}
					case STAT_FIRST_QM:
					{
						if(ch == '\\')
						{
							curStat = STAT_ESC_AFTER_FIRST_QM;
						}
						else if(ch == '"')
						{
							curStat = STAT_CODE;
						}
						//Any other keys
						else
						{
							if(isNewLine(ch))
							{
								codeCount++;
							}
						}
						break;
					}
					case STAT_ESC_AFTER_FIRST_QM:
					{
						if(isNewLine(ch))
						{
							codeCount++;
						}
						curStat = STAT_FIRST_QM;
						break;
					}
					case STAT_CODE:
					{
						fromStat = STAT_CODE;
						if(ch == '/')
						{
							curStat = STAT_FIRST_SLASH;
						}
						else if(ch == '"')
						{
							curStat = STAT_FIRST_QM;
						}
						else if(isSpace(ch))
						{
							
						}
						else if(isNewLine(ch))
						{
							curStat = STAT_NONE;
							codeCount++;
						}
						else if(ch == '\r')
						{
							
						}
						//Any other keys
						else
						{
							curStat = STAT_CODE;
						}
						break;
					}
					case STAT_BLOCK_COMMENT_END:
					{
						fromStat = STAT_BLOCK_COMMENT_END;
						if(ch == '/')
						{
							curStat = STAT_FIRST_SLASH;
						}
						else if(ch == '"')
						{
							curStat = STAT_FIRST_QM;
						}
						else if(isSpace(ch))
						{
							
						}
						else if(ch == '\r')
						{
							
						}
						else if(isNewLine(ch))
						{
							curStat = STAT_NONE;
						}
						//Any other keys
						else
						{
							curStat = STAT_CODE;
						}
						break;
					}
				}
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public void parse1(BufferedReader reader)
	{
		String line = null;
		int curStat = STAT_NONE;
		int ch = -1;
		try
		{
			while( (line = reader.readLine()) != null)
			{
				line = line.trim();
				if(line.isEmpty())
				{
//					if(curStat == STAT_STAR_AFTER_SLASH)
//					{
//						commentCount++;
//					}
//					else
//					{
//						spaceCount++;
//					}
					spaceCount++;
					continue;
				}

				if(curStat == STAT_STAR_AFTER_SLASH || curStat == STAT_STAR_IN_BLOCK_COMMENT || curStat == STAT_ESC_AFTER_SECOND_STAR)
				{
					
				}
				else
				{
					curStat = STAT_NONE;
				}
				for(int i = 0, size = line.length(); i < size; i++)
				{
					ch = line.charAt(i);				
					switch(curStat)
					{
						case STAT_NONE:
						{
							if(ch == '/')
							{
								curStat = STAT_FIRST_SLASH;
							}
							else if(ch == '"')
							{
								curStat = STAT_FIRST_QM;
							}
							else if(ch == '\'')
							{
								curStat = STAT_FIRST_SQM;
							}
							if(i == size - 1)
							{
								codeCount++;
							}
							break;
						}
						case STAT_FIRST_SLASH:
						{
							if(ch == '/')
							{
								curStat = STAT_SECOND_SLASH;
							}
							else if(ch == '*')
							{
								curStat = STAT_STAR_AFTER_SLASH;
							}
							else
							{
								curStat = STAT_NONE;
							}
							if(i == size - 1)
							{
								if(curStat == STAT_SECOND_SLASH || curStat == STAT_STAR_AFTER_SLASH)
								{
									commentCount++;
								}
								else
								{
									codeCount++;
								}								
							}
							break;
						}
						case STAT_SECOND_SLASH:
						{
							if(i == size - 1)
							{
								commentCount++;
							}
							break;
						}
						case STAT_STAR_AFTER_SLASH:
						{
							if(ch == '*')
							{
								curStat = STAT_STAR_IN_BLOCK_COMMENT;
							}
							if(i == size - 1)
							{
								commentCount++;
							}
							break;
						}
						case STAT_STAR_IN_BLOCK_COMMENT:
						{
							if(ch == '\\')
							{
								curStat = STAT_ESC_AFTER_SECOND_STAR;
							}
							else if(ch == '/')
							{
								curStat = STAT_NONE;
								if(i != size - 1)
								{
									commentCount++;
								}
							}
							//Any other keys
							else
							{
								curStat = STAT_STAR_AFTER_SLASH;
							}
							if(i == size - 1)
							{
								commentCount++;
							}
							break;
						}
						case STAT_ESC_AFTER_SECOND_STAR:
						{						
							curStat = STAT_STAR_AFTER_SLASH;
							if(i == size - 1)
							{
								commentCount++;
							}
							break;
						}
						case STAT_FIRST_QM:
						{
							if(ch == '\\')
							{
								curStat = STAT_ESC_AFTER_FIRST_QM;
							}
							else if(ch == '"')
							{
								curStat = STAT_NONE;
							}
							if(i == size - 1)
							{
								codeCount++;
							}
							break;
						}
						case STAT_ESC_AFTER_FIRST_QM:
						{
							curStat = STAT_FIRST_QM;
							if(i == size - 1)
							{
								codeCount++;
							}
							break;
						}
						case STAT_FIRST_SQM:
						{
							if(ch == '\\')
							{
								curStat = STAT_ESC_AFTER_FIRST_SQM;
							}
							else if(ch == '\'')
							{
								curStat = STAT_NONE;
							}
							if(i == size - 1)
							{
								codeCount++;
							}
							break;
						}
						case STAT_ESC_AFTER_FIRST_SQM:
						{
							curStat = STAT_FIRST_SQM;
							if(i == size - 1)
							{
								codeCount++;
							}
							break;
						}
					}
				}
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	private static void error(String str)
	{
		System.out.println(str);
	}
	public void showResult()
	{
		System.out.printf("CODE COUNT = %d\n", codeCount);
		System.out.printf("COMMENT COUNT = %d\n", commentCount);
		System.out.printf("BLANK COUNT = %d\n", spaceCount);

	}
	
	public static void main(String[] args)
	{
		CodeCounter cc = new CodeCounter();
		BufferedReader reader = null;
		try
		{
			reader = new BufferedReader(new FileReader(new File("test.java")));
			cc.parse1(reader);
			
			cc.showResult();
			
			reader.close();
		}
		catch(Exception e)
		{
			// TODO: handle exception
		}
		
	}
}
