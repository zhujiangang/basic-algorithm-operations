package com.bao.examples.charset;

import java.io.OutputStream;

import org.apache.commons.io.HexDump;

import junit.framework.TestCase;

public class UnicodeTest extends TestCase
{
	private static final String TEST_STRING = "中文123";
	private static final String[] CHARSETS = { "GB2312", "GBK", "ISO-8859-1", "US-ASCII", "UTF-16",
		"UTF-16BE", "UTF-16LE", "UTF-32", "UTF-32BE", "UTF-32LE", "UTF-8"};

	public void testDump() throws Exception
	{
		byte[] bytes = null;
		OutputStream stdout = System.out;
		
		bytes = internal(TEST_STRING);
		System.out.printf("[%12s]: ", "Internal");
		HexDump.dump(bytes, 0, stdout, 0);
//		System.out.println();
		
		for(int i = 0; i < CHARSETS.length; i++)
		{
			bytes = TEST_STRING.getBytes(CHARSETS[i]);
//			System.out.print("[" + CHARSETS[i] + "]: ");
			System.out.printf("[%12s]: ", CHARSETS[i]);
			HexDump.dump(bytes, 0, stdout, 0);
//			System.out.println();
			
			String s = new String(bytes, CHARSETS[i]);
			if(!s.equals(TEST_STRING))
			{
				System.out.println("F");
			}
			else
			{
				System.out.println("T");
			}
		}
	}
	
	private byte[] internal(String s)
	{
		byte[] bytes = new byte[s.length() * 2];
		for(int i = 0, length = s.length(); i < length; i++)
		{
			char ch = s.charAt(i);
			bytes[i * 2] = getByte(ch, false);
			bytes[i * 2 + 1] = getByte(ch, true);
		}
		return bytes;
	}
	
	private byte getByte(char ch, boolean hi)
	{
		if(hi)
		{
			return (byte)((ch & 0xFF00) >> 8);
		}
		else
		{
			return (byte)(ch & 0x00FF);
		}
	}
}
