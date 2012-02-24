package com.bao.lc.util;

import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import org.apache.commons.io.HexDump;
import org.apache.commons.io.IOUtils;

import junit.framework.TestCase;

public class PropertiesTest extends TestCase
{
	public void testReadFile()
	{
		String params[] = { "/res-utf8.properties", "UTF-8", "/res-gb2312.properties", "GB2312"};

		Properties prop = new Properties();
		for(int i = 0; i < params.length; i += 2)
		{
			prop.clear();
			assertTrue(loadFile(params[i], params[i+1], prop));
			CommonUtil.trimUtf8Bom(prop);
			
			System.out.println(prop);
			
			assertEquals(prop.size(), 3);
			for(int j = 1; j <= 3; j++)
			{
				String key = "test" + j;
				assertTrue(prop.containsKey(key));
				System.out.println(key + "=" + prop.getProperty(key));
			}
			
			try
			{
				prop.store(new FileOutputStream("aaa" + i + ".txt"), "!!!comments?!!!");
			}
			catch(Exception e)
			{
				e.printStackTrace();
			}
		}
	}
	
	private boolean loadFile(String file, String charsetName, Properties prop)
	{
		boolean result = false;
		try
		{
			InputStreamReader reader = new InputStreamReader(getClass().getResourceAsStream(file), charsetName);
			prop.load(reader);
			
			result = true;
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
		return result;
	}
	
	public void testReadFile2()
	{
		String params[] = { "/res-utf8.properties", "UTF-8"};
		
		List<String> lineList = new ArrayList<String>();
		assertTrue(readFile(params[0], params[1], lineList));
		
		for(int i = 0, size = lineList.size(); i < size; i++)
		{
			System.out.println(lineList.get(i));
		}
	}
	
	private boolean readFile(String file, String charsetName, List<String> lineList)
	{
		boolean result = false;
		BufferedReader reader = null;
		try
		{
			reader = new BufferedReader(new InputStreamReader(getClass().getResourceAsStream(file), charsetName));
			
			String line = null;
			while( (line = reader.readLine()) != null )
			{
				lineList.add(line);
				
				//First Line
				if(lineList.size() == 1)
				{
					byte[] bytes = line.getBytes("UTF-8"/*"ISO-8859-1"*/);
					HexDump.dump(bytes, 0, System.out, 0);
				}
			}
			
			result = true;
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		finally
		{
			IOUtils.closeQuietly(reader);
		}
		
		return result;
	}
}
