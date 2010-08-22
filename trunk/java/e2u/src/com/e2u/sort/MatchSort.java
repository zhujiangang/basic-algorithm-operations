package com.e2u.sort;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

public class MatchSort
{
	private int curRound = MatchUtil.COMMON_INVALID;
	private int playerCount = MatchUtil.COMMON_INVALID;
	private int roundCount = MatchUtil.COMMON_INVALID;
	private int minorScoreMode = MatchUtil.MINOR_SCORE_OPPONENT_MODE;
	private int matchIDGenerator = 0;
	

	private Map<Integer, SectionSelectee> scoreSectionMap = new TreeMap<Integer, SectionSelectee>(ComparatorFactory.getComparator4Score());
	private Set<Integer> matchedPlayerSet = new HashSet<Integer>();
	
	private int failedTime = 0;
	public MatchSort(int playerCount, int roundCount)
	{
		this.playerCount = playerCount;
		if(roundCount <= 0)
		{
			this.roundCount = MatchUtil.log2(this.playerCount) + 2;
		}
		else
		{
			this.roundCount = roundCount;
		}
		this.curRound = 0;
	}
	
	public int getCurRound()
	{
		return curRound;
	}

	public int getPlayerCount()
	{
		return playerCount;
	}

	public int getRoundCount()
	{
		return roundCount;
	}
	private void updateRound()
	{
		curRound++;
	}
	private synchronized int generateMatchID()
	{
		matchIDGenerator++;
		return matchIDGenerator;
	}
	private synchronized int getCurMatchID()
	{
	    return matchIDGenerator;
	}
	private synchronized void removeMatchID()
	{
	    if(matchIDGenerator == 0)
	    {
	        throw new IllegalArgumentException("[Fatal Error]: matchIDGenerator = 0");
	    }
	    matchIDGenerator--;
	}
	public void initPlayerInformation(int count)
	{
		for(int i = 0; i < count; i++)
		{
			Player player = new Player();
			player.id = (i + 1);
			player.name = "player" + player.id;
			player.score = 0;
			player.matchList = new ArrayList<Integer>();
			
			MatchDataSource.getInstance().getPlayerMap().put(Integer.valueOf(player.id), player);
		}
	}
	public void startArrange()
	{
		updateRound();
		
		int matchCount = MatchDataSource.getInstance().getPlayerMap().size() / 2;
		
		Map<Integer, Match> map = new TreeMap<Integer, Match>();
		for(int i = 0; i < matchCount; i++)
		{
			Match match = generateMatch( (i + 1), matchCount + (i + 1), (i % 2) == 0 );		
			map.put(Integer.valueOf(match.id), match);
		}
		
		MatchDataSource.getInstance().getMatchMap().put(Integer.valueOf(this.curRound), map);
	}
	public void playMath()
	{		
		Map<Integer, Match> map = MatchDataSource.getInstance().getMatchMap().get(Integer.valueOf(this.curRound));
		Player player = null;
		for(Iterator iter = map.entrySet().iterator(); iter.hasNext(); )
		{
			Map.Entry<Integer, Match> entry = (Map.Entry<Integer, Match>)iter.next();
			
			Match match = entry.getValue();
			
			match.result = (byte)MatchUtil.randPlayMatch();
			match.player1.foul = MatchUtil.randFoul();
			match.player2.foul = MatchUtil.randFoul();
			
			player = MatchDataSource.getInstance().getPlayer(match.player1.playerID);
			player.addMatch(match.id);
			
			player = MatchDataSource.getInstance().getPlayer(match.player2.playerID);
			player.addMatch(match.id);
		}
	}
	public void showResult()
	{
		Map<Integer, Match> map = MatchDataSource.getInstance().getMatchMap().get(Integer.valueOf(this.curRound));
		
		for(Iterator iter = map.entrySet().iterator(); iter.hasNext(); )
		{
			Map.Entry<Integer, Match> entry = (Map.Entry<Integer, Match>)iter.next();
			
			Match match = entry.getValue();
			
			showMatch(match);
		}
	}
	public void arrange()
	{
		updateRound();
		updateAllPlayers();
		sortCurrentPlayers();
				
		matchedPlayerSet.clear();
		failedTime = 0;
		Map<Integer, Match> map = new TreeMap<Integer, Match>();
		doArrangeMatch(map);
		MatchDataSource.getInstance().getMatchMap().put(Integer.valueOf(this.curRound), map);
	}
	
	private Player getMatchedPlayer(Player player)
	{
	    Iterator<Map.Entry<Integer, SectionSelectee>> iter = null;
	    SectionSelectee ssee = null;
	    Player candidatePlayer = null;
	    for(iter = scoreSectionMap.entrySet().iterator(); iter.hasNext(); )
	    {
            Map.Entry<Integer, SectionSelectee> entry = iter.next();            
            ssee = entry.getValue();
            
            //First
            if(player.requireFirstThisRound() > 0)
            {
                candidatePlayer = getMatchedPlayer(player, ssee.reqSecondPlayerList, false);
                if(candidatePlayer != null)
                {
                    return candidatePlayer;
                }
                candidatePlayer = getMatchedPlayer(player, ssee.reqFirstPlayerList, true);
                if(candidatePlayer != null)
                {
                    return candidatePlayer;
                }             
            }
            else
            {
                candidatePlayer = getMatchedPlayer(player, ssee.reqFirstPlayerList, false);
                if(candidatePlayer != null)
                {
                    return candidatePlayer;
                }
                candidatePlayer = getMatchedPlayer(player, ssee.reqSecondPlayerList, true);
                if(candidatePlayer != null)
                {
                    return candidatePlayer;
                }                 
            }
	    }
	    return null;
	}
	
	private boolean arrangeMatch(List<Player> playerList, Map<Integer, Match> map)
	{
	    if(playerList == null || playerList.isEmpty())
	    {
	        return true;
	    }
	    Player p1 = null, p2 = null;        
	    Iterator<Player> p1Iter = null;
	    for(p1Iter = playerList.iterator(); p1Iter.hasNext(); )
        {
            p1 = p1Iter.next();
            if(matchedPlayerSet.contains(p1.id))
            {
                continue;
            }
            p2 = getMatchedPlayer(p1);
            if(p2 == null)
            {    
                MatchUtil.debug("failedTime="  + failedTime + ", No available matched player for " + p1.id);
                failedTime++;
                removePreviousMatches(Math.min(failedTime, map.size()), map);
                
                if(failedTime > MatchDataSource.getInstance().getPlayerMap().size())
                {
                    MatchUtil.debug("Too many failed times. failedTime = " + failedTime);
                    //System.exit(-1);
                }
                p2 = getMatchedPlayer(p1);
                
                while(p2 == null)
                {     
                    MatchUtil.debug("failedTime="  + failedTime + ", No available matched player for " + p1.id);
                    removePreviousMatches(Math.min(1, map.size() - 1), map);
                    p2 = getMatchedPlayer(p1);
                } 
                MatchUtil.debug("Found failedTime="  + failedTime + ", p1 = " + p1.id + ", p2 = " + p2.id);
                
                {
                    Match match = generateMatch(p1, p2);
                    map.put(match.id, match);
                    
                    matchedPlayerSet.add(p1.id);
                    matchedPlayerSet.add(p2.id);
                    return false;
                }
            }
            Match match = generateMatch(p1, p2);
            map.put(match.id, match);
            
            matchedPlayerSet.add(p1.id);
            matchedPlayerSet.add(p2.id);
        }
	    
	    return true;
	}
	
	private Player getMatchedPlayer(Player player, List<Player> candidateList, boolean isReverse)
	{
	    if(candidateList == null || candidateList.isEmpty())
	    {
	        return null;
	    }
        Player candidatePlayer = null;
        int i, end, step;
        if(!isReverse)
        {
            i = 0;
            end = candidateList.size();
            step = 1;
        }
        else
        {
            i = candidateList.size() - 1;
            end = -1;
            step = -1;
        }
        for(; i != end; i += step)
        {
            candidatePlayer = candidateList.get(i);
            //same one
            if(candidatePlayer.id == player.id)
            {
                continue;
            }
            //has selected
            if(matchedPlayerSet.contains(candidatePlayer.id))
            {
                continue;
            }
            //has fighted
            if(MatchDataSource.getInstance().isFightBefore(player.id, candidatePlayer.id))
            {
                continue;
            }
            return candidatePlayer;
        }
        return null;
	}
	
	private void doArrangeMatch(Map<Integer, Match> map)
	{
	    Iterator<Map.Entry<Integer, SectionSelectee>> iter = null;
	    SectionSelectee ssee = null;
	    
	    while(matchedPlayerSet.size() != MatchDataSource.getInstance().getPlayerMap().size())
	    {
	        for(iter = scoreSectionMap.entrySet().iterator(); iter.hasNext(); )
	        {
	            Map.Entry<Integer, SectionSelectee> entry = iter.next();            
	            ssee = entry.getValue();
	            
	            if(!arrangeMatch(ssee.getLessPlayerList(), map))
	            {
	                break;
	            }
	            if(!arrangeMatch(ssee.getMorePlayerList(), map))
	            {
	                break;
	            }
	        }	        
	    }
	}
	private Match generateMatch(Player p1, Player p2)
	{
        Match match = new Match();
        match.id = generateMatchID();
        
        Comparator<Player> comp = ComparatorFactory.getComparator4SelectFirst(curRound);
        //p2 is less than p1, the less one is first.
        if(comp.compare(p1, p2) > 0)
        {
            Player tempPlayer = p1;
            p1 = p2;
            p2 = tempPlayer;
        }
        match.player1 = new MatchPlayerInfo();
        match.player1.playerID = p1.id;
        match.player1.foul = 0;
        
        match.player2 = new MatchPlayerInfo();
        match.player2.playerID = p2.id;
        match.player2.foul = 0;
        
        return match;	    
	}
	
	private void removePreviousMatches(int previousCount, Map<Integer, Match> map)
	{
	    this.printSeparator();
	    MatchUtil.debug("removePreviousMatches, previousCount="  + previousCount);
	    Match match = null;
	    for(int i = 0; i < previousCount; i++)
	    {
	        if(map.containsKey(getCurMatchID()))
	        {
	            match = map.get(getCurMatchID());

	            MatchUtil.debug(match.toString());
	            map.remove(match.id);
	            matchedPlayerSet.remove(match.player1.playerID);
	            matchedPlayerSet.remove(match.player2.playerID);
	            removeMatchID();
	        }
	        else
	        {
	            throw new IllegalArgumentException("The match doesn't exist, matchID = " + getCurMatchID());
	        }
	    }
	    
	    for(Iterator<Map.Entry<Integer, Match>> matchIter = map.entrySet().iterator(); matchIter.hasNext(); )
        {
            Map.Entry<Integer, Match> matchEntry = matchIter.next();
            showMatch(matchEntry.getValue());
        }
	    this.printSeparator();
	}
	
	private void getClosestOpponent(int previousCount, Map<Integer, Match> map)
	{
	    
	}
	private Match generateMatch(int p1, int p2, boolean isP1First)
	{
		Match match = new Match();
		match.id = generateMatchID();
		
		if(!isP1First)
		{
			int tmp = p1;
			p1 = p2;
			p2 = tmp;
		}
		
		match.player1 = new MatchPlayerInfo();
		match.player1.playerID = p1;
		match.player1.foul = 0;
		
		match.player2 = new MatchPlayerInfo();
		match.player2.playerID = p2;
		match.player2.foul = 0;
		
		return match;
	}
	
	public Match generateMatch(int p1, int p2, int firstPlayerID)
	{
		if(p2 == firstPlayerID)
		{
			p2 = p1;
			p1 = firstPlayerID;
		}
		Match match = new Match();
		
		match.id = generateMatchID();
		
		match.player1 = new MatchPlayerInfo();
		match.player1.playerID = p1;
		match.player1.foul = 0;
		
		match.player2 = new MatchPlayerInfo();
		match.player2.playerID = p2;
		match.player2.foul = 0;
		
		return match;
	}
	
	private void updateAllPlayers()
	{
		Collection<Player> players = MatchDataSource.getInstance().getPlayerMap().values();
		
		for(Iterator<Player> iter = players.iterator(); iter.hasNext(); )
		{
			Player player = iter.next();
			player.calAllOfflineMetrics();
		}
	}
	public void sortCurrentPlayers()
	{
	    scoreSectionMap.clear();
	    
        Collection<Player> players = MatchDataSource.getInstance().getPlayerMap().values();
        SectionSelectee ssee = null;
        for(Iterator<Player> iter = players.iterator(); iter.hasNext(); )
        {
            Player player = iter.next();
            
            if(!scoreSectionMap.containsKey(player.score))
            {
                ssee = new SectionSelectee();
                scoreSectionMap.put(player.score, ssee);
            }
            else
            {
                ssee = scoreSectionMap.get(player.score);
            }
            
            ssee.addPlayer(player);
        }
        
        Comparator<Player> comparator = ComparatorFactory.getComparatorInOneSection(curRound);

        for(Iterator<SectionSelectee> iter = scoreSectionMap.values().iterator(); iter.hasNext(); )
        {
            ssee = iter.next();
            ssee.sort(comparator);
        }
		if(MatchUtil.isDebug())
		{
			printSeparator();
	        for(Iterator<Map.Entry<Integer, SectionSelectee>> iter = scoreSectionMap.entrySet().iterator(); iter.hasNext(); )
	        {
	            Map.Entry<Integer, SectionSelectee> entry = iter.next();
	            
	            showSectionSelectee(entry.getValue());
	        }
		}
	}

	public void showArrangeTable()
	{
		Map<Integer, Match> map = MatchDataSource.getInstance().getMatchMap().get(Integer.valueOf(this.curRound));
		
		for(Iterator iter = map.entrySet().iterator(); iter.hasNext(); )
		{
			Map.Entry<Integer, Match> entry = (Map.Entry<Integer, Match>)iter.next();
			
			Match match = entry.getValue();
			
			showMatch(match);
		}
	}
	
	public static void showMatch(Match match)
	{
		System.out.println(match);
	}
	public static void showPlayer(Player player)
	{
		System.out.println(player);
	}
	public static void showSectionSelectee(SectionSelectee ssee)
	{
	    System.out.println(ssee);
	}
	
	public void printSeparator()
	{
	    System.out.printf("================round %2d=================\n", curRound);
	}
	
	public static void main(String args[])
	{
		int playerCount = 10;
		int roundCount = 6;
		
		MatchSort matchSort = new MatchSort(playerCount, roundCount);
		matchSort.initPlayerInformation(playerCount);
		
		matchSort.startArrange();
		
		
		for(int i = 0; i < roundCount; i++)
		{
		    matchSort.printSeparator();
			matchSort.showArrangeTable();
			
			matchSort.playMath();
			
			matchSort.printSeparator();
			matchSort.showResult();
			
			//not last round match
			if(i < roundCount - 1)
			{
				matchSort.arrange();
			}
		}
		matchSort.printSeparator();
		
		matchSort.sortCurrentPlayers();
	}
}
