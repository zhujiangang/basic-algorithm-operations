package com.e2u.tree;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import com.e2u.bit.BitUtil;

public class Deflater
{
	private String[] encodeTable = null;

	private int curByte = 0;
	private int curByteBitLeft = 8;

	private List<Byte> encodeByteList = null;

	public Deflater(String[] codeTable)
	{		
		encodeTable = new String[256];
		System.arraycopy(codeTable, 0, encodeTable, 0, encodeTable.length);
		
		curByte = 0;
		curByteBitLeft = 8;
	
		encodeByteList = new LinkedList<Byte>();
	}
	
	private void reset()
	{
		curByte = 0;
		curByteBitLeft = 8;
		
		encodeByteList.clear();
	}
	public void addCompressInputData(byte[] data, int len)
	{
		String str = null;

		int tempByte = 0;
		int offset = 0;
		for(int i = 0; i < len; i++)
		{
			str = encodeTable[BitUtil.unsigned(data[i])];

			offset = 0;
			while(offset < str.length())
			{
				if(str.length() - offset > curByteBitLeft)
				{
					tempByte = toByte(str.substring(offset, offset + curByteBitLeft));
					offset += curByteBitLeft;
					curByte |= tempByte;

					encodeByteList.add((byte) curByte);
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

					encodeByteList.add((byte) curByte);
					curByte = 0;
					curByteBitLeft = 8;
					break;
				}
			}
		}
	}

	public byte[] finishCompress()
	{
		// process last byte
		if(curByteBitLeft == 8)
		{
			curByteBitLeft = 0;
		}
		if(curByteBitLeft < 8 && curByteBitLeft != 0)
		{
			encodeByteList.add((byte) curByte);
		}

		// output
		byte[] result = new byte[encodeByteList.size() + 1];
		result[0] = (byte) curByteBitLeft;

		Iterator<Byte> iter = encodeByteList.iterator();
		for(int i = 1; iter.hasNext(); i++)
		{
			result[i] = iter.next();
		}

		reset();
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
				throw new IllegalArgumentException("Invalid huffman code "
					+ str);
			}
		}
		return result;
	}
}
