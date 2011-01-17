package com.e2u.fsm;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class FSM
{
	private static final int STAT_NONE = 0;
	private static final int STAT_ESC = 1;
	private static final int STAT_VAR = 2;
	private static final int STAT_ESC_VAR = 3;

	public String parseGrammar(String str, Map<String, Object> out)
	{
		String result = "";
		String varname = "";
		char[] buffer = str.toCharArray();

		int curStat = STAT_NONE;
		for(int i = 0; i < buffer.length; i++)
		{
			switch(curStat)
			{
				case STAT_NONE:
				{
					if(buffer[i] == '\\')
					{
						curStat = STAT_ESC;
					}
					else if(buffer[i] == '%')
					{
						curStat = STAT_VAR;
						varname = "";
					}
					else
					{
						result += buffer[i];
					}
					break;
				}
				case STAT_ESC:
				{
					result += buffer[i];
					curStat = STAT_NONE;
					break;
				}
				case STAT_VAR:
				{
					if(buffer[i] == '\\')
					{
						curStat = STAT_ESC_VAR;
					}
					else if(buffer[i] == '%')
					{
						curStat = STAT_NONE;
						if(out.containsKey(varname))
						{
							result += out.get(varname);
						}						
					}
					else
					{
						varname += buffer[i];
					}
					break;
				}
				case STAT_ESC_VAR:
				{
					varname += buffer[i];
					curStat = STAT_VAR;
					break;
				}
			}
		}

		return result;
	}
	private OnAction appendResultAction = new AppendResult();
	private OnAction appendVarNameAction = new AppendVarName();
	private OnAction parVarNameAction = new ParseVarName();
	private static int[][] successor = 
	{
		{STAT_ESC, STAT_VAR, STAT_NONE},
		{STAT_NONE},
		{STAT_ESC_VAR, STAT_NONE, STAT_VAR},
		{STAT_VAR}
	};
	private static String[][] grammar = 
	{
		{"\\", "%", ""},
		{""},
		{"\\", "%", ""},
		{""}
	};
	private OnAction[][] actions = 
	{
		{null, null, appendResultAction},
		{appendResultAction},
		{null, parVarNameAction, appendVarNameAction},
		{appendVarNameAction}
	};
	
	
	
	public String parseGrammar1(String str, Map<String, Object> out)
	{
		char[] buffer = str.toCharArray();
		StringBuffer result = new StringBuffer();
		StringBuffer varname = new StringBuffer();
		List params = new ArrayList(5);
		params.add(result);
		params.add(varname);
		params.add(out);
		params.add(null);
		int curStat = STAT_NONE;
		for(int i = 0; i < buffer.length; i++)
		{			
			params.set(3, new Integer(buffer[i]));
			curStat = transition(curStat, buffer[i], params);
		}
		
		return result.toString();
	}
	
	private int transition(int state, char c, List params)
	{
		int result = -1;
		String[] grammars = grammar[state];
		for(int i = 0; i < grammars.length; i++)
		{
			if(grammars[i].isEmpty() || grammars[i].indexOf(c) != -1)
			{
				if(actions[state][i] != null)
				{
					actions[state][i].action(params);
				}
				result = successor[state][i];
				break;    
			}
		}
		return result;
	}
	                                       
	private static class AppendResult implements OnAction
	{
		public void action(List params)
		{
			StringBuffer result = (StringBuffer)params.get(0);
			char c = (char)((Integer)params.get(3)).intValue();
			
			result.append(c);
		}
	}
	private static class AppendVarName implements OnAction
	{
		public void action(List params)
		{
			StringBuffer varname = (StringBuffer)params.get(1);
			char c = (char)((Integer)params.get(3)).intValue();						
			
			varname.append(c);
		}
	}
	private static class ParseVarName implements OnAction
	{
		public void action(List params)
		{
			StringBuffer result = (StringBuffer)params.get(0);
			StringBuffer varname = (StringBuffer)params.get(1);
			Map<String, Object> map = (Map<String, Object>)params.get(2);
			
			if(map.containsKey(varname.toString()))
			{
				result.append(map.get(varname.toString()));
			}
			
			varname.setLength(0);
		}
	}
	
	private static interface OnAction
	{
		public void action(List params);
	}
	
	private static void test()
	{
		FSM fsm = new FSM();
		Map<String, Object> out = new HashMap<String, Object>();
		
		out.put("p1", "xyz");
		out.put("p2", "8888");
		out.put("Va%lue Onl\\%y@sME", "zaq");
		
		String input = "\\\\32 -- %p1% -- %p2% -- %Va\\%lue Onl\\\\\\%y@sME% -- qw1";
		String strings[] = 
		{
			"\\\\32 -- %p1% -- %p2% -- %Va\\%lue Onl\\\\\\%y@sME% -- qw1",
			"\\\\32%p1%%p2%%Va\\%lue Onl\\\\\\%y@sME%qw1"
		};
		String result = null;

		for(int i = 0; i < strings.length; i++)
		{
			result = fsm.parseGrammar(strings[i], out);
			System.out.println(result);
			
			result = fsm.parseGrammar1(strings[i], out);
			System.out.println(result);
		}
	}
	public static void main(String[] args)
	{
		test();
	}
}
