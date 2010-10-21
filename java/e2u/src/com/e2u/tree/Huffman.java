package com.e2u.tree;

import java.util.*;


public class Huffman
{
	private HuffmanNode root = null;
	private Map<Byte, String> encodeMap = new TreeMap<Byte, String>();
	
	public Huffman()
	{
		root = null;
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
		
		updateEncodeMap(root);
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
			encodeMap.put(node.element, getPath(node));
			return;
		}
		updateEncodeMap(node.lchild);
		updateEncodeMap(node.rchild);
	}
	
	private String compressInternal(byte[] data)
	{
		StringBuilder sb = new StringBuilder();
		
		String str = null;
		for(int i = 0; i < data.length; i++)
		{
			str = encodeMap.get(data[i]);
			
			sb.append(str);
		}
		return sb.toString();
	}
	
	public byte[] compress(byte[] data)
	{
		List<Byte> byteList = new LinkedList<Byte>();
		
		String str = null;
		int curByte = 0;
		int curByteBitLeft = 8;
		
		int curLeftByte = 0;
		int offset = 0;
		for(int i = 0; i < data.length; i++)
		{
			str = encodeMap.get(data[i]);
			
			offset = 0;
			while(offset < str.length())
			{
				if(str.length() - offset > curByteBitLeft)
				{
					curLeftByte = toByte(str.substring(offset, offset + curByteBitLeft));
					offset += curByteBitLeft;
					curByte |= curLeftByte;
					
					byteList.add((byte)curByte);
					curByte = 0;				
					curByteBitLeft = 8;					
				}
				else if(str.length() - offset < curByteBitLeft)
				{
					curLeftByte = toByte(str.substring(offset, str.length()));
					
					curByteBitLeft = curByteBitLeft - (str.length() - offset);
					curLeftByte <<= curByteBitLeft;
					
					curByte |= curLeftByte;
					
					break;
				}
				else
				{
					curLeftByte = toByte(str.substring(offset, offset + curByteBitLeft));
					curByte |= curLeftByte;
					
					byteList.add((byte)curByte);
					curByte = 0;				
					curByteBitLeft = 8;
					break;
				}
			}
		}
		
		if(curByteBitLeft < 8)
		{
			byteList.add((byte)curByte);
		}
		
		byte[] result = new byte[byteList.size() + 1];
		int i = 1;
		for(Iterator<Byte> iter = byteList.iterator(); iter.hasNext(); )
		{
			result[i++] = iter.next();
		}
		result[0] = (byte)curByteBitLeft;
		if(curByteBitLeft == 8)
		{
			result[0] = 0;
		}
		byteList.clear();
		return result;
	}
	
	public byte[] decompress(byte[] data)
	{
		int emptyBitInLastByte = data[0];
		
		List<Byte> byteList = new LinkedList<Byte>();
		
		String byteStr = null;
		
		int offset = 0;
		int len = 0;
		
		HuffmanNode curNode = root;
		for(int i = 1; i < data.length; i++)
		{
			byteStr = toBin(data[i]);
			
			offset = 0;
			
			if(i == data.length - 1)
			{
				len = byteStr.length() - emptyBitInLastByte;
			}
			else
			{
				len = byteStr.length();
			}
			
			while(offset < len)
			{
				if(byteStr.charAt(offset) == '0')
				{
					curNode = curNode.lchild;
				}
				else if(byteStr.charAt(offset) == '1')
				{
					curNode = curNode.rchild;
				}
				offset++;
				
				if(curNode.isLeaf())
				{
					byteList.add(curNode.element);
					curNode = root;
				}			
			}	
		}
		
		byte[] result = new byte[byteList.size()];
		int i = 0;
		for(Iterator<Byte> iter = byteList.iterator(); iter.hasNext(); )
		{
			result[i++] = iter.next();
		}
		byteList.clear();
		return result;
	}
	
	private static int toByte(String str)
	{
		int result = 0;
		char c;
		for(int i = 0, len = str.length(); i < len; i++)
		{
			c = str.charAt(i);
			if(c == '0')
			{
				result = (result << 1);
			}
			else if(c == '1')
			{
				result = (result << 1) + 1;
			}
			else
			{
				throw new IllegalArgumentException("Invalid huffman code " + str);
			}
		}
		return result;
	}
	
	public void showPath()
	{
//		showPath(root);
		
		Iterator<Map.Entry<Byte, String>> iter = encodeMap.entrySet().iterator();
		while(iter.hasNext())
		{
			Map.Entry<Byte, String> entry = iter.next();
			System.out.println(toHex(entry.getKey()) + "=" + entry.getValue());
		}
	}
	
	public void showPath(HuffmanNode node)
	{
		if(node == null)
		{
			return;
		}
		if(node.lchild == null && node.rchild == null)
		{			
			System.out.println(String.format("%s=%s", toHex(node.element), getPath(node)));
			return;
		}
		showPath(node.lchild);
		showPath(node.rchild);
	}
	public void buildTree(byte[] elements, int weights[])
	{
		if(elements == null || weights == null)
		{
			throw new NullPointerException("elements or weights can't be null");
		}
		
		List<HuffmanNode> leafNodeList = new ArrayList<HuffmanNode>(elements.length * 2 + 1);
		for(int i = 0; i < elements.length; i++)
		{
			leafNodeList.add(new HuffmanNode(elements[i], weights[i]));
		}
		
		buildTree(leafNodeList);
	}
	
	public static String toHex(byte b)
	{
		int value = unsigned(b);
		
		char[] chars = new char[2];
		chars[0] = toHex(value >> 4);
		chars[1] = toHex(value & 0x0F);
		
		return new String(chars);
	}
	
	public static String toBin(byte b)
	{
		int value = unsigned(b);
		
		int mask = 0x80;
		char[] result = new char[8];
		for(int i = 0; i < 8; i++)
		{
			if( (value & mask) != 0 )
			{
				result[i] = '1';
			}
			else
			{
				result[i] = '0';
			}
			mask >>= 1;
		}		
		return new String(result);		
	}
	
	public static char toHex(int x)
	{
		if(x >= 0 && x <= 9)
		{
			return (char)('0' + x);
		}
		else if(x >= 10 && x <= 15)
		{
			return (char)('A' + (x - 10));
		}
		throw new IllegalArgumentException(x + " is out of bound");
	}
	
	public static int unsigned(byte b)
	{
		return ((int)b) & 0xFF;
	}
	public static void showString(String str)
	{
		int lineCount = 8;
		for(int i = 0; i < str.length(); i += lineCount)
		{
			if(i + lineCount > str.length())
			{
				System.out.println(str.substring(i, str.length()));
				break;
			}
			else
			{
				System.out.println(str.substring(i, i + lineCount));
			}			
		}
	}
	public static void showHex(byte[] bytes)
	{
		for(int i = 0; i < bytes.length; i++)
		{
			System.out.print(toHex(bytes[i]) + " ");
			if(i % 16 == 15)
			{
				System.out.println();
			}
		}
		System.out.println();
	}
	
	private static void generateTestData(byte[] elements, int[] weights)
	{
		for(int i = 0; i < elements.length; i++)
		{
			elements[i] = (byte)i;			
			weights[i] = (int)(Math.random() * 1000);
		}
		for(int i = 0; i < elements.length; i++)
		{
			System.out.println(toHex(elements[i]) + "=" + weights[i]);
		}
		System.out.println();
	}
	
	public static void main(String[] args)
	{
//		byte[] elements = {(byte)0x30, (byte)0x31, (byte)0x32, (byte)0x33, (byte)0x34, (byte)0x35};
//		int[] weights = {120, 20, 503, 23, 9, 234};
		
		int result = toByte("01110011");
		System.out.println(result);
		System.out.println(toBin((byte)result));
		
		int len = 256;
		byte[] elements = new byte[len];
		int[] weights = new int[len];
		
		generateTestData(elements, weights);
		
		Huffman huffman = new Huffman();
		huffman.buildTree(elements, weights);
		
		huffman.showPath();
		
		len = 1025;
		byte[] inputData = new byte[len];
		int index = 0;
		for(int i = 0; i < len; i++)
		{
			index = (int)(Math.random() * elements.length);
			inputData[i] = elements[index];
		}
		
		showHex(inputData);
		System.out.println();
		System.out.println();
		
		byte[] encoded = huffman.compress(inputData);
		showHex(encoded);
		System.out.println();
		System.out.println();
		
//		showString(huffman.compressInternal(inputData));
		
		byte[] decoded = huffman.decompress(encoded);
		showHex(decoded);
		System.out.println();
		System.out.println();
	}
	
	public static class HuffmanNode implements Comparable<HuffmanNode>
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
			this((byte)0, weight);
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
			HuffmanNode other = (HuffmanNode)obj;
			return this.element == other.element;
		}
		
		public int hashCode()
		{
			return (int)element;
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
	}
}


