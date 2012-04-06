package com.bao.lc.site.s3.bean;

import java.util.List;


public class ListFilter implements ComparableFilter
{
	private Comparable value;
	private List values;
	public ListFilter(Comparable value, List values)
	{
		this.value = value;
		this.values = values;
	}
	@Override
	public int compareTo(Object o)
	{
		if(!(o instanceof ListFilter))
		{
			return 1;
		}
		
		ListFilter that = (ListFilter)o;
		return this.value.compareTo(that.value);
	}

	@Override
	public boolean accept(Object o)
	{
		return values.indexOf(value) >= 0;
	}
	@Override
	public Object getValue()
	{
		return value;
	}

}
