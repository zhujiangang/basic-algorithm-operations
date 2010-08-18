package com.e2u.sort;

import java.util.Comparator;

public class PlayerComparator implements Comparator<Player>
{
	private boolean littleEndian = true;
	public PlayerComparator(int round)
	{
		littleEndian = MatchUtil.littleEndian(round);
	}
	public int compare(Player p1, Player p2)
	{
		return compareHelper(p2, p1);
	}
	//p1 has better score than p2, return > 0
	private int compareHelper(Player p1, Player p2)
	{
		int ret = 0;
		
		ret = p1.score - p2.score;
		if(ret != 0)
		{
			return ret;
		}
		
		ret = p1.minorScore - p2.minorScore;
		if(ret != 0)
		{
			return ret;
		}
		
		ret = p1.winRoundCount - p2.winRoundCount;
		if(ret != 0)
		{
			return ret;
		}
		
		//Notice the orders from now
		ret = p2.foul - p1.foul;
		if(ret != 0)
		{
			return ret;
		}	

		ret = p2.firstWinRound - p1.firstWinRound;
		if(ret != 0)
		{
			return ret;
		}
		
		ret = p1.id - p2.id;
		if(littleEndian)
		{
			ret *= -1;
		}
		
		return ret;
	}
}
