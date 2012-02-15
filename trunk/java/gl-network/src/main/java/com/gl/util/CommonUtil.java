package com.gl.util;

import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CommonUtil
{
	public static int getRegexValue(String str, String regex, List<String> valueList, int[] groupIndexes)
	{
		Pattern pattern = Pattern.compile(regex);
		Matcher matcher = pattern.matcher(str);
		
		int matchCount = 0;
		while(matcher.find())
		{
			matchCount++;
			
			int groupCount = matcher.groupCount();
			for(int i = 0; i < groupIndexes.length; i++)
			{
				if(groupIndexes[i] > groupCount)
				{
					//ERROR
					return 0;
				}
				valueList.add(matcher.group(groupIndexes[i]));
			}
		}
		
		return matchCount;
	}
	
	public static String getRegexValue(String str, String regex, int groupIndex)
	{
		String result = null;
		Pattern pattern = Pattern.compile(regex);
		Matcher matcher = pattern.matcher(str);
		
		int matchCount = 0;
		while(matcher.find())
		{
			matchCount++;
			
			int groupCount = matcher.groupCount();
			if(groupIndex > groupCount)
			{
				//ERROR
				continue;
			}
			
			result = matcher.group(groupIndex);
			
			if(groupCount > 1)
			{
				for(int i = 1; i <= groupCount; i++)
				{
					System.out.println(matcher.group(i));
				}
			}
		}
		
		if(matchCount > 1)
		{
			return null;
		}
		return result;
	}
}
