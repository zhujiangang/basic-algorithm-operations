package com.bao.lc.site.s3.bean;

import java.util.List;


public class ListElementCount implements ComparableFilter<TrainTicketInfo, ListElementCount>
{
	private List<Integer> indexCountList = null;
	private int min = 0;
	
	public ListElementCount(List<Integer> indexCountList, int min)
	{
		this.indexCountList = indexCountList;
		this.min = min;
	}
	
	/**
	 * The first one is the smaller one.
	 */
	@Override
	public int compareTo(ListElementCount o)
	{
		if(o == null)
		{
			return -1;
		}
		
		for(int i = 0, size = this.indexCountList.size(); i < size; i++)
		{
			int thisCount = this.indexCountList.get(i);
			int thatCount = o.indexCountList.get(i);
			
			if(thisCount != thatCount)
			{
				if(thisCount >= min && thatCount >= min)
				{
					return thisCount - thatCount;
				}
				else if(thisCount >= min)
				{
					return -1;
				}
				else if(thatCount >= min)
				{
					return 1;
				}
			}
		}
		
		return 0;
	}
	
	@Override
	public boolean accept(TrainTicketInfo target)
	{
		for(int i = 0, size = this.indexCountList.size(); i < size; i++)
		{
			int thisCount = this.indexCountList.get(i);
			if(thisCount >= min)
			{
				return true;
			}
		}
		return false;
	}
}
