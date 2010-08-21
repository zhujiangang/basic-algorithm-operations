package com.e2u.sort;

import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class ComparatorFactory
{
    private static Comparator<Integer> scoreComparator = null;
    private static Comparator<Player> selectFirstComparator = null;
    
    private static Map<Integer, Comparator<Player>> selectFirstMap = new HashMap<Integer, Comparator<Player>>();
    public static Comparator<Player> getComparatorForMatchPlayer(int round)
    {
        return new ComparatorForMatchPlayer(round);
    }
    
    public static Comparator<Player> getComparatorInOneSection(int round)
    {
        return new ComparatorInOneSectionPlayer(round);
    }
    
    public static Comparator<Player> getComparator4SelectFirst(int round)
    {
        Comparator<Player> result = null;
        if(selectFirstMap.containsKey(round))
        {
            result = selectFirstMap.get(round);
        }
        else
        {
            result = new Comparator4SelectFirst(round);
            selectFirstMap.put(round, result);
        }
        return result;
    }
    public static Comparator<Integer> getComparator4Score()
    {
        if(scoreComparator == null)
        {
            scoreComparator = new Comparator4Score();
        }
        return scoreComparator;
    }
    
    public static class Comparator4Score implements Comparator<Integer>
    {
        public int compare(Integer s1, Integer s2)
        {
            return s2.intValue() - s1.intValue();
        }
    }
    
    public static class ComparatorForMatchPlayer implements Comparator<Player>
    {
        private int curRound = 0;
        public ComparatorForMatchPlayer(int round)
        {
            this.curRound = round;
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

            //who has more second hands
            ret = p1.firstHandBalance - p2.firstHandBalance;
            if(ret != 0)
            {
            	//who has more first hands who is more front
            	if(p1.firstHandBalance < 0 && p2.firstHandBalance < 0)
            	{
            		ret = (-1) * ret;
            	}
                return ret;
            }
            
            //lian xian
            ret = p1.seqFirstHand - p2.seqFirstHand;
            if(ret != 0)
            {
            	if( !(p1.seqFirstHand > 0 && p2.seqFirstHand > 0) )
            	{
            		ret = (-1) * ret;
            	}
            	return ret;
            }
            
            
            ret = p1.id - p2.id;
            if(MatchUtil.littleEndian(curRound))
            {
            	ret = (-1) * ret;
            }   
            return ret;
        }
    }
    
    
    
    public static class ComparatorInOneSectionPlayer implements Comparator<Player>
    {
        private int curRound = 0;
        public ComparatorInOneSectionPlayer(int round)
        {
            this.curRound = round;
        }
        public int compare(Player p1, Player p2)
        {
            return compareHelper(p2, p1);
        }
        //p1 has better score than p2, return > 0
        private int compareHelper(Player p1, Player p2)
        {
            if(p1.score != p2.score || p1.requireFirstThisRound() != p2.requireFirstThisRound())
            {
                String msg = String.format("[Error]: Not in one section, p1{score=%2d, f=%d}, " +
                        "p2{score=%2d, f=%d}", p1.score, p1.requireFirstThisRound(), 
                        p2.score, p2.requireFirstThisRound());
                throw new IllegalArgumentException(msg);
            }
            
            int ret = 0;
            
            ret = Math.abs(p1.firstHandBalance) - Math.abs(p2.firstHandBalance);       
            if(ret != 0)
            {
                return ret;
            }
            
            ret = Math.abs(p1.seqFirstHand) - Math.abs(p2.seqFirstHand);
            if(ret != 0)
            {
                return ret;
            }
            ret = p1.id - p2.id;
            if(MatchUtil.littleEndian(curRound))
            {
                ret *= (-1);
            }
            
            return ret;
        }
    }
    
    public static class Comparator4SelectFirst implements Comparator<Player>
    {
        private int curRound = 0;
        public Comparator4SelectFirst(int round)
        {
            this.curRound = round;
        }
        public int compare(Player p1, Player p2)
        {
            return compareHelper(p2, p1);
        }
        
        //who first, who bigger
        private int compareHelper(Player p1, Player p2)
        {
            int ret = 0;
            
            ret = p1.firstHandBalance - p2.firstHandBalance;
            if(ret != 0)
            {
                ret *= (-1);
                return ret;
            }
            
            ret = p1.seqFirstHand - p2.seqFirstHand;
            if(ret != 0)
            {
                ret *= (-1);
                return ret;
            }
            
            ret = p1.id - p2.id;
            if(MatchUtil.littleEndian(curRound))
            {
                ret *= (-1);
            }
            
            return ret;
        }
    }
}
