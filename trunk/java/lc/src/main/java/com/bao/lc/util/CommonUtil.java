package com.bao.lc.util;

import java.io.File;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Properties;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.JOptionPane;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpHost;
import org.apache.http.client.utils.URIUtils;

import com.bao.lc.common.exception.ParseException;

public class CommonUtil
{
	private static Log log = LogFactory.getLog(CommonUtil.class); 
	
	public static int getRegexValue(String str, String regex,
		List<String> valueList, boolean allGroup, int flags)
	{
		Pattern pattern = Pattern.compile(regex, flags);
		Matcher matcher = pattern.matcher(str);

		int matchCount = 0;
		while(matcher.find())
		{
			matchCount++;

			if(allGroup)
			{
				for(int i = 0, groupCount = matcher.groupCount(); i <= groupCount; i++)
				{
					valueList.add(matcher.group(i));
				}
			}
			else
			{
				valueList.add(matcher.group());
			}
		}

		return matchCount;
	}

	public static void getRegexValueOnce(String str, String regex,
		List<String> valueList, boolean allGroup, int flags) throws ParseException
	{
		int matchCount = getRegexValue(str, regex, valueList, allGroup, flags);
		if(matchCount != 1)
		{
			throw new ParseException("Found match not 1. matchCount="
				+ matchCount);
		}
	}

	public static String getRegexValueOnce(String str, String regex, int index, int flags)
		throws ParseException
	{
		List<String> valueList = new ArrayList<String>();
		getRegexValueOnce(str, regex, valueList, true, flags);

		if(valueList.size() <= index)
		{
			throw new ParseException(
				"Can't find the specified group at index: " + index + " of "
					+ valueList.size() + " groups");
		}

		return valueList.get(index);
	}
	
	public static String getRegexValueOnce(String str, String regex, int index)
		throws ParseException
	{
		return getRegexValueOnce(str, regex, index, 0);
	}
	
	public static int toInt(String str)
	{
		int result = 0;
		try
		{
			result = Integer.parseInt(str);
		}
		catch(NumberFormatException e)
		{
			log.error("failed to convert string [" + str + "] to integer.", e);
		}
		
		return result;
	}
	
	public static void trimUtf8Bom(Properties prop)
	{
		String key = null;
		byte[] bytes = null;
		boolean found = false;
		
		byte[] UTF8_BOM = {(byte)0xEF, (byte)0xBB, (byte)0xBF};
		
		for(Iterator<Object> iter = prop.keySet().iterator(); iter.hasNext(); )
		{
			key = (String)iter.next();
			try
			{
				bytes = key.getBytes("UTF-8");
			}
			catch(UnsupportedEncodingException e)
			{
				bytes = key.getBytes();
			}
			
			
			if(bytes.length >= 3)
			{
				found = true;
				for(int i = 0; i < 3; i++)
				{
					if(bytes[i] != UTF8_BOM[i])
					{
						found = false;
						break;
					}
				}
				if(found)
				{
					break;
				}
			}
		}
		
		if(found)
		{
			String newKey = null;
			try
			{
				newKey = new String(bytes, 3, bytes.length - 3, "UTF-8");
			}
			catch(UnsupportedEncodingException e)
			{
				newKey = key;
			}
			
			String value = prop.getProperty(key);
			
			//update new key if needed
			if( !newKey.isEmpty() && !newKey.startsWith("#"))
			{
				prop.setProperty(newKey, value);
			}
			
			//Remove the old key
			prop.remove(key);
		}
	}
	
	public static String getValidationCode(String filePath)
	{
		File file = new File(filePath);
		String fileURL = null;

		try
		{
			fileURL = file.toURI().toURL().toString();
		}
		catch(MalformedURLException e)
		{
			log.error("Failed to convert filePath [" + filePath + "] to URL.", e);
			fileURL = "file:///" + filePath;
		}
		String message = String.format(
			"<html><img src=\"%s\" width=\33\" height=\55\"><br><center>%s</center><br></html>",
			fileURL, "Please input validation code:");
		
		String result = JOptionPane.showInputDialog(null, message);
		return result;
	}
	
	public static String normalizeRelativeURL(String str, HttpHost host)
	{
		String result = str;
		do
		{
			if(host == null)
			{
				break;
			}

			try
			{
				URI uri = new URI(str);
				// Only process relative URI
				if(!uri.isAbsolute())
				{
					URI newUri = URIUtils.rewriteURI(uri, host);
					result = newUri.toString();

					if(log.isDebugEnabled())
					{
						log.debug(String.format("normalizeRelativeURL from [%s] to [%s].", str,
							result));
					}
				}
			}
			catch(URISyntaxException e)
			{
				log.error(
					String.format("Failed to normalizeRelativeURL url=[%s], host=[%s]", str, host),
					e);
			}
		}
		while(false);

		return result;
	}
}