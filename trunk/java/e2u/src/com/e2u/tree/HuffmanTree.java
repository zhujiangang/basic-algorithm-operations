package com.e2u.tree;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import com.e2u.bit.BitUtil;

public class HuffmanTree
{
	private HuffmanNode root = null;
	private String[] encodeTable = null;
	
	public HuffmanTree()
	{
		encodeTable = new String[256];
	}
	
	public HuffmanTree(int[] weights)
	{
		if(weights == null)
		{
			throw new NullPointerException("weights is null");
		}
		if(weights.length < 256)
		{
			throw new IllegalArgumentException("The weights length is less than " + 256);
		}
		encodeTable = new String[256];
		
		byte[] elements = new byte[256];
		
		for(int i = 0; i < 256; i++)
		{
			elements[i] = (byte)i;
		}
		
		buildTree(elements, weights);
	}
	
	public HuffmanNode getRoot()
	{
		return root;
	}
	public String[] getEncodeTable()
	{
		return encodeTable;
	}
	
	public void buildTree(List<HuffmanNode> initialNodeList)
	{				
		List<HuffmanNode> leafNodeList = new ArrayList<HuffmanNode>(initialNodeList.size());
		leafNodeList.addAll(initialNodeList);
		
		HuffmanNode lc = null, rc = null, parent = null;
		
		int lastOneIndex = -1, lastTwoIndex = -1;
		while(leafNodeList.size() > 1)
		{
			Collections.sort(leafNodeList);
			
			lastOneIndex = leafNodeList.size() - 1;
			lastTwoIndex = leafNodeList.size() - 2;
			
			lc = leafNodeList.get(lastOneIndex);
			rc = leafNodeList.get(lastTwoIndex);
			
			leafNodeList.remove(lastOneIndex);
			leafNodeList.remove(lastTwoIndex);
			
			parent = new HuffmanNode(lc.weight + rc.weight);
			lc.parent = parent;
			rc.parent = parent;
			
			parent.lchild = lc;
			parent.rchild = rc;
			
			leafNodeList.add(parent);
		}
		
		leafNodeList.clear();
		root = parent;
		
		updateEncodeMap();
	}
	
	public String getPath(HuffmanNode node)
	{
		if(node == null)
		{
			return null;
		}
		StringBuilder sb = new StringBuilder();
		
		while(node.parent != null)
		{
			if(node.parent.lchild == node)
			{
				sb.append('0');
			}
			else
			{
				sb.append('1');
			}
			node = node.parent;
		}
		
		return sb.reverse().toString();
	}
	
	private void updateEncodeMap()
	{
		updateEncodeMap(root);
	}
	private void updateEncodeMap(HuffmanNode node)
	{
		if(node == null)
		{
			return;
		}
		if(node.lchild == null && node.rchild == null)
		{			
			encodeTable[BitUtil.unsigned(node.element)] = getPath(node);
			return;
		}
		updateEncodeMap(node.lchild);
		updateEncodeMap(node.rchild);
	}
	
	public void buildTree(byte[] elements, int weights[])
	{
		if(elements == null || weights == null)
		{
			throw new NullPointerException("elements or weights can't be null");
		}
		
		List<HuffmanNode> leafNodeList = new ArrayList<HuffmanNode>(elements.length);
		for(int i = 0; i < elements.length; i++)
		{
			leafNodeList.add(new HuffmanNode(elements[i], weights[i]));
		}
		
		buildTree(leafNodeList);
	}
}
