package com.e2u.sort;

public class Match
{
	public int id = MatchUtil.COMMON_INVALID;
	public int player1ID = MatchUtil.COMMON_INVALID;
	public int player2ID = MatchUtil.COMMON_INVALID;
	public byte firstPlayer = MatchUtil.COMMON_INVALID;
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
}
