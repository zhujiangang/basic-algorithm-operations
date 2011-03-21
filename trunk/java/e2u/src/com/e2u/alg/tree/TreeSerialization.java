package com.e2u.alg.tree;

import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import javax.swing.tree.DefaultMutableTreeNode;

import com.e2u.bit.BitUtil;

public class TreeSerialization
{	
	public static byte[] encode(DefaultMutableTreeNode root)
	{	
		ByteBuffer byteBuffer = new ByteBuffer();
		
		LinkedList<SNode> que = new LinkedList<SNode>();
		
		SNode rootTNode = new SNode(root, 0, -1);
		que.add(rootTNode);
				
		int level = 0;
		int nodeNumber = -1;
		
		int count = 0;		
		SNode curNode = null;		
		while(!que.isEmpty())
		{
			curNode = que.removeFirst();
			byteBuffer.add(curNode.encode());
			
			if(curNode.level > level)
			{
				level = curNode.level;
				nodeNumber = 0;
			}
			else
			{
				nodeNumber++;
			}
			
			//put all the children into the queue
			count = curNode.node.getChildCount();
			for(int i = 0; i < count; i++)
			{
				DefaultMutableTreeNode node = (DefaultMutableTreeNode)curNode.node.getChildAt(i);				
				SNode tnode = new SNode(node, level + 1, nodeNumber);
				
				que.add(tnode);
			}		
		}
		return byteBuffer.getData();
	}
	public static DefaultMutableTreeNode decode(byte[] bytes)
	{
		SNode snode = SNode.decode(bytes, 0);
		DefaultMutableTreeNode root = snode.node;
		
		List<List<DefaultMutableTreeNode>> nodeListList = new ArrayList<List<DefaultMutableTreeNode>>();		
		List<DefaultMutableTreeNode> nodeList = new ArrayList<DefaultMutableTreeNode>();
		nodeListList.add(nodeList);

		int curParentLevel = snode.level;
		//root node
		nodeList.add(snode.node);
		
		for(int i = 12; i < bytes.length; i += 12)
		{	
			snode = SNode.decode(bytes, i);
			
			//The current level's storage list exist?
			if(snode.level > nodeListList.size() - 1)
			{
				nodeList = new ArrayList<DefaultMutableTreeNode>();
				nodeListList.add(nodeList);
			}
			nodeList.add(snode.node);
			
			if(snode.level == curParentLevel + 2)
			{
				//Only the parent level and current level's list data will be used
				//So the stale data can be deleted.
				nodeListList.get(curParentLevel).clear();
				
				curParentLevel++;
			}
			DefaultMutableTreeNode p = nodeListList.get(curParentLevel).get(snode.parentIndex);
			p.add(snode.node);
		}
		
		return root;
	}
	private static void breadPrint(DefaultMutableTreeNode r)
	{
		DefaultMutableTreeNode node;
		
		Enumeration enu = r.breadthFirstEnumeration();		
		while(enu.hasMoreElements())
		{
			node = (DefaultMutableTreeNode)enu.nextElement();
			System.out.print(node.getUserObject() + " ");
		}
		System.out.println();
	}
	
	/**
	 * A simple byte array wrapper class
	 */
	private static class ByteBuffer
	{
		private int offset = 0;
		private byte[] bytes = null;
		public ByteBuffer()
		{
			bytes = new byte[1024];
			offset = 0;
		}
		public void add(byte[] src)
		{
			for(int i = 0; i < src.length; i++)
			{
				bytes[offset++] = src[i];
			}
		}
		public byte[] getData()
		{
			if(offset <= 0)
			{
				return new byte[0];
			}
			byte[] r = new byte[offset];
			System.arraycopy(bytes, 0,r, 0, offset);
			return r;
		}
	}
	
	/**
	 * Serialization Node
	 */
	private static class SNode
	{
		public DefaultMutableTreeNode node;
		//node's level, start from 0
		public int level;
		//the index of the parent
		public int parentIndex;
		
		public SNode(){}
		public SNode(DefaultMutableTreeNode node, int level, int parentIndex)
		{
			this.node = node;
			this.level = level;
			this.parentIndex = parentIndex;
		}
		public byte[] encode()
		{
			byte[] bytes = new byte[12];
			BitUtil.toByte(bytes, 0, level);
			BitUtil.toByte(bytes, 4, parentIndex);
			BitUtil.toByte(bytes, 8, (Integer)(node.getUserObject()));
			return bytes;
		}
		public static SNode decode(byte[] bytes, int offset)
		{
			SNode tnode = new SNode();
			tnode.level = BitUtil.constructInt(bytes, offset + 0);
			tnode.parentIndex = BitUtil.constructInt(bytes, offset + 4);
			
			DefaultMutableTreeNode node = new DefaultMutableTreeNode();
			int userData = BitUtil.constructInt(bytes, offset + 8);
			node.setUserObject(userData);
			tnode.node = node;
			
			return tnode;
		}
	}
	
	private static DefaultMutableTreeNode buildTree()
	{
		Map<Integer, DefaultMutableTreeNode> map = new HashMap<Integer, DefaultMutableTreeNode>();
		
		DefaultMutableTreeNode node;
		for(int i = 0; i < 16; i++)
		{
			node = new DefaultMutableTreeNode(i);
			map.put(i, node);
		}

		// 0
		node = map.get(0);
		node.add(map.get(1));
		node.add(map.get(2));
		node.add(map.get(3));
		node.add(map.get(4));
		
		// 1
		node = map.get(1);
		node.add(map.get(5));
		node.add(map.get(6));
		node.add(map.get(7));
		node.add(map.get(8));
		
		// 3
		node = map.get(3);
		node.add(map.get(9));
		node.add(map.get(10));
		node.add(map.get(11));
		
		// 4
		node = map.get(4);
		node.add(map.get(12));
		node.add(map.get(13));

		// 7
		node = map.get(7);
		node.add(map.get(14));
		node.add(map.get(15));
		
		//root node
		node = map.get(0);
		
		//for gc
		map.clear();
		
		return node;
	}
	public static void test()
	{
		DefaultMutableTreeNode intputRoot = buildTree();
		breadPrint(intputRoot);
		
		byte[] bytes = TreeSerialization.encode(intputRoot);
		DefaultMutableTreeNode outputRoot = TreeSerialization.decode(bytes);
		breadPrint(outputRoot);
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		test();
	}

}
