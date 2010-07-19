package com.e2u.gui;

public class GridLocator
{
	private int columns = -1;
	private int componentIndex = 0;
	
	public GridLocator(int columns)
	{
		this.columns = columns;
		this.componentIndex = 0;
	}
	public int getColumns()
	{
		return columns;
	}
	public int getGridX()
	{
		return (componentIndex % columns);
	}
	public int getGridY()
    {
    	return (componentIndex++ / columns);
    }
	public int getGridY(int gridwidth)
    {
    	int ret = componentIndex / columns;
    	componentIndex += gridwidth;
    	return ret;
    }    
	public void moveToNextPos()
    {
    	componentIndex++;
    }
}
