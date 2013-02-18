package com.bao.examples.tree.nma;

public class TreeNodeObject 
{
	public int index = 0;
	public Object data = null;
	public TreeNodeObject()
	{
		this(0, null);
	}
	public TreeNodeObject(int index)
	{
		this(index, null);
	}
	public TreeNodeObject(int index, Object data)
	{
		this.index = index;
		this.data = data;
	}
	public void setData(Object data)
	{
		this.data = data;
	}
	public String toString()
	{
		return String.valueOf(index) + "-" + ((data == null) ? "null" : data.toString());
	}
}
