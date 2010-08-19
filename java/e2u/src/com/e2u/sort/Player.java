package com.e2u.sort;

import java.util.List;

public class Player
{
	//basic information
	public int id = -1;
	public String name = null;
//	public String unit = null;
//	public byte gender = -1;
	
	//Additional info
	public List<Integer> matchList = null; //input
	
	//calculated by matchList, 5 metrics
	public int score = 0; //total personal score, online
	public int minorScore = 0; //opponent score or self accumulate score, offline calculate
	public int winRoundCount = 0; //the round number of winning, online
	public int foul = 0;  //online
	public int firstWinRound = 0; //offline calculate
	
	//online, the metric value of the player start with the first hand.
	//If the player start with the first hand, firstHandBalance--,
	//else with second hand, firstHandBalance++;
	public int firstHandBalance = 0;
	
	//how many sequence times the player in firstHand or secondHand
	public int seqFirstHand = 0;
	
	public synchronized void addMatch(int matchID)
	{
		Match match = MatchDataSource.getInstance().getMatch(matchID);
		if(match == null)
		{
			throw new IllegalArgumentException("No such matchID: " + matchID);
		}
		matchList.add(matchID);
		
		score += MatchUtil.getTheRoundScore(id, match);
		winRoundCount += MatchUtil.getTheRoundWin(id, match);
		
		if(id == match.player1.playerID)
		{
			foul += match.player1.foul;
		}
		else
		{
			foul += match.player2.foul;
		}
		
		//First hand
		if(id == match.getFirstPlayerID())
		{
		    firstHandBalance--;
		    if(seqFirstHand <= 0)
		    {
		        seqFirstHand = 1;
		    }
		    else
		    {
		        seqFirstHand++;
		    }
		}
		else
		{
		    firstHandBalance++;
	        if(seqFirstHand >= 0)
            {
                seqFirstHand = -1;
            }
            else
            {
                seqFirstHand--;
            }
		}
	}
	
	/**
	 * [NOTICE]: this method must be called after all the players's score have been calculated.
	 * @return
	 */
	public int calAllOpponentScoreSum()
	{
		int result = 0;
		for(int i = 0, size = matchList.size(); i < size; i++)
		{
			Match match = MatchDataSource.getInstance().getMatch(matchList.get(i));
			if(match == null)
			{
				throw new IllegalArgumentException("[Fatal error] Can't find the match: " + matchList.get(i));
			}
			result += MatchUtil.getTheOpponentScore(id, match);
		}
		return result;
	}
	public int calAccumulateScore()
	{
		int result = 0;
		int currentScore = 0;
		int theRoundScore = 0;
		for(int i = 0, size = matchList.size(); i < size; i++)
		{
			Match match = MatchDataSource.getInstance().getMatch(matchList.get(i));
			if(match == null)
			{
				throw new IllegalArgumentException("[Fatal error] Can't find the match: " + matchList.get(i));
			}
			theRoundScore = MatchUtil.getTheRoundScore(id, match);
			currentScore += theRoundScore;
			result += currentScore;
		}
		return result;
	}
	/**
	 * Calculate all the offline metrics of a players
	 */
	public synchronized void calAllOfflineMetrics()
	{
		calMinorScore();
	}
	public synchronized void calMinorScore()
	{
		int minorScoreMode = MatchDataSource.getInstance().getMinorScoreMode();
		if(minorScoreMode == MatchUtil.MINOR_SCORE_OPPONENT_MODE)
		{
			minorScore = calAllOpponentScoreSum();
		}
		else if(minorScoreMode == MatchUtil.MINOR_SCORE_ACCUMULATE_MODE)
		{
			minorScore = calAccumulateScore();
		}
		else
		{
			throw new IllegalArgumentException("[Fatal error] Unknow minor score mode: " + minorScoreMode);
		}	
	}
	public boolean isFirstPlayerInMatch(int matchIndexInReverse)
	{
		if(matchList.isEmpty())
		{
			return false;
		}
		Integer matchID = matchList.get(matchList.size() - matchIndexInReverse);
		Match match = MatchDataSource.getInstance().getMatch(matchID);
		
		return id == match.getFirstPlayerID();
	}
	public boolean isFirstPlayerInLastMatch()
	{
		return isFirstPlayerInMatch(1);
	}
	
	/**
	 * 
	 * @return true, if this round need first
	 */
	public int requireFirstThisRound()
	{
	    if(firstHandBalance > 0)
	    {
	        return 1;
	    }
	    else if(firstHandBalance < 0)
	    {
	        return -1;
	    }
	    
	    if(seqFirstHand > 0)
	    {
	        return -1;
	    }
	    else if(seqFirstHand < 0)
	    {
	        return 1;
	    }
	    
		return 0;
	}
	
	public int getScoreInLastMatch()
	{
		if(matchList.isEmpty())
		{
			throw new IllegalArgumentException("No last match");
		}
		Integer matchID = matchList.get(matchList.size() - 1);
		Match match = MatchDataSource.getInstance().getMatch(matchID);
		return MatchUtil.getTheRoundScore(id, match);
	}
	
	public int hashCode()
	{
		return id;
	}
	public boolean equals(Object obj)
	{
		if(!(obj instanceof Player))
		{
			return false;
		}
		Player other = (Player)obj;
		return id == other.id;
	}
	
	public String toString()
	{
		String format = "[id]=%3d, [firstHandBalance]=%2d, [seqFirstHand]=%2d, [score]=%3d, [minorScore]=%3d, [winRoundCount]=%2d, " +
				"[foul]=%3d, [firstWinRound]=%3d";		
		return String.format(format, id, firstHandBalance, seqFirstHand, score, minorScore, winRoundCount, foul, firstWinRound);
	}
}
