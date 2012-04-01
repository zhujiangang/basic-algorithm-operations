package com.bao.lc.util;

public interface Filter<T>
{
	boolean accept(T target);
}
