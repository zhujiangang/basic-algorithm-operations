package com.bao.lc.site.s3;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.chain.Context;
import org.apache.commons.io.IOUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;

import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.params.HttpCommandParams;

public class TdUtils
{
	private static Log log = LogFactory.getLog(TdUtils.class);
	
	private static boolean isInitialized = false;
	private static Map<String, String> stationNameMap = new HashMap<String, String>();
	
	private static boolean init()
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
	
	public static String getStationCode(String name)
	{
		if(!isInitialized)
		{
			isInitialized = init();
		}
		return stationNameMap.get(name);
	}
	
	public static String getCharset(HttpResponse rsp, Context context)
	{
		String charsetName = HttpClientUtils.getCharset(rsp);
		if(charsetName == null)
		{
			charsetName = HttpCommandParams.getDefaultResponseCharset(context);
		}
		return charsetName;
	}
}
