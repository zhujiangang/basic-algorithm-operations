package com.bao.lc.client;

import java.util.Iterator;
import java.util.List;

import org.apache.commons.io.IOUtils;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

public class CnetGetterTest extends TestCase
{
	/**
	 * Create the test case
	 * 
	 * @param testName
	 *            name of the test case
	 */
	public CnetGetterTest(String testName)
	{
		super(testName);
	}

	/**
	 * @return the suite of tests being tested
	 */
	public static Test suite()
	{
		return new TestSuite(CnetGetterTest.class);
	}

	/**
	 * Rigourous Test :-)
	 */
	public void testGet1()
	{
		String location = "/com/bao/lc/client/urls.txt";
		
		List<?> lineList = null;
		try
		{
			lineList = IOUtils.readLines(CnetGetterTest.class.getResourceAsStream(location));
		}
		catch(Exception e)
		{
			e.printStackTrace();
			fail("Failed to load resource: " + location);
		}
		
		for(Iterator<?> iter = lineList.iterator(); iter.hasNext(); )
		{
			String url = (String)iter.next();
			
			System.out.println(url);
		}
	}
}
