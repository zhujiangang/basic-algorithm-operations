package com.e2u.sort;

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
	
	
	public static int rand()
	{
		Random random = new Random();
		return random.nextInt();
	}
	public static int randPlayMatch()
	{
		return rand() % 3 + 1;
	}
}
