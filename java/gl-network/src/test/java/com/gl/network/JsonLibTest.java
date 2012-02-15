package com.gl.network;

import com.gl.util.JsonLibUtil;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

public class JsonLibTest extends TestCase
{
	/**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public JsonLibTest( String testName )
    {
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( JsonLibTest.class );
    }

    /**
     * Rigourous Test :-)
     */
    public void testJsonLib1()
    {
    	String files[] = {"json1.txt", "json2.txt", "json3.txt", "json4.txt", "json5.txt"};
    	
    	for(int i = 0; i < files.length; i++)
    	{
    		JsonLibUtil.showJSONFile("/" + files[i]);
    	}
        assertTrue( true );
    }
}
