package com.e2u.sort;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

public class MatchSort implements PermCallBack
{
	private int curRound = MatchUtil.COMMON_INVALID;
	private int playerCount = MatchUtil.COMMON_INVALID;
	private int roundCount = MatchUtil.COMMON_INVALID;
	private int minorScoreMode = MatchUtil.MINOR_SCORE_OPPONENT_MODE;
	private int matchIDGenerator = 0;
	

	private TreeMap<Integer, SectionSelectee> scoreSectionMap = new TreeMap<Integer, SectionSelectee>(ComparatorFactory.getComparator4Score());
	private Set<Integer> matchedPlayerSet = new HashSet<Integer>();
	private Map<Integer, Integer> failedMap = new HashMap<Integer, Integer>();
	private List<Player> failedList = new ArrayList<Player>();
	private List<Match> preProcessMatchList = new ArrayList<Match>();
	private TreeSet<Integer> curRoundMatchIDSet = new TreeSet<Integer>();
	
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
		
		generateCurRoundMatchID();
	}
	private synchronized int generateMatchID()
	{
		matchIDGenerator++;
		return matchIDGenerator;
	}
	private synchronized int selectFirstMatchID()
	{
		if(curRoundMatchIDSet.isEmpty())
		{
			throw new IllegalArgumentException("[Fatal Error]: No available Match ID");
		}
		int firstID = curRoundMatchIDSet.first();
		curRoundMatchIDSet.remove(firstID);
		return firstID;
	}
	private synchronized int getFirstMatchID()
	{
		if(curRoundMatchIDSet.isEmpty())
		{
			throw new IllegalArgumentException("[Fatal Error]: No available Match ID");
		}
		return curRoundMatchIDSet.first();
	}
	
	private synchronized void removeMatch(Map<Integer, Match> map, int matchID)
	{
		if(!map.containsKey(matchID))
		{
			throw new IllegalArgumentException("[Fatal Error]: The match ID doesn't exist, matchID = " + matchID);
		}		
        Match match = map.get(matchID);
        
        map.remove(matchID);
        matchedPlayerSet.remove(match.player1.playerID);
        matchedPlayerSet.remove(match.player2.playerID);
        
        curRoundMatchIDSet.add(matchID);
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
		failedMap.clear();
		failedList.clear();
		preProcessMatchList.clear();
		TreeMap<Integer, Match> map = new TreeMap<Integer, Match>();
		doArrangeMatch(map);
		MatchDataSource.getInstance().getMatchMap().put(Integer.valueOf(this.curRound), map);
	}
	
	private void generateCurRoundMatchID()
	{
		curRoundMatchIDSet.clear();
		
		int half = playerCount / 2;
		for(int i = 0; i < half; i++)
		{
			curRoundMatchIDSet.add(generateMatchID());
		}
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
            	MatchUtil.debug(String.format("[START]: p1=%2d, NO OPPONENT", p1.id));
            	incFailedTime(p1.id);
            	MatchUtil.debug(String.format("[END]: p1=%2d, NO OPPONENT", p1.id));
            	continue;
            	/*
            	MatchUtil.debug(String.format("[START]: p1=%2d, NO OPPONENT", p1.id));
            	
            	List<Player> missMatchPlayerList = new ArrayList<Player>();

            	incFailedTime(p1.id);
            	if(getFailedTime(p1.id) >= 2)
            	{
            		missMatchPlayerList.addAll(failedList);
            	}
            	else
            	{
            		missMatchPlayerList.add(p1);
//            		p2 = getNextPlayer(p1);
//            		if(p2 != null)
//            		{
//            			missMatchPlayerList.add(p2);
//            		}           		
            	}  
            	
            	if(!searchMatchForFailed(missMatchPlayerList, map))
            	{
            		throw new IllegalArgumentException("DeadLock, missMatchPlayerList = " + missMatchPlayerList);
            	}  
            	missMatchPlayerList.clear();
     		
            	Match match = null;
            	for(int i = 0, size = preProcessMatchList.size(); i < size; i++)
            	{
            		match = preProcessMatchList.get(i);
            		
            		match = generateMatch(match.player1.playerID, match.player2.playerID);
            		map.put(match.id, match);            		
            	}
            	
            	preProcessMatchList.clear();
            	MatchUtil.debug(String.format("[END]: p1=%2d, NO OPPONENT", p1.id));
            	return true;
            	
            	*/
            }
            Match match = generateMatch(p1, p2);
            map.put(match.id, match);
            
            matchedPlayerSet.add(p1.id);
            matchedPlayerSet.add(p2.id);
        }
	    
	    return true;
	}
	
	private Match searchOpponentInMatch(TreeMap<Integer, Match> map, Player player)
	{
		for(int key = map.lastKey(); !map.isEmpty(); key = map.lowerKey(key))
		{
			Match match = map.get(key);
			if(!MatchDataSource.getInstance().isFightBefore(player.id, match.player1.playerID))
			{
				return match;
			}
			if(!MatchDataSource.getInstance().isFightBefore(player.id, match.player2.playerID))
			{
				return match;
			}
		}
		return null;
	}
	
	public boolean searchMatchForFailed(List<Player> playerList, TreeMap<Integer, Match> map)
	{
		boolean ret = false;
		//1. rotate outside
		ret = rotateSearch(playerList, map, true);
		if(ret)
		{
			return true;
		}
		//2. rotate inside
		ret = rotateSearch(playerList, map, false);
		if(ret)
		{
			return true;
		}
		
		List params = new ArrayList(2);
    	params.add(playerList);
    	params.add(map);
    	
    	//3. perm outside
    	params.add(Boolean.TRUE);   	
    	ret = MatchUtil.perm(playerList.size(), this, params);
    	if(ret)
		{
			return true;
		}
		//4. perm inside
    	params.set(2, Boolean.FALSE);   	
    	ret = MatchUtil.perm(playerList.size(), this, params);
    	if(ret)
		{
			return true;
		}
    	return false;
	}
	
	//Rotate use
	public boolean rotateSearch(List<Player> playerList, TreeMap<Integer, Match> map, boolean isOutSide)
	{
		boolean ret = false;
		for(int i = 0, size = playerList.size(); i < size; i++)
		{
			if(isOutSide)
			{
				ret = searchMatchOutFailedList(playerList, map, null);
			}
			else
			{
				ret = searchMatchInFailedList(playerList, map, null);
			}
			if(ret)
			{
				//restore orders
				Collections.rotate(playerList, i);
				return true;
			}
			//Move forward 1 step
			Collections.rotate(playerList, -1);
		}
		return false;
	}
	//Perm use
	public boolean doOper(int[] a, List params)
	{
		restorePreProcessMatchList();
		List<Player> playerList = (List<Player>)params.get(0);
		TreeMap<Integer, Match> map = (TreeMap<Integer, Match>)params.get(1);
		Boolean bOutSide = (Boolean)params.get(2);
		
		if(bOutSide.booleanValue())
		{
			return searchMatchOutFailedList(playerList, map, a);
		}
		else
		{
			return searchMatchInFailedList(playerList, map, a);
		}
	}
	
	private boolean searchMatchOutFailedList(List<Player> playerList, TreeMap<Integer, Match> map, int a[])
	{
		restorePreProcessMatchList();
		Player player = null, opponet = null;
		for(int i = 0, size = playerList.size(); i < size; i++)
		{
			if(a != null)
			{
				player = playerList.get(a[i]);
			}
			else
			{
				player = playerList.get(i);
			}			
			
			if(matchedPlayerSet.contains(player.id))
			{
				continue;
			}
			opponet = getMatchedPlayer(player);
			
			while(opponet == null)
			{
    			if(map.size() <= 0)
    			{
    				return false;
    			}
				removePreviousMatches(Math.min(1, map.size()), map);
				opponet = getMatchedPlayer(player);
			}
			
			preProcessMatchList.add(preGenerateMatch(player, opponet));
            matchedPlayerSet.add(player.id);
            matchedPlayerSet.add(opponet.id);
		}
		
		return true;
	}
	
	//Internal search
	private boolean searchMatchInFailedList(List<Player> playerList, Map<Integer, Match> map, int a[])
	{
		restorePreProcessMatchList();
		Player player = null, opponent = null;
		int i, size = playerList.size();
		for(i = 0; i < size - 1; i++)
		{
			if(a != null)
			{
				player = playerList.get(a[i]);
			}
			else
			{
				player = playerList.get(i);
			}			
			
			if(matchedPlayerSet.contains(player.id))
			{
				continue;
			}
			opponent = null;
			for(int j = i + 1; j < size; j++)
			{
				if(a != null)
				{
					opponent = playerList.get(a[j]);
				}
				else
				{
					opponent = playerList.get(j);
				}
				if(matchedPlayerSet.contains(opponent.id))
				{
					opponent = null;
					continue;
				}
				if(MatchDataSource.getInstance().isFightBefore(player.id, opponent.id))
				{
					opponent = null;
					continue;
				}
				else
				{
					break;
				}
			}			
			if(opponent == null)
			{
				return false;
			}
			else
			{
				preProcessMatchList.add(preGenerateMatch(player, opponent));
	            matchedPlayerSet.add(player.id);
	            matchedPlayerSet.add(opponent.id);
			}
		}
		
		if( i == (size - 1) )
		{
			//Look for the opponent outside
			opponent = getMatchedPlayer(player);
			if(opponent == null)
			{
				return false;
			}
			else
			{
				preProcessMatchList.add(preGenerateMatch(player, opponent));
	            matchedPlayerSet.add(player.id);
	            matchedPlayerSet.add(opponent.id);
			}
		}
		return true;		
	}
	
	private void restorePreProcessMatchList()
	{
		Match match = null;
		for(int i = 0, size = preProcessMatchList.size(); i < size; i++)
		{
			match = preProcessMatchList.get(i);
			
            matchedPlayerSet.remove(match.player1.playerID);
            matchedPlayerSet.remove(match.player2.playerID);
		}
		
		preProcessMatchList.clear();
	}
	
	private void incFailedTime(int playerID)
	{
		if(!failedMap.containsKey(playerID))
		{
			failedMap.put(playerID, 1);
			failedList.add(MatchDataSource.getInstance().getPlayer(playerID));
		}
		else
		{
			int failed = failedMap.get(playerID);
			failed++;
			failedMap.put(playerID, failed);
		}
	}
	
	private int getFailedTime(int playerID)
	{
		Integer failed = failedMap.get(playerID);
		if(failed == null)
		{
			return 0;
		}
		return failed.intValue();
	}
	
	private int getGreaterFailedPlayer(int failed, List<Player> playerList)
	{
		int maxFailed = 0;
		Player player = null;
		for(Iterator<Map.Entry<Integer, Integer>> iter = failedMap.entrySet().iterator(); iter.hasNext(); )
		{
			Map.Entry<Integer, Integer> entry = iter.next();
			if(entry.getValue() >= failed)
			{
				if(entry.getValue() > maxFailed)
				{
					maxFailed = entry.getValue();
				}
				player = MatchDataSource.getInstance().getPlayer(entry.getKey());
				playerList.add(player);
				player = getNextPlayer(player);
				if(player != null)
				{
					playerList.add(player);
				}
			}
		}
		return maxFailed;
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
	
	private void doArrangeMatch(TreeMap<Integer, Match> map)
	{
	    Iterator<Map.Entry<Integer, SectionSelectee>> iter = null;
	    SectionSelectee ssee = null;
	    
	    do
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
	        
	        if(!failedList.isEmpty())
	        {
            	if(!searchMatchForFailed(failedList, map))
            	{
            		throw new IllegalArgumentException("DeadLock, missMatchPlayerList = " + failedList);
            	}  
            	failedList.clear();
            	failedMap.clear();
     		
            	Match match = null;
            	for(int i = 0, size = preProcessMatchList.size(); i < size; i++)
            	{
            		match = preProcessMatchList.get(i);
            		
            		match = generateMatch(match.player1.playerID, match.player2.playerID);
            		map.put(match.id, match);            		
            	}
            	
            	preProcessMatchList.clear();
	        }
	    }
	    while(matchedPlayerSet.size() != MatchDataSource.getInstance().getPlayerMap().size());
	}
	private Match generateMatch(int player1ID, int player2ID)
	{
		Player p1 = MatchDataSource.getInstance().getPlayer(player1ID);
		Player p2 = MatchDataSource.getInstance().getPlayer(player2ID);
		
		return generateMatch(p1, p2);
	}
	private Match generateMatch(Player p1, Player p2)
	{
        Match match = new Match();
        match.id = selectFirstMatchID();
        
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
	private Match preGenerateMatch(Player p1, Player p2)
	{
		Match match = new Match();
		
        match.player1 = new MatchPlayerInfo();
        match.player1.playerID = p1.id;
        
        match.player2 = new MatchPlayerInfo();
        match.player2.playerID = p2.id;
        
        return match;
	}
	
	private void removePreviousMatches(int previousCount, TreeMap<Integer, Match> map)
	{
	    MatchUtil.debug("Start removePreviousMatches, previousCount="  + previousCount);
	    this.printSeparator();
	    Match match = null;
	    int matchID = 0;
	    for(int i = 0; i < previousCount; i++)
	    {
	    	if(map.isEmpty())
	    	{
	    		throw new IllegalArgumentException("The match map is empty now");
	    	}
	    	matchID = map.lastKey();
            match = map.get(matchID);

            MatchUtil.debug("Removed: " + match.toString());
            
            map.remove(matchID);
            matchedPlayerSet.remove(match.player1.playerID);
            matchedPlayerSet.remove(match.player2.playerID);
            
            curRoundMatchIDSet.add(matchID);
	    }
//	    for(Iterator<Map.Entry<Integer, Match>> matchIter = map.entrySet().iterator(); matchIter.hasNext(); )
//        {
//            Map.Entry<Integer, Match> matchEntry = matchIter.next();
//            showMatch(matchEntry.getValue());
//        }
//	    this.printSeparator();
	    MatchUtil.debug("End removePreviousMatches, previousCount="  + previousCount);
	}
	private Match generateMatch(int p1, int p2, boolean isP1First)
	{
		Match match = new Match();
		match.id = this.selectFirstMatchID();
		
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
	
	private Player getNextPlayer(Player player)
	{
		SectionSelectee ssee = scoreSectionMap.get(player.score);
		if(ssee == null)
		{
			throw new IllegalArgumentException("[Fatal]: ssee = null, player = " + player);
		}
		Player next = ssee.getNextPlayer(player);
		if(next == null)
		{
			Integer nextScore = scoreSectionMap.higherKey(player.score);
			if(nextScore == null)
			{
//				throw new IllegalArgumentException("[Fatal]: nextScore == null, player = " + player);
				MatchUtil.debug("[Fatal]: nextScore == null, player = " + player);
				return null;
			}
			ssee = scoreSectionMap.get(nextScore);
			next = ssee.getFirstPlayer();
		}
		return next;
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
		int roundCount = 9;
		
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
