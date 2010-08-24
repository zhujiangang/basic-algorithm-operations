package com.e2u.sort;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

public class MatchUtil
{
	public final static int COMMON_INVALID = -1;
	
	public final static byte FIRST_PLAYER_1 = 1;
	public final static byte FIRST_PLAYER_2 = 2;	
	
	public final static int MATCH_RESULT_WINNER_PLAYER_1 = 1;
	public final static int MATCH_RESULT_WINNER_PLAYER_2 = 2;
	public final static int MATCH_RESULT_WINNER_NONE = 3;
	
	public final static int SCORE_WINNER = 2;
	public final static int SCORE_DRAWER = 1;
	public final static int SCORE_LOSER = 0;
	
	public final static int PLAYER_RESULT_WIN = 1;
	public final static int PLAYER_RESULT_DRAW = 2;
	public final static int PLAYER_RESULT_LOSE = 3;
	
	public final static int MINOR_SCORE_OPPONENT_MODE = 1;
	public final static int MINOR_SCORE_ACCUMULATE_MODE = 2;
	
	private static boolean DEBUG = true;
	
	private static Map<Integer, String> resultStrMap = new HashMap<Integer, String>(4);
	
	static
	{
		resultStrMap.put(Integer.valueOf(MATCH_RESULT_WINNER_PLAYER_1), 
			String.valueOf(SCORE_WINNER) + ":" + String.valueOf(SCORE_LOSER));
		
		resultStrMap.put(Integer.valueOf(MATCH_RESULT_WINNER_PLAYER_2), 
			String.valueOf(SCORE_LOSER) + ":" + String.valueOf(SCORE_WINNER));
		
		resultStrMap.put(Integer.valueOf(MATCH_RESULT_WINNER_NONE), 
			String.valueOf(SCORE_DRAWER) + ":" + String.valueOf(SCORE_DRAWER));
	}
	
	public static int rand()
	{
		Random random = new Random();
		int ret = 0;
		for(int i = 0; i < 5; i++)
		{
			ret = random.nextInt(Integer.MAX_VALUE);
			random.setSeed(System.currentTimeMillis() + ret);
		}
		
		return ret;
	}
	public static int randPlayMatch()
	{
		int ret = rand() % 3 + 1;
		return ret;
	}
	//Get a number in 0-2
	public static int randFoul()
	{
		int max = 100;
		int ret = rand() % max;
		
		if(ret <= 90 * max / 100)
		{
			return 0;
		}
		else if(ret <= 95 * max / 100)
		{
			return 1;
		}
		return 2;
	}
	
	public static String getP1vsP2Result(int result)
	{
		return resultStrMap.get(result);
	}
	
	public static int getTheRoundScore(int playerID, Match match)
	{
		if(playerID == match.player1.playerID)
		{
			switch(match.result)
			{
			case MATCH_RESULT_WINNER_PLAYER_1:
				return SCORE_WINNER;
			case MATCH_RESULT_WINNER_PLAYER_2:
				return SCORE_LOSER;
			case MATCH_RESULT_WINNER_NONE:
				return SCORE_DRAWER;
			}
		}
		else if(playerID == match.player2.playerID)
		{
			switch(match.result)
			{
			case MATCH_RESULT_WINNER_PLAYER_1:
				return SCORE_LOSER;
			case MATCH_RESULT_WINNER_PLAYER_2:
				return SCORE_WINNER;
			case MATCH_RESULT_WINNER_NONE:
				return SCORE_DRAWER;
			}
		}
		throw new IllegalArgumentException("Invalid input, playerID = " + playerID + ", match = " + match);
	}
	
	public static int getTheRoundWin(int playerID, Match match)
	{
		if(playerID == match.player1.playerID && (match.result == MATCH_RESULT_WINNER_PLAYER_1))
		{
			return 1;
		}
		if(playerID == match.player2.playerID && (match.result == MATCH_RESULT_WINNER_PLAYER_2))
		{
			return 1;
		}
		return 0;
	}
	
	public static int getTheOpponentScore(int playerID, Match match)
	{
		Player opponent = null;
		if(match.player1.playerID == playerID)
		{
			opponent = MatchDataSource.getInstance().getPlayer(match.player2.playerID);
		}
		else
		{
			opponent = MatchDataSource.getInstance().getPlayer(match.player1.playerID);
		}
		if(opponent == null)
		{
			throw new IllegalArgumentException("[Fatal Error]: can't find the opponent, playerID = "
				+ playerID + ", matchID = " + match.id);
		}
		return opponent.score;
	}

	public static int log2(int x)
	{
		if(x <= 0)
		{
			throw new IllegalArgumentException("The input value is less than zero: x=" + x);
		}
		int a = 1;
		int ret = 0;
		while(a < x)
		{
			a = a << 1;
			ret++;
		}			
		return ret;
	}
	
	public static boolean littleEndian(int round)
	{
		return round % 2 == 1;
	}
	
	/**
	 * @param a
	 * @param startIndex
	 * @param endIndex
	 * @param sepIndex the start index of the second part
	 */
	public static void rotate(Object[] a, int startIndex, int endIndex, int sepIndex)
	{
		reverse(a, startIndex, sepIndex - 1);
		reverse(a, sepIndex, endIndex);
		reverse(a, startIndex, endIndex);
	}
	public static void reverse(Object[] a, int from, int to)
	{
		while(from < to)
		{
			swap(a, from, to);
			from++;
			to--;
		}
	}
	public static void swap(Object[] a, int x, int y)
	{
		Object temp = a[x];
		a[x] = a[y];
		a[y] = temp;
	}
	public static void swap(int[] a, int x, int y)
	{
		int temp = a[x];
		a[x] = a[y];
		a[y] = temp;
	}
	
	private static int count = 0;
	public static boolean perm(int size, PermCallBack pcb, List params)
	{
		int[] in = new int[size];
		for(int i = 0; i < size; i++)
		{
			in[i] = i;
		}
		return perm(in, 0, size - 1, pcb, params);
	}
	public static boolean perm(int in[], int begin, int end, PermCallBack pcb, List params)
	{
		if(begin == end)
		{
			count++;
			return pcb.doOper(in, params);
		}
		else
		{
			for(int i = begin; i <= end; i++)
			{
				swap(in, begin, i);
				if(perm(in, begin + 1, end, pcb, params))
				{
					return true;
				}
				swap(in, begin, i);
			}
		}
		return false;
	}
	
	public static boolean isDebug()
	{
		return DEBUG;
	}
	
	public static void printSeparator()
	{
		System.out.println("=====================================");
	}
	
	public static void debug(String str)
	{
	    if(isDebug())
	    {
	        System.out.println(str);
	    }
	}
	
	private static class PermCallBackImp implements PermCallBack
	{
		public boolean doOper(int[] a, List params)
		{
			if(a[0] == 3 && a[1] == 2 && a[2] == 4)
			{
				for(int i = 0; i < a.length; i++)
				{
					System.out.print(" " + a[i]);
				}
				System.out.println();
				Object obj = params.get(0);
				System.out.println(obj);
				return true;
			}
			return false;
		}
	}
	
	public static void ut()
	{
//		for(int i = 0; i < 33; i++)
//		{
//			System.out.printf("log2(%d) = %d\n", (i + 1), log2(i + 1));
//		}
//		
//		int[] a = new int[5];
//		for(int i = 0; i < a.length; i++)
//		{
//			a[i] = i;
//		}
		List params = new ArrayList();
		params.add("abcdefg");
		int size = 5;
		if(perm(size, new PermCallBackImp(), params))
		{
			System.out.println("true Out now, count = " + count);
//			for(int i = 0; i < size; i++)
//			{
//				System.out.print(" " + a[i]);
//			}
			System.out.println();
		}
		else
		{
			System.out.println("false Out now, count = " + count);
		}
	}
	public static void main(String[] args)
	{
		ut();
	}
}
