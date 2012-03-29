package com.bao.lc.site.s3.params;

import java.util.List;

import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;

import com.bao.lc.site.s3.bean.TrainTicketInfo;

public class TdParams
{
	public static Log getUI(Context context)
	{
		Log uiLog = (Log)context.get(TdPNames.PARAM_UI);
		if(uiLog == null)
		{
			throw new IllegalStateException("User interface log can't be null.");
		}
		return uiLog;
	}
	
	public static List<TrainTicketInfo> getTicketInfoList(Context context)
	{
		return (List<TrainTicketInfo>)context.get(TdPNames.TICKET_INFO_LIST);
	}
}
