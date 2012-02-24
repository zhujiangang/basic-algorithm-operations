package com.bao.lc.common;

import java.io.*;
import java.util.Arrays;

import org.apache.commons.io.HexDump;

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
			int readCount = read(bytes);
			
			if(readCount < 0)
			{
				return;
			}
			
			super.count = readCount;
			
			if(readCount < 3)
			{
				super.pos = 0;
			}
			else if(readCount == 3)
			{
				if(Arrays.equals(bytes, UTF8_BOM))
				{
					System.out.println(super.pos);
				}
				else
				{
					super.pos = 0;
				}
			}
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) throws Exception
	{
		UTF8BufferedInputStream bis = new UTF8BufferedInputStream(new FileInputStream("a.txt"));
		
		BufferedReader reader = new BufferedReader(new InputStreamReader(bis, "UTF-8"));
		
		int lineCount = 0;
		String line = null;
		
		while((line = reader.readLine()) != null)
		{
			lineCount++;
			
			System.out.println("[" + lineCount + "]: [" + line + "]");
			HexDump.dump(line.getBytes("UTF-8"), 0, System.out, 0);
		}
	}
}
