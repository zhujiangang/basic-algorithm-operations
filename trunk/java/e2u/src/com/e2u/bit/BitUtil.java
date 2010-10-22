package com.e2u.bit;

public class BitUtil
{
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
			if((value & mask) != 0)
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
			return (char) ('0' + x);
		}
		else if(x >= 10 && x <= 15)
		{
			return (char) ('A' + (x - 10));
		}
		throw new IllegalArgumentException(x + " is out of bound");
	}

	public static int unsigned(byte b)
	{
		return ((int) b) & 0xFF;
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
	
	public static int constructInt(byte[] in, int offset) 
	{
		int ret = ((int) in[offset + 3] & 0xff);
		ret = (ret << 8) | ((int) in[offset + 2] & 0xff);
		ret = (ret << 8) | ((int) in[offset + 1] & 0xff);
		ret = (ret << 8) | ((int) in[offset + 0] & 0xff);
		return (ret);
	}
	
	public static void toByte(byte[] out, int offset, int x)
	{
		out[offset] = (byte)(x & 0xFF);
		out[offset + 1] = (byte)((x >> 8) & 0xFF);
		out[offset + 2] = (byte)((x >> 16) & 0xFF);
		out[offset + 3] = (byte)((x >> 24) & 0xFF);
	}
	
	public static void main(String[] args)
	{
		byte[] in = {'T', 'C', 'L', 'G'};
		
		int result = constructInt(in, 0);
		
		System.out.println(Integer.toHexString(result));
		
		toByte(in, 0, result);
		
		for(int i = 0; i < in.length; i++)
		{
			System.out.println(toHex(in[i]));
		}
	}
}
