package com.e2u.sort;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;
import java.util.TreeSet;

public class MatchSort
{
	private int curRound = MatchUtil.COMMON_INVALID;
	private int playerCount = MatchUtil.COMMON_INVALID;
	private int roundCount = MatchUtil.COMMON_INVALID;
	private int minorScoreMode = MatchUtil.MINOR_SCORE_OPPONENT_MODE;
	private int matchIDGenerator = 0;
	
	private Player[] playerSortBuf = null;
	
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
		
		playerSortBuf = new Player[this.playerCount];
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
	public void initPlayerSortBuf()
	{
		Collection<Player> players = MatchDataSource.getInstance().getPlayerMap().values();
		
		int i = 0;
		for(Iterator<Player> iter = players.iterator(); iter.hasNext();)
		{
			Player player = iter.next();
			playerSortBuf[i++] = player;
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
		
		//Indicate if the player has been selected.
		boolean[] flags = new boolean[playerSortBuf.length];
		for(int i = 0; i < flags.length; i++)
		{
			flags[i] = false;
		}
		
		Map<Integer, Match> map = new TreeMap<Integer, Match>();
		int count = 0;
		int p1 = 0;
		int p2;
		for(p1 = 0; count < playerCount; p1++)
		{
			if(flags[p1])
			{
				continue;
			}
			//Select p2
			for(p2 = p1 + 1; p2 < playerSortBuf.length; p2++)
			{
				if(!flags[p2])
				{
					break;
				}
			}
			
			//Is fight before?
			while(MatchDataSource.getInstance().isFightBefore(playerSortBuf[p1].id, playerSortBuf[p2].id))
			{
				p2++;
				for(; p2 < playerSortBuf.length; p2++)
				{
					if(!flags[p2])
					{
						break;
					}
				}
				if(p2 >= playerSortBuf.length)
				{
					throw new IllegalArgumentException("[Fatal Error]: can't find a suitable player 2: p1 = " + p1);
				}
			}
			
			//p1 and p2 doesn't meet before
			boolean isP1LastFirst = playerSortBuf[p1].isFirstPlayerInLastMatch();
			boolean isP2LastFirst = playerSortBuf[p2].isFirstPlayerInLastMatch();
			//Perfect
			if( (isP1LastFirst && !isP2LastFirst) || (!isP1LastFirst && isP2LastFirst) )
			{
				flags[p1] = true;
				flags[p2] = true;
				Match match = generateMatch(playerSortBuf[p1].id, playerSortBuf[p2].id, !isP1LastFirst);
				map.put(match.id, match);
			}
			//p1 and p2 has the same first hand in the last match
			else
			{
				int oldp2 = p2;
				
				while(true)
				{
					//Find a same score opponent for p1
					p2++;
					for(; p2 < playerSortBuf.length; p2++)
					{
						if(!flags[p2])
						{
							break;
						}
					}
					//No more available p2
					if(p2 >= playerSortBuf.length)
					{
						p2 = oldp2;
						break;
					}
					//The score of new p2 is less than the old p2
					else if(playerSortBuf[p2].score != playerSortBuf[oldp2].score)
					{
						p2 = oldp2;
						break;
					}
					//meeted
					else if(MatchDataSource.getInstance().isFightBefore(playerSortBuf[p1].id, playerSortBuf[p2].id))
					{
						continue;
					}
					else if(playerSortBuf[p2].isFirstPlayerInLastMatch() == isP1LastFirst)
					{
						continue;
					}
					else
					{
						break;
					}
				}
				
				isP2LastFirst = playerSortBuf[p2].isFirstPlayerInLastMatch();
				
				flags[p1] = true;
				flags[p2] = true;
				if( (isP1LastFirst && !isP2LastFirst) || (!isP1LastFirst && isP2LastFirst) )
				{
					Match match = generateMatch(playerSortBuf[p1].id, playerSortBuf[p2].id, !isP1LastFirst);
					map.put(match.id, match);
				}
				else
				{
					//prefer little
					if(MatchUtil.littleEndian(curRound))
					{
						Match match = generateMatch(playerSortBuf[p1].id, playerSortBuf[p2].id, true);
						map.put(match.id, match);
					}
					else
					{
						Match match = generateMatch(playerSortBuf[p1].id, playerSortBuf[p2].id, false);
						map.put(match.id, match);
					}
				}
			}
			
			count += 2;
		}
		
		MatchDataSource.getInstance().getMatchMap().put(Integer.valueOf(this.curRound), map);
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
		initPlayerSortBuf();
		Arrays.sort(playerSortBuf, new PlayerComparator(curRound));
		if(MatchUtil.isDebug())
		{
			MatchUtil.printSeparator();
			for(int i = 0; i < playerSortBuf.length; i++)
			{
				showPlayer(playerSortBuf[i]);
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
	
	public static void main(String args[])
	{
		int playerCount = 10;
		int roundCount = 6;
		
		MatchSort matchSort = new MatchSort(playerCount, roundCount);
		matchSort.initPlayerInformation(playerCount);
		
		matchSort.startArrange();
		
		
		for(int i = 0; i < roundCount; i++)
		{
			MatchUtil.printSeparator();
			matchSort.showArrangeTable();
			
			matchSort.playMath();
			
			MatchUtil.printSeparator();
			matchSort.showResult();
			
			//not last round match
			if(i < roundCount - 1)
			{
				matchSort.arrange();
			}
		}
		MatchUtil.printSeparator();
		
		matchSort.sortCurrentPlayers();
	}
}
