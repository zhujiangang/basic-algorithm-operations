package com.e2u.sort;

import java.util.Comparator;

public class ComparatorFactory
{
    public static Comparator<Player> getComparatorForMatchPlayer(int round)
    {
        return new ComparatorForMatchPlayer(round);
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
}
