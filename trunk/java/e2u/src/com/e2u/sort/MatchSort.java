package com.e2u.sort;

import java.util.ArrayList;
import java.util.Map;
import java.util.TreeMap;

public class MatchSort
{
	private Map<Integer, MatchPlayer> playerMap = new TreeMap<Integer, MatchPlayer>();
	private Map<Integer, Match> matchMap = new TreeMap<Integer, Match>();
	
	private int curRound = MatchUtil.COMMON_INVALID;
	
	public void initPlayerInformation(int count)
	{
		for(int i = 0; i < count; i++)
		{
			MatchPlayer player = new MatchPlayer();
			player.id = (i + 1);
			player.name = "player" + player.id;
			player.score = 0;
			player.matchList = new ArrayList<Integer>();
			
			playerMap.put(Integer.valueOf(player.id), player);
		}
	}
	public void startArrange()
	{
		int matchCount = playerMap.size() / 2;
		
		for(int i = 0; i < matchCount; i++)
		{
			Match match = new Match();
			
			match.id = (i + 1);
			match.player1ID = (i + 1);
			match.player2ID = matchCount + (i + 1);
			
			if(i % 2 == 0)
			{
				match.firstPlayer = MatchUtil.FIRST_PLAYER_1;
			}
			else
			{
				match.firstPlayer = MatchUtil.FIRST_PLAYER_2;
			}
			
			matchMap.put(Integer.valueOf(match.id), match);
		}
	}
	public void playMath()
	{		
	}
	public void showResult()
	{
		
	}
	public void arrange()
	{
	}
	public void showArrangeTable()
	{
		
	}
	
	
	public static void main(String args[])
	{
		int playerCount = 10;
		int matchRound = 3;
		
		MatchSort matchSort = new MatchSort();
		matchSort.initPlayerInformation(playerCount);
		
		matchSort.startArrange();
		matchSort.showArrangeTable();
		
		for(int i = 0; i < matchRound; i++)
		{
			matchSort.playMath();
			
			matchSort.showResult();
			
			//not last round match
			if(i < matchRound - 1)
			{
				matchSort.arrange();
				matchSort.showArrangeTable();
			}
		}		
	}
}
