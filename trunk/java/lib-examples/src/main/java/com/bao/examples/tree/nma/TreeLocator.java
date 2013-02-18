package com.bao.examples.tree.nma;

import java.util.LinkedList;

import javax.swing.tree.*;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class TreeLocator 
{
	private static Log log = LogFactory.getLog(TreeLocator.class);
	
	private DefaultTreeModel treeModel = null;
	private String pathJoinStr = null;
	private MyTreeNode currNode = null;
	public TreeLocator()
	{
		this((MyTreeNode)TreeUtil.DUMMY_NODE.clone());
	}
	public TreeLocator(MyTreeNode root)
	{
		treeModel = new DefaultTreeModel(root);
		setPathJoinStr(TreeUtil.UDM_RESULT_SPLIT_JOIN_STR);
		
		currNode = root;
	}
	public DefaultTreeModel getTreeModel()
	{
		return treeModel;
	}
	public String getPathJoinStr()
	{
		return pathJoinStr;
	}
	public void setPathJoinStr(String pathJoinStr)
	{
		this.pathJoinStr = pathJoinStr;
	}
	public String addNewNode(String node)
	{
		if(node == null || node.isEmpty())
		{
			return null;
		}
		
		String[] pathIndex = node.split(TreeUtil.SPLIT_REGEX);
		
		return addNewNode(pathIndex.length);
	}
	
	public String addNewNode(int depth)
	{		
		MyTreeNode parent = getLastNode(depth - 1);
		if(parent == null)
		{
			log.error("Can't find the parent of node, depth = "  + depth);
			return null;
		}
		TreeNodeObject treeUserObject = new TreeNodeObject(parent.getChildCount() + 1);
		MyTreeNode treeNode = new MyTreeNode(treeUserObject);
		currNode = treeNode;
		parent.add(treeNode);
		
		TreePath tp = getPath(treeNode);
		return toString(tp);
	}
	
	public void setCurrNodeData(Object obj)
	{
		if(currNode == null)
		{
			throw new NullPointerException("");
		}
		TreeNodeObject treeObj = (TreeNodeObject)currNode.getUserObject();
		treeObj.setData(obj);
	}
	
	/**
	 * Get the last TreeNode in this level
	 * @param level level is from zero(0)
	 * @return
	 */
	public MyTreeNode getLastNode(int level)
	{
		MyTreeNode node = (MyTreeNode)treeModel.getRoot();
		for(int i = 1; i <= level; i++)
		{
			if(node.getChildCount() == 0)
			{
				log.error("The node has no children at level " + level);
				return null;
			}
			node = (MyTreeNode)node.getLastChild();
			if(node == null)
			{
				return null;
			}
		}
		return node;
	}
	
	public TreePath getPath(MyTreeNode node)
	{
		return getPath(node, 0);
	}
	
	public TreePath getPath(MyTreeNode node, int startLevel)
	{
		TreeNode[] treeNodes = node.getPath();
		
		MyTreeNode tempNode = null;
    	TreePath tp = null;
    	for(int i = startLevel; i < treeNodes.length; i++)
    	{
    		tempNode = (MyTreeNode)treeNodes[i];
    		
    		TreeNodeObject userObject = (TreeNodeObject)tempNode.getUserObject();
    		
    		if(tp == null)
    		{
    			tp = new TreePath(userObject);
    		}
    		else
    		{
    			tp = tp.pathByAddingChild(userObject);
    		}
    	}
    	return tp;
	}
	
	public MyTreeNode getNode(TreePath tp)
	{
		return (MyTreeNode)tp.getLastPathComponent();
	}
	public MyTreeNode getNode(String pathStr)
	{
		TreePath tp = parseTreePath(pathStr);
		return (MyTreeNode)tp.getLastPathComponent();
	}
	/**
	 * Path String in TreePath form
	 * @param pathStr
	 * @return
	 */
	public TreePath parseTreePath(String pathStr)
	{
		String[] path = pathStr.split(TreeUtil.SPLIT_REGEX);
		TreeNodeObject[] tpcs = toTreeNodeObject(path);
		
		TreePath tp = new TreePath(tpcs);
		
		return tp;
	}
	/**
	 * TreePath String form
	 * @param tp
	 * @return
	 */
	public String toString(TreePath tp)
	{
        StringBuffer sb = new StringBuffer();

        for(int counter = 0, maxCounter = tp.getPathCount(); counter < maxCounter; counter++) 
        {
            if(counter > 0)
            	sb.append(pathJoinStr);
            sb.append(tp.getPathComponent(counter));
        }
        return sb.toString();
	}
	/**
	 * Convert a path from String to TreeNodeObject form.
	 * @param aPath
	 * @return
	 */
	public TreeNodeObject toTreeNodeObject(String aPath)
	{
		TreeNodeObject tpc = new TreeNodeObject();
		tpc.index = Integer.parseInt(aPath);
		
		return tpc;
	}
	/**
	 * Convert a path list from String to TreeNodeObject form.
	 * @param path
	 * @return
	 */
	public TreeNodeObject[] toTreeNodeObject(String[] path)
	{
		TreeNodeObject[] tpcs = new TreeNodeObject[path.length];
		for(int i = 0; i < path.length; i++)
		{
			tpcs[i] = toTreeNodeObject(path[i]);
		}
		return tpcs;
	}
	
	
	public void dispose()
	{
		LinkedList<MyTreeNode> que = new LinkedList<MyTreeNode>();
		que.add((MyTreeNode)treeModel.getRoot());
		
		MyTreeNode curNode = null;
		while(!que.isEmpty())
		{
			curNode = que.removeFirst();
			//put all the children into the queue
			if(curNode.getChildCount() > 0)
			{				
				que.addAll(curNode.getChildren());
			}
			curNode.dispose();
		}		
		treeModel.setRoot(null);
		treeModel = null;
	}
}
