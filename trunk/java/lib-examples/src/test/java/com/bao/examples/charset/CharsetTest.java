package com.bao.examples.charset;

import java.nio.charset.Charset;
import java.util.SortedMap;

import junit.framework.TestCase;

public class CharsetTest extends TestCase
{
	public void testCharset()
	{
		SortedMap<String,Charset> map = Charset.availableCharsets();
		System.out.println(map);
	}
}
