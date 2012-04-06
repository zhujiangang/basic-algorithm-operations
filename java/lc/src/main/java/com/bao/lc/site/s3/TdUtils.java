package com.bao.lc.site.s3;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.io.IOUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.AppConfig;
import com.bao.lc.site.s3.bean.TrainTicketInfo;

public class TdUtils
{
	private static Log log = LogFactory.getLog(TdUtils.class);
	
	private static Map<String, String> stationNameMap = new HashMap<String, String>();
	
	private static Map<String, String> cardTypeMap = new HashMap<String, String>();
	private static Map<String, String> ticketTypeMap = new HashMap<String, String>();
	private static Map<Integer, String> seatClassMap = new HashMap<Integer, String>();
	
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
		initHelper(cardType, ";", ",", cardTypeMap);
		
		String ticketType = AppConfig.getInstance().getPropInternal("td.passenger.info.ticket.type");
		initHelper(ticketType, ";", ",", ticketTypeMap);
		
		
		String seatClass = AppConfig.getInstance().getPropInternal("td.passenger.info.seat.class");
		String[] entries = seatClass.split(",");
		for(int i = 0; i < entries.length; i++)
		{
			seatClassMap.put(i, entries[i]);
		}
	}
	
	private static void initHelper(String content, String entryDelim, String valueDelim, Map<String, String> map)
	{
		String[] entries = content.split(entryDelim);
		for(String entry : entries)
		{
			String[] values = entry.split(valueDelim);
			
			if(values.length != 2)
			{
				log.error("Unexpected values. entry=" + entry);
				continue;
			}
			
			map.put(values[0], values[1]);
		}
	}
	
	public static String getStationCode(String name)
	{
		return stationNameMap.get(name);
	}
	
	public static String getSeatClassValue(int seatType)
	{
		if(!seatClassMap.containsKey(seatType))
		{
			log.error("The seatType [" + seatType + "] doesn't exist.");
			seatType = TrainTicketInfo.NONE_SEAT;
		}
		
		return seatClassMap.get(seatType);
	}
	
	public static String getCardTypeValue(String cardType)
	{
		return cardTypeMap.get(cardType);
	}
	
	public static String getTicketTypeValue(String ticketType)
	{
		return ticketTypeMap.get(ticketType);
	}
}
