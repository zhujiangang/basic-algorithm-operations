package com.bao.examples.uri;

import java.net.URI;
import java.net.URLDecoder;
import java.net.URLEncoder;

import junit.framework.TestCase;

public class URLEncDecTest extends TestCase
{
	protected String[] data = { "http://www.yahoo.com/user/default.aspx", "http://www.abc.com/徐工",
		"http://www.ac/%u5F90%u5EFA%u7389", "?!=()#%&", "Visit W3School!" };

	protected String[] encoding = { "GBK", "GB2312", "UTF-8", "ASCII", "ISO-8859-1" };

	protected void setUp() throws Exception
	{
	}

	public void testEncode() throws Exception
	{
		String orig, encoded, decoded;
		for(int i = 0; i < encoding.length; i++)
		{
			System.out.println("\nEncoding: " + encoding[i]);
			for(int j = 0; j < data.length; j++)
			{
				orig = data[j];
				encoded = URLEncoder.encode(orig, encoding[i]);
				System.out.println(encoded);

				decoded = URLDecoder.decode(encoded, encoding[i]);
				assertTrue(decoded.equals(orig));
			}
		}
	}

	public void testDecode() throws Exception
	{
		for(int i = 0; i < data.length; i++)
		{
			System.out.println(URLDecoder.decode(data[i], "UTF-8"));
		}
	}

	public void testURIToString() throws Exception
	{
		for(int i = 0; i < data.length; i++)
		{
			URI uri = new URI(data[i]);
			System.out.println(uri.toString());
		}
	}

	public void testURIToASCIIString() throws Exception
	{
		for(int i = 0; i < data.length; i++)
		{
			URI uri = new URI(data[i]);
			System.out.println(uri.toASCIIString());
		}
	}
}
