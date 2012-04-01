package com.bao.lc.site.s3.bean;

import java.util.List;


public class ListElementIndex implements ComparableFilter<TrainTicketInfo, ListElementIndex>
{
	private Object value = null;
	private List valueList = null;
	
	public ListElementIndex(Object o, List valueList)
	{
		this.value = o;
		this.valueList = valueList;
	}
	@Override
	public int compareTo(ListElementIndex o)
	{
		int index1 = valueList.indexOf(value);
		int index2 = o.valueList.indexOf(o.value);
		
		if(index1 != index2)
		{
			if(index1 < 0)
			{
				return 1;
			}
			if(index2 < 0)
			{
				return -1;
			}
			return index1 - index2;
		}
		
		
		return 0;
	}
	@Override
	public boolean accept(TrainTicketInfo target)
	{
		return valueList.indexOf(value) >= 0;
	}
}
