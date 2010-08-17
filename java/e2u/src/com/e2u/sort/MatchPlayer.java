package com.e2u.sort;

import java.util.List;

public class MatchPlayer
{
	//basic information
	public int id = -1;
	public String name = null;
//	public String unit = null;
//	public byte gender = -1;
	
	//Additional info
	public int score = 0;
	public List<Integer> matchList = null;
	
	public int hashCode()
	{
		return id;
	}
	public boolean equals(Object obj)
	{
		if(!(obj instanceof MatchPlayer))
		{
			return false;
		}
		MatchPlayer other = (MatchPlayer)obj;
		return id == other.id;
	}
}
