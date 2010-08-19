package com.e2u.sort;

public class Match
{
	public int id = MatchUtil.COMMON_INVALID;
	public MatchPlayerInfo player1 = null;
	public MatchPlayerInfo player2 = null;
//	public byte firstPlayer = MatchUtil.COMMON_INVALID;
	public byte result = MatchUtil.COMMON_INVALID;
	
	public int hashCode()
	{
		return id;
	}
	public boolean equals(Object obj)
	{
		if(!(obj instanceof Match))
		{
			return false;
		}
		Match other = (Match)obj;
		return id == other.id;
	}
	
	public int getFirstPlayerID()
	{
		return player1.playerID;
	}
	
	public String toString()
	{
		StringBuilder sb = new StringBuilder();
		if(result == MatchUtil.COMMON_INVALID)
		{
			sb.append("[id]=%3d, p1=%3d  V.S  p2=%3d, [F]=%3d");			
			return String.format(sb.toString(), id, player1.playerID, player2.playerID, getFirstPlayerID());
		}
		else
		{
			sb.append("[id]=%3d, p1=%3d  %s  p2=%3d, [F]=%3d");		
			return String.format(sb.toString(), id, player1.playerID, 
				MatchUtil.getP1vsP2Result(result), player2.playerID, getFirstPlayerID());
		}
	}
}
