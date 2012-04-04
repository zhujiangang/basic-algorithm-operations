package com.bao.lc.bean;

public class BasicIDValuePair implements IDValuePair
{
	private int id;
	private ResultCode rc;
	private String value;
	public BasicIDValuePair(int id, String value)
	{
		this.id = id;
		this.value = value;
		
		this.rc = null;
	}
	
	public BasicIDValuePair(ResultCode rc, String value)
	{
		this.id = rc.getID();
		
		this.rc = rc;
		this.value = value;
	}
	
	@Override
	public int getID()
	{
		return this.id;
	}

	@Override
	public String getValue()
	{
		return this.value;
	}
	
	@Override
	public String toString()
	{
		if(this.rc != null)
		{
			return String.format("rc=%s,value=%s", rc, value);
		}
		else
		{
			return String.format("id=%d,value=%s", id, value);
		}
	}
}
