package com.bao.lc.site.s2.commands;

import java.util.Calendar;

import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;

import com.bao.lc.AppConfig;
import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.common.exception.ParseException;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s2.ZyConstants;
import com.bao.lc.util.MiscUtils;

public class GetTimeDiff extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(GetTimeDiff.class);
	
	protected IDValuePair preExecute(Context context) throws Exception
	{
		super.preExecute(context);
		
		String timeDiffURL = AppConfig.getInstance().getPropInternal("zy.systime.url");
		context.put(HttpCommandPNames.TARGET_REQUEST, new HttpGet(timeDiffURL));
		
		return ResultCode.RC_OK;
	}
	
	protected IDValuePair postExecute(Context context) throws Exception
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);

		//start to parse
		String content = HttpClientUtils.saveToString(rsp.getEntity(), "UTF-8");
		
		Calendar cal = findTime(content);
		if(cal != null)
		{
			Calendar now = Calendar.getInstance();
			//add 300ms for network transport
			cal.add(Calendar.MILLISECOND, 300);
			
			long diff = now.getTimeInMillis() - cal.getTimeInMillis();
			
			context.put(ZyConstants.PARAM_TIME_DIFF, Long.valueOf(diff));
			
			log.info("System time diff with site time: " + diff);
		}

		context.remove(HttpCommandPNames.TARGET_REQUEST);
		context.remove(HttpCommandPNames.TARGET_REFERER);

		return ResultCode.RC_OK;
	}
	
	private Calendar findTime(String content)
	{
		String regex = "var dd=new Date\\(\"(.+?)\"\\)";
		
		String time = null;
		try
		{
			time = MiscUtils.getRegexValueOnce(content, regex, 1);
		}
		catch(ParseException e)
		{
			log.warn("Failed to get system time.", e);
		}
		
		if(time == null)
		{
			return null;
		}
		Calendar cal = MiscUtils.toCalendar(time, "(\\d+?)/(\\d+?)/(\\d+?) (\\d+?):(\\d+?):(\\d+?)");
		return cal;
	}
}
