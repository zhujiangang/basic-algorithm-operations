package com.bao.lc.util;

import java.util.List;

public interface SortFilter<T> extends Filter<T>
{
	public int getCategory();

	public List getValues();
}
