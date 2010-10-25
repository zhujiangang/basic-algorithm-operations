package com.e2u.tree;

import java.util.ArrayList;
import java.util.List;

import com.e2u.bit.BitUtil;
import com.e2u.util.Heap;

public class HuffmanTree
{
	//The huffman tree root node
	private HuffmanNode root = null;
	
	//The byte encode table, which contains the codes of byte 0-255 
	private String[] encodeTable = null;
	
	/**
	 * Default
	 */
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
	public void buildTree(List<HuffmanNode> initialNodeList)
	{
		Heap<HuffmanNode> hp = new Heap<HuffmanNode>(initialNodeList);
		
		HuffmanNode lc = null, rc = null, parent = null;
		
		while(hp.size() > 1)
		{
			lc = hp.remove();
			rc = hp.remove();
			
			parent = new HuffmanNode(lc.weight + rc.weight);
			parent.lchild = lc;
			parent.rchild = rc;
			
			lc.parent = parent;
			rc.parent = parent;
			
			hp.insert(parent);
		}
		
		root = hp.remove();
		
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
	
	public HuffmanNode getRoot()
	{
		return root;
	}
	public String[] getEncodeTable()
	{
		return encodeTable;
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
}
