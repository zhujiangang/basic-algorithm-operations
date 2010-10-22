package com.e2u.tree;

public class HuffmanNode implements Comparable<HuffmanNode>
{
	public byte element;
	public int weight;
	public HuffmanNode parent;
	public HuffmanNode lchild;
	public HuffmanNode rchild;

	public HuffmanNode(byte element, int weight)
	{
		this.element = element;
		this.weight = weight;
		this.parent = null;
		this.lchild = null;
		this.rchild = null;
	}

	public HuffmanNode(int weight)
	{
		this((byte) 0, weight);
	}

	public boolean isLeaf()
	{
		return this.lchild == null && this.rchild == null;
	}

	public boolean equals(Object obj)
	{
		if(!(obj instanceof HuffmanNode))
		{
			return false;
		}
		HuffmanNode other = (HuffmanNode) obj;
		return this.element == other.element;
	}

	public int hashCode()
	{
		return (int) element;
	}

	public int compareTo(HuffmanNode o)
	{
		int result = o.weight - weight;
		if(result == 0)
		{
			return 0;
		}
		else if(result > 0)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	
	public String toString()
	{
		return "element=" + element + ", weight=" + weight;
	}
}
