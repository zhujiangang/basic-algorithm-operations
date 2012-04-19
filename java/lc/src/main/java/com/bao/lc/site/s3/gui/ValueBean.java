package com.bao.lc.site.s3.gui;

public interface ValueBean<T>
{
	public static final String VALUE_PROPERTY = "ValueBean.value";

	public void setValue(T value);

	public T getValue();
}
