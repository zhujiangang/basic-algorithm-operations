package com.e2u.tree;

import com.e2u.bit.BitUtil;

public class CGFileHeader
{
	public static final int HEADER_SIZE = 16 + 16 + 16 + 4 * 256; //1072 in bytes
	public static final int FILE_NAME_SIZE_BY_BYTE = 32;
	public static final int WEIGHT_TABLE_SIZE_BY_INT = 256;
	
	public int headerName;
	public int headerVer;
	public int originalLength;
	public int contentOffset;
	
	//32 bytes
	public byte[] originalFileName;
	
	//256
	public int[] weightTable;
	
	public CGFileHeader()
	{
		headerName = 0x474C4354; //GLCT
		headerVer = 0x01000000; //1.0.0.0
		
		contentOffset = HEADER_SIZE; //Default
		
		originalFileName = new byte[FILE_NAME_SIZE_BY_BYTE];
		for(int i = 0; i < FILE_NAME_SIZE_BY_BYTE; i++)
		{
			originalFileName[i] = 0;
		}
		weightTable = new int[WEIGHT_TABLE_SIZE_BY_INT];
	}
	public CGFileHeader(byte[] in, int offset)
	{
		originalFileName = new byte[FILE_NAME_SIZE_BY_BYTE];
		weightTable = new int[WEIGHT_TABLE_SIZE_BY_INT];
		
		headerName = BitUtil.constructInt(in, offset);
		offset += 4;
		
		headerVer = BitUtil.constructInt(in, offset);
		offset += 4;
		
		originalLength = BitUtil.constructInt(in, offset);
		offset += 4;
		
		contentOffset = BitUtil.constructInt(in, offset);
		offset += 4;
		
		for(int i = 0; i < FILE_NAME_SIZE_BY_BYTE; i++)
		{
			originalFileName[i] = in[offset + i];
		}		
		offset += FILE_NAME_SIZE_BY_BYTE;
		
		for(int i = 0; i < WEIGHT_TABLE_SIZE_BY_INT; i++)
		{
			weightTable[i] = BitUtil.constructInt(in, offset);
			offset += 4;
		}
	}
	
	public byte[] getBytes()
	{
		byte[] result = new byte[HEADER_SIZE];
		
		int offset = 0;
		
		BitUtil.toByte(result, offset, headerName);
		offset += 4;
		
		BitUtil.toByte(result, offset, headerVer);
		offset += 4;
	
		BitUtil.toByte(result, offset, originalLength);
		offset += 4;
		
		BitUtil.toByte(result, offset, contentOffset);
		offset += 4;
		
		for(int i = 0; i < FILE_NAME_SIZE_BY_BYTE; i++)
		{
			result[offset + i] = originalFileName[i];
		}		
		offset += FILE_NAME_SIZE_BY_BYTE;
		
		for(int i = 0; i < WEIGHT_TABLE_SIZE_BY_INT; i++)
		{
			BitUtil.toByte(result, offset, weightTable[i]);
			offset += 4;
		}
		
		return result;
	}
}
