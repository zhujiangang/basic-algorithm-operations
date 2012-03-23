package com.bao.lc.util;

import java.io.File;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.script.ScriptContext;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;
import javax.script.SimpleBindings;
import javax.swing.JOptionPane;

import org.apache.commons.lang.ObjectUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.builder.CompareToBuilder;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.htmlparser.lexer.Lexer;
import org.htmlparser.lexer.Page;
import org.htmlparser.nodes.TextNode;
import org.htmlparser.tags.TableColumn;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserFeedback;
import org.htmlparser.Node;
import org.htmlparser.Parser;

import com.bao.lc.common.LoggerFeedback;
import com.bao.lc.common.exception.ParseException;

public class MiscUtils
{
	private static Log log = LogFactory.getLog(MiscUtils.class);
	
	public static final DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");

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

	public static void getRegexValueOnce(String str, String regex, List<String> valueList,
		boolean allGroup, int flags) throws ParseException
	{
		int matchCount = getRegexValue(str, regex, valueList, allGroup, flags);
		if(matchCount != 1)
		{
			throw new ParseException("Found match not 1. matchCount=" + matchCount);
		}
	}

	public static String getRegexValueOnce(String str, String regex, int index, int flags)
		throws ParseException
	{
		List<String> valueList = new ArrayList<String>();
		getRegexValueOnce(str, regex, valueList, true, flags);

		if(valueList.size() <= index)
		{
			throw new ParseException("Can't find the specified group at index: " + index + " of "
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

		byte[] UTF8_BOM = { (byte) 0xEF, (byte) 0xBB, (byte) 0xBF };

		for(Iterator<Object> iter = prop.keySet().iterator(); iter.hasNext();)
		{
			key = (String) iter.next();
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

			// update new key if needed
			if(!newKey.isEmpty() && !newKey.startsWith("#"))
			{
				prop.setProperty(newKey, value);
			}

			// Remove the old key
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

	public static int convertDayOfWeek(int x)
	{
		if(x < 1 || x > 7)
		{
			throw new IllegalArgumentException("Out of range 1-7");
		}
		if(x == 7)
		{
			return Calendar.SUNDAY;
		}
		return x + 1;
	}

	public static void updateCalendar(Calendar cal, int week, int dayOfWeek)
	{
		cal.add(Calendar.WEEK_OF_YEAR, week - 1);
		cal.set(Calendar.DAY_OF_WEEK, convertDayOfWeek(dayOfWeek));
	}

	/**
	 * 
	 * @param dateStr
	 *            : examples, 2012-3-8 8:00:00, 2012-3-27 10:59:00
	 * @return
	 */
	public static Calendar toCalendar(String dateStr)
	{
		String regex = "(\\d+?)-(\\d+?)-(\\d+?) (\\d+?):(\\d+?):(\\d+?)";
		return toCalendar(dateStr, regex);
	}
	
	public static Calendar toCalendar(String dateStr, String regex)
	{
		List<String> valueList = new ArrayList<String>();

		int matchCount = getRegexValue(dateStr, regex, valueList, true, 0);
		if(matchCount != 1)
		{
			log.error("Failed to format str to date: " + dateStr);
			return null;
		}
		Calendar cal = Calendar.getInstance();
		cal.set(Calendar.MILLISECOND, 0);

		int index = 1;
		cal.set(Calendar.YEAR, toInt(valueList.get(index++)));
		cal.set(Calendar.MONTH, toInt(valueList.get(index++)) - 1);
		cal.set(Calendar.DAY_OF_MONTH, toInt(valueList.get(index++)));
		cal.set(Calendar.HOUR_OF_DAY, toInt(valueList.get(index++)));
		cal.set(Calendar.MINUTE, toInt(valueList.get(index++)));
		cal.set(Calendar.SECOND, toInt(valueList.get(index++)));

		return cal;
	}

	public static boolean isSameDay(Calendar cal1, Calendar cal2)
	{
		return compareDay(cal1, cal2) == 0;
	}
	
	public static int compareDay(Calendar cal1, Calendar cal2)
	{
		CompareToBuilder cb = new CompareToBuilder();
		cb.append(cal1.get(Calendar.YEAR), cal2.get(Calendar.YEAR));
		cb.append(cal1.get(Calendar.MONTH), cal2.get(Calendar.MONTH));
		cb.append(cal1.get(Calendar.DAY_OF_MONTH), cal2.get(Calendar.DAY_OF_MONTH));
		return cb.toComparison();
	}

	public static long diff(Calendar cal1, Calendar cal2)
	{
		return cal1.getTimeInMillis() - cal2.getTimeInMillis();
	}

	public static long diffWithNow(Calendar cal)
	{
		return diff(Calendar.getInstance(), cal);
	}

	public static void sleep(int interval, Random rand)
	{
		if(interval == 0)
		{
			return;
		}
		if(interval < 0 && rand == null)
		{
			return;
		}

		long sleepTime = 0;
		if(interval < 0)
		{
			sleepTime = rand.nextInt(Math.abs(interval));
		}
		else
		{
			sleepTime = interval;
		}

		log.debug("Sleep " + sleepTime + " seconds.");

		sleepTime *= 1000;

		try
		{
			Thread.sleep(sleepTime);
		}
		catch(Exception e)
		{
			// ignore
		}
	}
	
	public static void sleep(long millis)
	{
		if(millis <= 0)
		{
			return;
		}
		
		log.debug("[Sleep] start: " + millis + " (ms).");
		try
		{
			Thread.sleep(millis);
		}
		catch(InterruptedException e)
		{
			log.warn("InterruptedException when sleep.", e);
		}
		log.debug("[Sleep]  stop: " + millis + " (ms).");
	}

	public static String escapeJS(String s)
	{
		ScriptEngineManager sem = new ScriptEngineManager();
		ScriptEngine engine = sem.getEngineByExtension("js");

		SimpleBindings bindings = new SimpleBindings();
		bindings.put("str", s);

		engine.setBindings(bindings, ScriptContext.ENGINE_SCOPE);

		try
		{
			Object res = engine.eval("escape(str)");
			return ObjectUtils.toString(res);
		}
		catch(ScriptException e)
		{
			log.error("Failed to eval JS. s = " + s, e);
		}

		return s;
	}
	
	public static String randJS()
	{
		ScriptEngineManager sem = new ScriptEngineManager();
		ScriptEngine engine = sem.getEngineByExtension("js");

		try
		{
			Object res = engine.eval("Math.random()");
			return ObjectUtils.toString(res);
		}
		catch(ScriptException e)
		{
			log.error("Failed to eval JS", e);
		}

		return String.valueOf(Math.random());
	}

	public static String encode(final String content, final String encoding)
	{
		if(encoding == null)
		{
			throw new IllegalArgumentException("Empty charset");
		}
		try
		{
			return URLEncoder.encode(content, encoding);
		}
		catch(UnsupportedEncodingException problem)
		{
			throw new IllegalArgumentException(problem);
		}
	}

	public static String decode(final String content, final String encoding)
	{
		if(encoding == null)
		{
			throw new IllegalArgumentException("Empty charset");
		}
		try
		{
			return URLDecoder.decode(content, encoding);
		}
		catch(UnsupportedEncodingException problem)
		{
			throw new IllegalArgumentException(problem);
		}
	}

	public static String getTableColumnText(TableColumn tableColumn)
	{
		String result = "";

		do
		{
			if(tableColumn == null)
			{
				break;
			}
			NodeList children = tableColumn.getChildren();
			if(children == null || children.size() <= 0)
			{
				break;
			}

			for(int i = 0, size = children.size(); i < size; i++)
			{
				Node node = children.elementAt(i);
				if(node instanceof TextNode)
				{
					String text = StringUtils.strip(((TextNode) node).getText(), " \t\r\n");
					if(!StringUtils.isEmpty(text))
					{
						return text;
					}
				}
			}
		}
		while(false);

		return result;
	}

	public static String toString(Map<?, ?> map)
	{
		if(map == null)
		{
			return null;
		}
		StringBuilder sb = new StringBuilder();
		sb.append("size=[").append(map.size()).append("] ");
		for(Map.Entry<?, ?> entry : map.entrySet())
		{
			sb.append(entry.getKey()).append("=[").append(entry.getValue()).append("] ");
		}
		return sb.toString();
	}

	public static Parser createParser(String text, String charset, Log logger)
	{
		Lexer lexer = new Lexer(new Page(text, charset));
		ParserFeedback feedback = new LoggerFeedback(logger);

		Parser parser = new Parser(lexer, feedback);
		return parser;
	}
	
	public static String toString(Calendar cal)
	{
		return dateFormat.format(cal.getTime());
	}
}
