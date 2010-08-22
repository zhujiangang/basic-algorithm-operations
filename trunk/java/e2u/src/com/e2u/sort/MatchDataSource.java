package com.e2u.sort;

import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;

public class MatchDataSource
{
	private Map<Integer, Map<Integer, Match>> matchMap = new TreeMap<Integer, Map<Integer, Match>>();
	private Map<Integer, Player> playerMap = new TreeMap<Integer, Player>();
	private int minorScoreMode = MatchUtil.MINOR_SCORE_OPPONENT_MODE;
	
	private static MatchDataSource instance = new MatchDataSource();

	private MatchDataSource()
	{
	}
	public static MatchDataSource getInstance()
	{
		return instance;
	}
	
	public Map<Integer, Map<Integer, Match>> getMatchMap()
	{
		return matchMap;
	}
	public Map<Integer, Player> getPlayerMap()
	{
		return playerMap;
	}
	public int getMinorScoreMode()
	{
		return minorScoreMode;
	}
	public Match getMatch(int id)
	{		
		for(Iterator iter = matchMap.entrySet().iterator(); iter.hasNext(); )
		{
			Map.Entry<Integer, Map<Integer, Match>> entry = (Map.Entry<Integer, Map<Integer, Match>>)iter.next();
			
			Map<Integer, Match> roundMatchMap = entry.getValue();
			
			if(roundMatchMap.containsKey(id))
			{
				return roundMatchMap.get(id);
			}
		}
		return null;
	}
	public Player getPlayer(int id)
	{
		if(playerMap.containsKey(id))
		{
			return playerMap.get(id);
		}
		return null;
	}
	
	public boolean isFightBefore(int player1ID, int player2ID)
	{
		Player player1 = playerMap.get(player1ID);		
		if(player1 == null)
		{
			throw new IllegalArgumentException("[Error]: Can't find player: player1=" + player1ID + ", player2=" + player2ID);
		}
		
		for(int i = 0, size = player1.matchList.size(); i < size; i++)
		{
			Match match = getMatch(player1.matchList.get(i));
			if(match == null)
			{
				throw new IllegalArgumentException("[Error]: Can't find match: matchID=" + player1.matchList.get(i));
			}
			if(player2ID == getOpponentID(player1ID, match))
			{
				return true;
			}
		}
		
		//This is not necessary. But place here for an assert.
		if(MatchUtil.isDebug())
		{
			Player player2 = playerMap.get(player2ID);
			if(player1 == null)
			{
				throw new IllegalArgumentException("[Error]: Can't find player: player1=" + player1ID + ", player2=" + player2ID);
			}
			for(int i = 0, size = player2.matchList.size(); i < size; i++)
			{
				Match match = getMatch(player2.matchList.get(i));
				if(match == null)
				{
					throw new IllegalArgumentException("[Error]: Can't find match: matchID=" + player2.matchList.get(i));
				}
				if(player1ID == getOpponentID(player2ID, match))
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	public int getOpponentID(int playerID, Match match)
	{
		if(match.player1.playerID == playerID)
		{
			return match.player2.playerID;
		}
		else if(match.player2.playerID == playerID)
		{
			return match.player1.playerID;
		}
		throw new IllegalArgumentException("[Error]: the player doesn't join the match, player=" + playerID + ", match=" + match);
	}
	
	public Player getOpponent(int playerID, Match match)
    {
        if(match.player1.playerID == playerID)
        {
            return getPlayer(match.player2.playerID);
        }
        else if(match.player2.playerID == playerID)
        {
            return getPlayer(match.player1.playerID);
        }
        throw new IllegalArgumentException("[Error]: the player doesn't join the match, player=" + playerID + ", match=" + match);
    }
}
