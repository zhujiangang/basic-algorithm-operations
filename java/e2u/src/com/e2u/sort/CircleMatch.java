package com.e2u.sort;

public class CircleMatch
{
	private int playerCount;
	public CircleMatch(int count)
	{
		this.playerCount = count;
	}
	
	public void arrange()
	{
		for(int i = 1; i < playerCount; i++)
		{
			System.out.printf("================Round %2d================\n", i);
			arrange(i);
		}
		System.out.println("====================END=============");
	}
	
	public void arrange(int round)
	{
		int opponent = -1;
		int opponentOfLastPlayer = -1;
		for(int i = 1; i < playerCount; i++)
		{
			opponent = getOpponent(i, round);
			if(opponent == i)
			{
				opponentOfLastPlayer = i;
				opponent = playerCount;				
			}
			showArrangement(i, opponent);
		}
		showArrangement(playerCount, opponentOfLastPlayer);
	}
	
	private int getOpponent(int playerNo, int round)
	{
		if(playerNo >= round)
		{
			return (playerCount - 1) - (playerNo - round);
		}
		else
		{
			return round - playerNo;
		}
	}
	
	private void showArrangement(int p1, int p2)
	{
		System.out.printf("%-3d  V.S  %3d\n", p1, p2);
	}
	public static void main(String[] args)
	{
		CircleMatch match = new CircleMatch(10);
		match.arrange();
	}
}
