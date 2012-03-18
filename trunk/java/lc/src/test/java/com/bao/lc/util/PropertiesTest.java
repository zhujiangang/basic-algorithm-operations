package com.bao.lc.util;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Properties;

import org.apache.commons.io.IOUtils;

import com.bao.lc.common.UTF8BufferedInputStream;

import junit.framework.TestCase;

public class PropertiesTest extends TestCase
{
	private static final String[] params = { "/res-utf8.properties", "UTF-8",
		"/res-utf8-nobom.properties", "UTF-8", "/res-gb2312.properties",
		"GB2312", };

	public void testReadFile()
	{
		Properties prop = new Properties();
		for(int i = 0; i < params.length; i += 2)
		{
			prop.clear();
			assertTrue(readFile(params[i], params[i + 1], false, prop));
			MiscUtils.trimUtf8Bom(prop);

			validation(prop);
		}
	}

	public void testReadFile2()
	{
		Properties prop = new Properties();
		for(int i = 0; i < params.length; i += 2)
		{
			prop.clear();
			assertTrue(readFile(params[i], params[i + 1], true, prop));

			validation(prop);
		}
	}

	private void validation(Properties prop)
	{
		assertEquals(prop.size(), 3);
		for(int j = 1; j <= 3; j++)
		{
			String key = "test" + j;
			assertTrue(prop.containsKey(key));
			System.out.println(key + "=" + prop.getProperty(key));
		}
	}

	private boolean readFile(String file, String charsetName,
		boolean processUTF8, Properties prop)
	{
		boolean result = false;

		InputStreamReader reader = null;
		InputStream is = null;
		try
		{
			is = getClass().getResourceAsStream(file);
			if(processUTF8 && "UTF-8".equalsIgnoreCase(charsetName))
			{
				is = new UTF8BufferedInputStream(is);
			}
			reader = new InputStreamReader(is, charsetName);

			prop.load(reader);
			result = true;
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		finally
		{
			IOUtils.closeQuietly(reader);
			IOUtils.closeQuietly(is);
		}

		return result;
	}
}
