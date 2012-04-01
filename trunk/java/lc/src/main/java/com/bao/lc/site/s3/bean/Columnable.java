package com.bao.lc.site.s3.bean;


public interface Columnable
{
	ComparableFilter<?, ?> getColumn(int column, Object ... args);
}
