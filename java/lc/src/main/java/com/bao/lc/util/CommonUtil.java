package com.bao.lc.util;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.bao.lc.common.exception.ParseException;

public class CommonUtil
{
	public static int getRegexValue(String str, String regex,
		List<String> valueList, boolean allGroup)
	{
		Pattern pattern = Pattern.compile(regex);
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
		List<String> valueList, boolean allGroup) throws ParseException
	{
		int matchCount = getRegexValue(str, regex, valueList, allGroup);
		if(matchCount > 1)
		{
			throw new ParseException("Found more than 1 match. matchCount="
				+ matchCount);
		}
	}

	public static String getRegexValueOnce(String str, String regex, int index)
		throws ParseException
	{
		List<String> valueList = new ArrayList<String>();
		getRegexValueOnce(str, regex, valueList, true);

		if(valueList.size() <= index)
		{
			throw new ParseException(
				"Can't find the specified group at index: " + index + " of "
					+ valueList.size() + " groups");
		}

		return valueList.get(index);
	}
}
