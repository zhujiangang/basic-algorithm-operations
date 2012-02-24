package com.bao.lc.common;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

public class UTF8BufferedInputStream extends BufferedInputStream
{
	private static final byte[] UTF8_BOM = {(byte)0xEF, (byte)0xBB, (byte)0xBF};
	
	public UTF8BufferedInputStream(InputStream in)
	{
		super(in);
		checkBom();
	}
	
	public UTF8BufferedInputStream(InputStream in, int size) 
	{
		super(in, size);
		if(size < 3)
		{
			buf = new byte[3];
		}
		
		checkBom();
	}
	
	protected void checkBom()
	{
		byte[] bytes = new byte[3];
		
		try
		{
			int nread = read(bytes, 0, bytes.length);
			
			//EOF
			if(nread < 0)
			{
				return;
			}
			
			/**
			 * Reset the cursor to the start position if meets ONE of the below 2 condition:
			 * 
			 * 1. There's less than 3 bytes read from input stream
			 * 2. The first 3 bytes of the input stream is NOT UTF-8 BOM
			 */
			if( nread < 3 || ((nread == 3) && !Arrays.equals(bytes, UTF8_BOM)) )
			{
				super.pos = 0;
			}
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
	}
}
