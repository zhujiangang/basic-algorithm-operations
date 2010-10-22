package com.e2u.tree;

import java.util.*;
import com.e2u.bit.*;


public class Huffman
{
	private HuffmanNode root = null;
	private Map<Byte, String> encodeMap = new TreeMap<Byte, String>();
	private String[] encodeTable = null;
	
	private int curByte = 0;
	private int curByteBitLeft = 8;
	
	private List<Byte> encodeByteList = null;
	
	public Huffman()
	{
		root = null;
		
		curByte = 0;
		curByteBitLeft = 8;
		
		encodeTable = new String[256];
		
		encodeByteList = new LinkedList<Byte>();
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
			encodeMap.put(node.element, getPath(node));
			
			encodeTable[BitUtil.unsigned(node.element)] = getPath(node);
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
	
	public void addCompressInputData(byte[] data)
	{
		String str = null;
		
		int tempByte = 0;
		int offset = 0;
		for(int i = 0; i < data.length; i++)
		{
//			str = encodeMap.get(data[i]);
			str = encodeTable[BitUtil.unsigned(data[i])];
			
			offset = 0;
			while(offset < str.length())
			{
				if(str.length() - offset > curByteBitLeft)
				{
					tempByte = toByte(str.substring(offset, offset + curByteBitLeft));
					offset += curByteBitLeft;
					curByte |= tempByte;
					
					encodeByteList.add((byte)curByte);
					curByte = 0;				
					curByteBitLeft = 8;					
				}
				else if(str.length() - offset < curByteBitLeft)
				{
					tempByte = toByte(str.substring(offset, str.length()));
					
					curByteBitLeft = curByteBitLeft - (str.length() - offset);
					tempByte <<= curByteBitLeft;
					
					curByte |= tempByte;
					
					break;
				}
				else
				{
					tempByte = toByte(str.substring(offset, offset + curByteBitLeft));
					curByte |= tempByte;
					
					encodeByteList.add((byte)curByte);
					curByte = 0;				
					curByteBitLeft = 8;
					break;
				}
			}
		}
	}
	public byte[] finishCompress()
	{
		//process last byte
		if(curByteBitLeft == 8)
		{
			curByteBitLeft = 0;
		}
		if(curByteBitLeft < 8 && curByteBitLeft != 0)
		{
			encodeByteList.add((byte)curByte);
		}
		
		
		//output
		byte[] result = new byte[encodeByteList.size() + 1];
		result[0] = (byte)curByteBitLeft;
		
		Iterator<Byte> iter = encodeByteList.iterator();
		for(int i = 1; iter.hasNext(); i++)
		{
			result[i] = iter.next();
		}
		
		curByte = 0;
		curByteBitLeft = 8;
		encodeByteList.clear();
		return result;
	}
	
	public byte[] compress(byte[] data)
	{
		addCompressInputData(data);
		return finishCompress();		
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
			byteStr = BitUtil.toBin(data[i]);
			
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
			System.out.println(BitUtil.toHex(entry.getKey()) + "=" + entry.getValue());
		}
		
		System.out.println("======================");
		System.out.println("======================");
		for(int i = 0; i < encodeTable.length; i++)
		{
			System.out.println(BitUtil.toHex((byte)i) + "=" + encodeTable[i]);
		}
		System.out.println("======================");
		System.out.println("======================");
	}
	
	public void showPath(HuffmanNode node)
	{
		if(node == null)
		{
			return;
		}
		if(node.lchild == null && node.rchild == null)
		{			
			System.out.println(String.format("%s=%s", BitUtil.toHex(node.element), getPath(node)));
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
	
	
	
	private static void generateTestData(byte[] elements, int[] weights)
	{
		for(int i = 0; i < elements.length; i++)
		{
			elements[i] = (byte)i;			
			weights[i] = (int)(Math.random() * 1000);
		}
		for(int i = 0; i < elements.length; i++)
		{
			System.out.println(BitUtil.toHex(elements[i]) + "=" + weights[i]);
		}
		System.out.println();
	}
	
	public static void main(String[] args)
	{
//		byte[] elements = {(byte)0x30, (byte)0x31, (byte)0x32, (byte)0x33, (byte)0x34, (byte)0x35};
//		int[] weights = {120, 20, 503, 23, 9, 234};
		
		int result = toByte("01110011");
		System.out.println(result);
		System.out.println(BitUtil.toBin((byte)result));
		
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
		
		BitUtil.showHex(inputData);
		System.out.println();
		System.out.println();
		
		byte[] encoded = huffman.compress(inputData);
		BitUtil.showHex(encoded);
		System.out.println();
		System.out.println();
		
		Deflater df = new Deflater(huffman.encodeTable);
		byte[] encoded2 = huffman.compress(inputData);
		BitUtil.showHex(encoded2);
		System.out.println();
		System.out.println();
		
//		showString(huffman.compressInternal(inputData));
		
		byte[] decoded = huffman.decompress(encoded);
		BitUtil.showHex(decoded);
		System.out.println();
		System.out.println();
		
		Inflater inf = new Inflater(huffman.root);
		byte[] decoded2 = inf.decompress(encoded);
		BitUtil.showHex(decoded2);
		System.out.println();
		System.out.println();
	}
}


