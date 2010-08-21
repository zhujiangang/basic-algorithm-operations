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
