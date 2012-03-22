package com.bao.examples.re;

import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.io.IOUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import junit.framework.TestCase;

public class RegexTest extends TestCase
{
	private static Log log = LogFactory.getLog(RegexTest.class);

	private static final String fileName = "input.txt";
	private static final String JS_LOGIN_REGEX = "var isLogin(.*?)=(.+?)var u_name = '(.*?)';";

	private String fileContent = null;

	protected void setUp() throws Exception
	{
		fileContent = IOUtils.toString(new FileInputStream(fileName), "UTF-8");
	}

	public void testRegex1()
	{
		int flags = Pattern.MULTILINE | Pattern.DOTALL;
		List<String> valueList = new ArrayList<String>();
		int matchCount = getRegexValue(fileContent.trim(), JS_LOGIN_REGEX, valueList, true, flags);
		if(matchCount != 1)
		{
			log.fatal("Unexpected: matchCount=" + matchCount);
			return;
		}

		String loginStatus = valueList.get(2).trim();
		String loginName = valueList.get(3).trim();
		log.info("Javascript code: loginStatus=" + loginStatus + ", loginName=" + loginName);
		assertTrue(StringUtils.equalsIgnoreCase(loginStatus, "true") && !loginName.isEmpty());
	}

	public static int getRegexValue(String str, String regex, List<String> valueList,
		boolean allGroup, int flags)
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
}
