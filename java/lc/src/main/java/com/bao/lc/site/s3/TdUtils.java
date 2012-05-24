package com.bao.lc.site.s3;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.IOUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.tags.ScriptTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.lc.AppConfig;
import com.bao.lc.common.ScriptCodeFilter;
import com.bao.lc.site.s3.bean.GenericBean;
import com.bao.lc.util.MiscUtils;

public class TdUtils
{
	private static Log log = LogFactory.getLog(TdUtils.class);
	

	public static final String DEFAULT_ENTRY_DELIM = ";";

	public static final String DEFAULT_FIELD_DELIM = ",";
	
	private static Map<String, String> stationNameMap = new HashMap<String, String>();
	
	private static List<GenericBean> cardTypeList = new ArrayList<GenericBean>();
	private static List<GenericBean> ticketTypeList = new ArrayList<GenericBean>();
	private static List<GenericBean> trainClassList = new ArrayList<GenericBean>();
	private static List<GenericBean> seatClassList = new ArrayList<GenericBean>();
	private static List<GenericBean> issaveList = new ArrayList<GenericBean>();
	private static List<GenericBean> filterCatList = new ArrayList<GenericBean>();
			
	private static final String JS_ERROR_MSG_REGEX = "var message = \"(.+?)\";";
	
	static
	{
		initStationNames();
		initPassengerInfoValues();
	}
	
	private static boolean initStationNames()
	{
		//https://dynamic.12306.cn/otsweb/js/common/station_name.js?version=2.4
		InputStream is = TdUtils.class.getResourceAsStream("station_name.js");
		String content = null;
		try
		{
			content = IOUtils.toString(is, "UTF-8");
		}
		catch(IOException e)
		{
			log.error("load station_name.js failed.", e);
			return false;
		}
		
		//Find the real content
		int fromIndex = content.indexOf('\'');
		if(fromIndex >= 0)
		{
			int toIndex = content.indexOf('\'', fromIndex + 1);
			if(toIndex >= 0)
			{
				content = content.substring(fromIndex + 1, toIndex);
			}
		}
		
		String[] stations = content.split("@");
		for(String station : stations)
		{
			if(station == null || station.isEmpty())
			{
				continue;
			}
			String fields[] = station.split("\\|");
			if(fields.length < 4)
			{
				log.error("The station is not unexpected. station=" + station);
				continue;
			}
			stationNameMap.put(fields[1], fields[2]);
		}
		
		return true;
	}
	
	private static void initPassengerInfoValues()
	{
		String cardType = AppConfig.getInstance().getPropInternal("td.passenger.info.card.type");
		initHelper(cardType, ";", ",", 2, cardTypeList);
		
		String ticketType = AppConfig.getInstance().getPropInternal("td.passenger.info.ticket.type");
		initHelper(ticketType, ";", ",", 2, ticketTypeList);
		
		String trainClass = AppConfig.getInstance().getPropInternal("td.passenger.info.train.class");
		initHelper(trainClass, ";", ",", 2, trainClassList);
		
		String seatClass = AppConfig.getInstance().getPropInternal("td.passenger.info.seat.class");
		initHelper(seatClass, ";", ",", 3, seatClassList);
		
		String isSave = AppConfig.getInstance().getPropInternal("td.passenger.info.issave");
		initHelper(isSave, ";", ",", 2, issaveList);
		
		String filterCat = AppConfig.getInstance().getPropInternal("td.filter.category");
		initHelper(filterCat, ";", ",", 2, filterCatList);
		
//		String[] entries = seatClass.split(";");
//		for(int i = 0; i < entries.length; i++)
//		{
//			String[] values = entries[i].split(",");
//			
//			if(values.length != 2)
//			{
//				log.error("Unexpected values. entry=" + entries[i]);
//				continue;
//			}
//			seatClassNameList.add(values[0]);
//			
//			seatClassNameMap.put(values[0], i);
//			seatClassIndexMap.put(i, values[1]);
//		}
	}
	
	private static void initHelper(String content, String entryDelim, String valueDelim,
		int expectedFieldCount, List<GenericBean> gbList)
	{
		String[] entries = content.split(entryDelim);
		for(String entry : entries)
		{
			String[] values = entry.trim().split(valueDelim);

			if(values.length != expectedFieldCount)
			{
				log.error("Unexpected values. entry=" + entry + ", expectedFieldCount="
					+ expectedFieldCount);
				continue;
			}

			GenericBean bean = new GenericBean(Arrays.asList(values));
			gbList.add(bean);
		}
	}
	
	public static String getStationCode(String name)
	{
		return stationNameMap.get(name);
	}
	
	public static String getSeatClassValue(int seatType)
	{
//		if(!seatClassIndexMap.containsKey(seatType))
//		{
//			log.error("The seatType [" + seatType + "] doesn't exist.");
//			seatType = TrainTicketInfo.NONE_SEAT;
//		}
//		
//		return seatClassIndexMap.get(seatType);
		
		return GenericBean.getField(String.valueOf(seatType), 0, 2, seatClassList);
	}
	
	public static String getSeatClassName(int seatType)
	{
		return GenericBean.getField(String.valueOf(seatType), 0, 1, seatClassList);
	}
	
	public static List<String> getSeatClassNameList()
	{
		return GenericBean.getFieldList(1, seatClassList);
	}
	
	public static List<GenericBean> getSeatClasses()
	{
		return seatClassList;
	}
	
	
	
	public static List<GenericBean> getTrainClasses()
	{
		return trainClassList;
	}
	
	public static List<GenericBean> getCardTypes()
	{
		return cardTypeList;
	}
	public static String getCardTypeValue(String cardType)
	{
		//0: name
		//1: value
//		return cardTypeMap.get(cardType);
		return GenericBean.getField(cardType, 0, 1, cardTypeList);
	}
	
	public static List<GenericBean> getTicketTypes()
	{
		return ticketTypeList;
	}
	public static String getTicketTypeValue(String ticketType)
	{
//		return ticketTypeMap.get(ticketType);
		//0: name
		//1: value
		return GenericBean.getField(ticketType, 0, 1, ticketTypeList);
	}
	
	public static String getTrainClassValue(String trainClass)
	{
//		return trainClassNameMap.get(trainClass);
		//0: name
		//1: value
		return GenericBean.getField(trainClass, 0, 1, trainClassList);
	}
	
	public static List<String> getTrainClassNameList()
	{
		return GenericBean.getFieldList(0, trainClassList);
	}
	
	public static List<GenericBean> getSaveList()
	{
		return issaveList;
	}
	
	public static String getFilterCatName(int category)
	{
		return GenericBean.getField(String.valueOf(category), 0, 1, filterCatList);
	}

	public static String toString(List list, String delim)
	{
		if(list == null || list.isEmpty())
		{
			return "";
		}
		StringBuilder sb = new StringBuilder();
		for(int i = 0, size = list.size(); i < size; i++)
		{
			if(i > 0)
			{
				sb.append(delim);
			}
			sb.append(list.get(i));
		}
		return sb.toString();
	}

	public static List<String> toList(String content, String delim)
	{
		if(content == null || content.isEmpty())
		{
			return new ArrayList<String>(0);
		}
		String[] values = content.split(delim);
		List<String> list = new ArrayList<String>(values.length);
		for(int i = 0; i < values.length; i++)
		{
			String value = values[i].trim();
			list.add(value);
		}
		return list;
	}

	public static List<Integer> toIndexList(String content, String delim, List fullDataList)
	{
		List<String> dataList = toList(content, delim);
		
		List<Integer> indexList = new ArrayList<Integer>();
		for(String data: dataList)
		{
			int index = indexOf(data, fullDataList);
			if(index != -1)
			{
				indexList.add(index);
			}
		}
		return indexList;
	}

	public static int indexOf(String data, List fullDataList)
	{
		if(fullDataList == null || fullDataList.isEmpty())
		{
			return -1;
		}
		for(int i = 0, size = fullDataList.size(); i < size; i++)
		{
			String s = String.valueOf(fullDataList.get(i));
			if(data.equals(s))
			{
				return i;
			}
		}
		return -1;
	}
	
	public static String getJsErrorMsg(String content, String charset)
	{
		Parser parser = MiscUtils.createParser(content, charset, log);

		// error message from java script code
		NodeFilter scriptFilter = new ScriptCodeFilter(JS_ERROR_MSG_REGEX);
		// parse
		
		NodeList nodeList = null;
		try
		{
			nodeList = parser.parse(scriptFilter);
		}
		catch(ParserException e)
		{
			log.error("Failed to parse JS_ERROR_MSG_REGEX: " + JS_ERROR_MSG_REGEX, e);
			return null;
		}
		if(nodeList.size() <= 0)
		{
			log.info("Can't find matched error message: " + JS_ERROR_MSG_REGEX);
			return null;
		}
		
		ScriptTag scriptErrorMsg = (ScriptTag)nodeList.elementAt(0);
		
		String srcCode = scriptErrorMsg.getScriptCode();
		List<String> valueList = new ArrayList<String>();
		int matchCount = MiscUtils.getRegexValue(srcCode, JS_ERROR_MSG_REGEX, valueList, true, 0);
		if(matchCount <= 0)
		{
			log.error("matchCount <= 0 for " + JS_ERROR_MSG_REGEX);
			return null;
		}
		
		String errorMsg = valueList.get(1);
		log.info("getJsErrorMsg result: " + errorMsg);
		
		return errorMsg;
	}
	
	public static String getVersion()
	{
		return "1.1";
	}
}
