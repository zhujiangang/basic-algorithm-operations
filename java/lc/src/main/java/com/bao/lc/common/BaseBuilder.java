package com.bao.lc.common;

public class BaseBuilder<T> implements Builder<T>
{
	private T value = null;
	
	public BaseBuilder(T value)
	{
		this.value = value;
	}
	@Override
	public T build()
	{
		return this.value;
	}
}
