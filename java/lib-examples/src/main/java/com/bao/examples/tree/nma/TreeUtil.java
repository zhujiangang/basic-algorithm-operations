package com.bao.examples.tree.nma;

import javax.swing.tree.*;

public class TreeUtil 
{
    public static final String UDM_RESULT_SPLIT_JOIN_STR = ".";
    public static final String SPLIT_REGEX = "\\.";
    
    public static final TreeNodeObject DUMMY_USER_OBJECT = new TreeNodeObject();
    
    public static final MyTreeNode DUMMY_NODE = new MyTreeNode(DUMMY_USER_OBJECT);
    
	public static String toString(TreePath path)
    {
        StringBuffer tempSpot = new StringBuffer();

        for(int counter = 0, maxCounter = path.getPathCount(); counter < maxCounter; counter++) 
        {
            if(counter > 0)
                tempSpot.append(TreeUtil.UDM_RESULT_SPLIT_JOIN_STR);
            tempSpot.append(path.getPathComponent(counter));
        }
        return tempSpot.toString();
    }
    public static TreePath parseTreePath(String pathStr)
    {
    	if(pathStr == null || pathStr.isEmpty())
    	{
    		return null;
    	}
    	String[] pathNodes = pathStr.split(SPLIT_REGEX);    	
    	
    	if(pathNodes.length == 0)
    	{
    		return null;
    	}
    	
    	TreePath tp = null;
    	for(int i = 0; i < pathNodes.length; i++)
    	{
    		int value = Integer.parseInt(pathNodes[i]);
    		
    		TreeNodeObject tpComponent = new TreeNodeObject(value);
    		
    		if(tp == null)
    		{
    			tp = new TreePath(tpComponent);
    		}
    		else
    		{
    			tp = tp.pathByAddingChild(tpComponent);
    		}
    	}
    	return tp;
    }

}
