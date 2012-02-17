package com.bao.lc.common;

public class IDNamePair
{
	private int id = 0;
	private String name = null;
	
	public IDNamePair()
	{
		this.id = 0;
		this.name = null;
	}
	
	public IDNamePair(int id, String name)
	{
		this.id = id;
		this.name = name;
	}
	
	public int getId()
	{
		return id;
	}

	public void setId(int id)
	{
		this.id = id;
	}

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public String toString()
	{
		return String.valueOf(id) + ": " + name;
	}
	
	public int hashCode()
	{
		return id;
	}
	
	public boolean equals(Object obj)
	{
		if(obj instanceof IDNamePair)
		{
			IDNamePair other = (IDNamePair)obj;
			return id == other.id;
		}
		
		return false;
	}
}
