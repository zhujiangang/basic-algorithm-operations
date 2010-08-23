package com.e2u.sort;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class SectionSelectee
{
    public List<Player> reqFirstPlayerList = null;
    public List<Player> reqSecondPlayerList = null;
    
    public SectionSelectee()
    {
        reqFirstPlayerList = new ArrayList<Player>();
        reqSecondPlayerList = new ArrayList<Player>();
    }
    
    public void addPlayer(Player player)
    {
        if(player.requireFirstThisRound() > 0)
        {
            reqFirstPlayerList.add(player);
        }
        else
        {
            reqSecondPlayerList.add(player);
        }
    }
    public void sort(Comparator<Player> comparator)
    {
        if(reqFirstPlayerList.size() > 1)
        {
            Collections.sort(reqFirstPlayerList, comparator);
        }
        if(reqSecondPlayerList.size() > 1)
        {
            Collections.sort(reqSecondPlayerList, comparator);
        }
    }
    
    public boolean isBalance()
    {
        if(reqFirstPlayerList.size() > 0 && reqSecondPlayerList.size() > 0)
        {
            return true;
        }
        return false;
    }
    
    public List<Player> getLessPlayerList()
    {
        if(reqFirstPlayerList.size() <= reqSecondPlayerList.size())
        {
            return reqFirstPlayerList;
        }
        else
        {
            return reqSecondPlayerList;
        }
    }
    public List<Player> getMorePlayerList()
    {
        if(reqFirstPlayerList.size() > reqSecondPlayerList.size())
        {
            return reqFirstPlayerList;
        }
        else
        {
            return reqSecondPlayerList;
        }
    }
    
    public Player getFirstPlayer()
    {
    	List<Player> lessPlayerList = getLessPlayerList();
    	if(!lessPlayerList.isEmpty())
    	{
    		return lessPlayerList.get(0);
    	}
    	else
    	{
    		return getMorePlayerList().get(0);
    	}
    }
    public Player getNextPlayer(Player player)
    {
    	List<Player> lessPlayerList = getLessPlayerList();
    	List<Player> morePlayerList = getMorePlayerList();
    	
    	int index = lessPlayerList.indexOf(player);
    	
    	//Doesn't exist in less list
    	if(index == -1)
    	{
    		index = morePlayerList.indexOf(player);
    		//Doesn't exist in more list either.
    		if(index == -1)
    		{
    			throw new IllegalArgumentException("[Fatal]: Player doesn't exist in this section, player = " + player);
    		}
    		//last one, no next
    		if(index == morePlayerList.size() - 1)
    		{
    			return null;
    		}
    		else
    		{
    			return morePlayerList.get(index + 1);
    		}
    	}
    	else
    	{
    		//last one
    		if(index == lessPlayerList.size() - 1)
    		{
    			if(morePlayerList.isEmpty())
    			{
    				return null;
    			}
    			else
    			{
    				return morePlayerList.get(0);
    			}
    		}
    		return lessPlayerList.get(index + 1);
    	}
    }
    
    public String toString()
    {
        StringBuffer sb = new StringBuffer();
        if(reqFirstPlayerList.size() <= reqSecondPlayerList.size())
        {
            for(int i = 0, size = reqFirstPlayerList.size(); i < size; i++)
            {
                sb.append(reqFirstPlayerList.get(i));
                sb.append("\n");
            }
            for(int i = 0, size = reqSecondPlayerList.size(); i < size; i++)
            {
                sb.append(reqSecondPlayerList.get(i));
                sb.append("\n");
            }
        }
        else
        {
            for(int i = 0, size = reqSecondPlayerList.size(); i < size; i++)
            {
                sb.append(reqSecondPlayerList.get(i));
                sb.append("\n");
            }
            for(int i = 0, size = reqFirstPlayerList.size(); i < size; i++)
            {
                sb.append(reqFirstPlayerList.get(i));
                sb.append("\n");
            }
        }
        return sb.toString();
    }
}
