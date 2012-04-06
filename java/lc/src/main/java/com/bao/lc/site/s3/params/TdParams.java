package com.bao.lc.site.s3.params;

import java.util.List;
import java.util.Map;

import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;

import com.bao.lc.site.s3.bean.PassengerInfo;
import com.bao.lc.site.s3.bean.TrainTicketInfo;

public class TdParams
{
	public static Log getUI(Context context)
	{
		Log uiLog = (Log)context.get(TdPNames._USER_INTERFACE);
		if(uiLog == null)
		{
			throw new IllegalStateException("User interface log can't be null.");
		}
		return uiLog;
	}
	
	public static List<TrainTicketInfo> getTicketInfoList(Context context)
	{
		return (List<TrainTicketInfo>)context.get(TdPNames._TICKET_INFO_LIST);
	}
	
	public static TrainTicketInfo getOrderTicketInfo(Context context)
	{
		return (TrainTicketInfo)context.get(TdPNames._ORDER_TICKET_INFO);
	}
	
	public static Map<String, String> getOrderFormParamMap(Context context)
	{
		return (Map<String, String>)context.get(TdPNames._ORDER_FORM_PARAMETER_MAP);
	}
	
	public static List<PassengerInfo> getPassengerList(Context context)
	{
		return (List<PassengerInfo>)context.get(TdPNames.PARAM_PASSENGER_LIST);
	}
}
