package com.bao.examples.tree.nma;

import java.util.Vector;

import javax.swing.tree.DefaultMutableTreeNode;

public class MyTreeNode extends DefaultMutableTreeNode
{
	private static final long serialVersionUID = 1802123836510467770L;
	
	public MyTreeNode()
	{
		super();
	}
	public MyTreeNode(Object userObject)
	{
		super(userObject);
	}
	public MyTreeNode(Object userObject, boolean allowsChildren)
	{
		super(userObject, allowsChildren);
	}
	public Vector getChildren()
	{
		return children;
	}
	public void dispose()
	{
		if(children != null)
		{
			children.clear();
			children = null;
		}		
		parent = null;
		userObject = null;		
	}
}
