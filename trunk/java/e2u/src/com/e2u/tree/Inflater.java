package com.e2u.tree;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import com.e2u.bit.*;

public class Inflater
{
	private HuffmanNode root = null;
	
	private HuffmanNode curNode = null;
	private int emptyBitInLastByte = -1;
	private int prevInputLastByte = Integer.MIN_VALUE;
	
	private List<Byte> decodeByteList = null;
	
	public Inflater(HuffmanNode root)
	{
		this.root = root;
		curNode = this.root;
		emptyBitInLastByte = -1;
		prevInputLastByte = Integer.MIN_VALUE;
		
		decodeByteList = new LinkedList<Byte>();
	}
	
	private void reset()
	{
		curNode = this.root;
		emptyBitInLastByte = -1;
		prevInputLastByte = Integer.MIN_VALUE;
		
		decodeByteList.clear();
	}
	
	public void addDecompressInputData(byte[] data, int len)
	{				
		if(prevInputLastByte != Integer.MIN_VALUE)
		{
			decode((byte)prevInputLastByte);
		}
		prevInputLastByte = data[len - 1];
		
		int index = 0;
		if(emptyBitInLastByte < 0)
		{
			emptyBitInLastByte = data[0];
			index++;
		}
		for( ; index < len - 1; index++)
		{
			decode(data[index]);
		}
	}
	
	public byte[] finishDecompress()
	{
		if(prevInputLastByte != Integer.MIN_VALUE)
		{
			decode((byte)prevInputLastByte, 8 - emptyBitInLastByte);
			
			if(curNode != root)
			{
				throw new IllegalArgumentException("Format Error");
			}
		}
		
		
		//output
		byte[] result = new byte[decodeByteList.size()];
		
		Iterator<Byte> iter = decodeByteList.iterator();
		for(int i = 0; iter.hasNext(); i++)
		{
			result[i] = iter.next();
		}
		
		reset();
		
		return result;
	}
	public byte[] decompress(byte[] data)
	{
		addDecompressInputData(data, data.length);
		return finishDecompress();
	}
	private void decode(byte b)
	{
		decode(b, 8);
	}
	private void decode(byte b, int len)
	{
		String byteStr = BitUtil.toBin(b);
		
		for(int offset = 0; offset < len; offset++)
		{
			if(byteStr.charAt(offset) == '0')
			{
				curNode = curNode.lchild;
			}
			else if(byteStr.charAt(offset) == '1')
			{
				curNode = curNode.rchild;
			}
			
			if(curNode.isLeaf())
			{
				decodeByteList.add(curNode.element);
				curNode = root;
			}			
		}
	}
}
