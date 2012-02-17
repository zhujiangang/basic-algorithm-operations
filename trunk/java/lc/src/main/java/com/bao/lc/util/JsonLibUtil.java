package com.bao.lc.util;

import java.io.IOException;
import java.io.InputStream;
import java.util.Iterator;

import org.apache.commons.io.IOUtils;

import net.sf.json.JSONObject;
import net.sf.json.JSONSerializer;

public class JsonLibUtil
{
	private static String toIndent(int indent)
	{
		StringBuilder sb = new StringBuilder();
		for(int i = 0; i < indent; i++)
		{
			sb.append("  ");
		}
		return sb.toString();
	}
	public static void showJSON(JSONObject json, int indent)
	{
		for(Iterator<?> iter = json.keySet().iterator(); iter.hasNext(); )
		{
			Object key = iter.next();
			
			Object value = json.get(key);
			
			if(value instanceof JSONObject)
			{
				System.out.println(toIndent(indent) + key + "={");
				showJSON((JSONObject)value, indent + 1);
				System.out.println(toIndent(indent) + "}");
			}
			else
			{
				System.out.println(toIndent(indent) + key + "=" + value);
			}
		}
	}
	
	public static void showJSON(String content)
	{
		JSONObject json = (JSONObject) JSONSerializer.toJSON(content);

		showJSON(json, 0);
	}
	
	public static void showJSONFile(String fileName)
	{
		InputStream is = JsonLibUtil.class.getResourceAsStream(fileName);
		
		try
		{
			String jsonTxt = IOUtils.toString(is);
			showJSON(jsonTxt);
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
		
	}
}
