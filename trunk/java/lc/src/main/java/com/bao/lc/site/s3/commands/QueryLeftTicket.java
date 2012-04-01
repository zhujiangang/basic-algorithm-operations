package com.bao.lc.site.s3.commands;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpUriRequest;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.RequestBuilder;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.common.URI2NameBuilder;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.TdUtils;
import com.bao.lc.site.s3.ai.TrainTicketSelector;
import com.bao.lc.site.s3.bean.TrainTicketInfo;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.site.s3.params.TdParams;
import com.bao.lc.util.MiscUtils;

public class QueryLeftTicket extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(QueryLeftTicket.class);
	
	public QueryLeftTicket()
	{
		super();
	}
	
	protected IDValuePair preExecute(Context context) throws Exception
	{
		// Remove the response if it exists
		HttpCommandParams.purgeResponseParams(context);
		
		//Request
		context.put(HttpCommandPNames.TARGET_REQUEST, buildTargetRequest(context));
		//Referrer
		String referer = MapUtils.getString(context, TdPNames.PARAM_QUERY_LEFT_TICKET_REFERER);
		context.put(HttpCommandPNames.TARGET_REFERER, referer);

		return ResultCode.RC_OK;
	}
	
	protected IDValuePair postExecute(Context context) throws Exception
	{
		//1. Save the response content
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		String charset = HttpCommandParams.getCharset(rsp, context);
		
		URI2NameBuilder ub = new URI2NameBuilder();
		ub.uri(HttpCommandParams.getTargetRequestURI(context));
		ub.addParamName("method").encoding(charset);
		
		String content = HttpClientUtils.saveToString(rsp.getEntity(), charset, ub);
		
		//2. Parse
		IDValuePair rc = parseResponseContent(context, content);
		
		return rc;
	}
	
	private HttpUriRequest buildTargetRequest(Context context)
	{
		RequestBuilder rb = new RequestBuilder();
		
		String uriString = MapUtils.getString(context, TdPNames.PARAM_QUERY_LEFT_TICKET_BASE_URL);
		String encoding = HttpCommandParams.getDefaultResponseCharset(context);
		
		rb.method(HttpGet.METHOD_NAME).uriStr(uriString).encoding(encoding);
		rb.paramMap(getParamMap(context));
		
		
		return rb.create();
	}
	
	private Map<String, String> getParamMap(Context context)
	{
		//Refer from: function loadData() @ https://dynamic.12306.cn/otsweb/js/order/query_single_bindevent.js?version=2.4
		
		/**
		 * $.ajax( {
		url : ctx+'/order/querySingleAction.do?method=queryLeftTicket',
		type : "GET",
		dataType:'text',
		data:{
			'orderRequest.train_date' : $('#startdatepicker').val(),
			'orderRequest.from_station_telecode' : $('#fromStation').val(),
			'orderRequest.to_station_telecode' : $('#toStation').val(),
			'orderRequest.train_no' : $('#trainCode').val(),
			'trainPassType' : getTrainPassType(),
			'trainClass' : getTrainClassString(),
			'includeStudent' : getIncludeStudent(),
			'seatTypeAndNum' : getSeanTypeAndNum(),
			'orderRequest.start_time_str' : $('#startTime').val()
		},
		 */
		String fromStation = MapUtils.getString(context, TdPNames.PARAM_FROM_STATION);
		String toStation = MapUtils.getString(context, TdPNames.PARAM_TO_STATION);
		String ticketDate = MapUtils.getString(context, TdPNames.PARAM_TICKET_DATE);
		String ticketTimeRange = MapUtils.getString(context, TdPNames.PARAM_TICKET_TIME_RANGE);
		
		Map<String, String> paramMap = new HashMap<String, String>();
		
		paramMap.put("orderRequest.train_date", ticketDate);
		paramMap.put("orderRequest.from_station_telecode", TdUtils.getStationCode(fromStation));
		paramMap.put("orderRequest.to_station_telecode", TdUtils.getStationCode(toStation));
		paramMap.put("orderRequest.train_no", "");
		paramMap.put("trainPassType", "QB");
		paramMap.put("trainClass", "QB#D#Z#T#K#QT#");
		paramMap.put("includeStudent", "00");
		paramMap.put("seatTypeAndNum", "");
		paramMap.put("orderRequest.start_time_str", ticketTimeRange);
		
		if(log.isDebugEnabled())
		{
			log.debug("paramMap=" + MiscUtils.toString(paramMap));
		}
		
		return paramMap;
	}
	
	private IDValuePair parseResponseContent(Context context, String content)
	{
		if(log.isTraceEnabled())
		{
			log.trace("raw data: " + content);
		}
		String str = content.replaceAll("&nbsp;", "");
		
		String[] rows = str.split("\\\\n");
		
		List<TrainTicketInfo> ticketInfoList = new ArrayList<TrainTicketInfo>(rows.length);
		for(String row : rows)
		{
			String[] fields = row.split(",");
			if(fields.length < TrainTicketInfo.RAW_FIELD_COUNT)
			{
				log.error("The ticket info is incomplete. row=" + row + ", raw data: " + content);
				return ResultCode.RC_TD_QUERY_LEFT_TICKET_ERROR;
			}
			
			TrainTicketInfo ticketInfo = new TrainTicketInfo(fields, 0);
			ticketInfoList.add(ticketInfo);
		}
		context.put(TdPNames._TICKET_INFO_LIST, ticketInfoList);
		
		if(log.isDebugEnabled())
		{
			log.debug("ticketInfoList=" + ticketInfoList);
		}
		
		TrainTicketSelector selector = new TrainTicketSelector(ticketInfoList, context);
		TrainTicketInfo ticket = selector.select();
		if(ticket == null)
		{
			TdParams.getUI(context).info("No avalable ticket with current filter!");
			return ResultCode.RC_TD_NO_AVAILABLE_TICKET_WITH_FILTER;
		}
		
		context.put(TdPNames._ORDER_TICKET_INFO, ticket);
		TdParams.getUI(context).info("Found the best matched ticket: " + ticket);
		
		
		return ResultCode.RC_OK;
	}
}
