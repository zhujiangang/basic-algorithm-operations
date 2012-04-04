package com.bao.lc.site.s3.commands;

import java.net.URI;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpUriRequest;

import com.bao.lc.bean.BasicIDValuePair;
import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.RequestBuilder;
import com.bao.lc.client.impl.PostRedirectStrategy;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.common.URI2NameBuilder;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.bean.TrainTicketInfo;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.site.s3.params.TdParams;
import com.bao.lc.util.MapOperation;
import com.bao.lc.util.MapRelation;
import com.bao.lc.util.MapXorResult;
import com.bao.lc.util.MiscUtils;

public class SubmitOrder extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(SubmitOrder.class);

	public SubmitOrder()
	{
		super();
	}

	protected IDValuePair preExecute(Context context) throws Exception
	{
		// Remove the response if it exists
		HttpCommandParams.purgeResponseParams(context);

		// Request
		IDValuePair rc = buildTargetRequest(context);
		if(rc != ResultCode.RC_OK)
		{
			return rc;
		}
		// Referrer
		String referer = MapUtils.getString(context, TdPNames.PARAM_SUBMIT_ORDER_REFERER);
		context.put(HttpCommandPNames.TARGET_REFERER, referer);

		return ResultCode.RC_OK;
	}

	protected IDValuePair postExecute(Context context) throws Exception
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		String charset = HttpCommandParams.getCharset(rsp, context);

		// Login page content
		URI2NameBuilder ub = new URI2NameBuilder();
		ub.uri(HttpCommandParams.getTargetRequestURI(context));
		ub.addParamName("method").encoding(charset);
		
		String submitOrderPage = HttpClientUtils.saveToString(rsp.getEntity(), charset, ub);
		
		//error url: http://www.12306.cn/mormhweb/logFiles/error.html
		PostRedirectStrategy redirectStrategy = HttpCommandParams.getRedirectStrategy(context);
		HttpUriRequest finalReq = redirectStrategy.getFinalRequest();
		if(finalReq == null || finalReq.getURI() == null)
		{
			log.error("Can't find redirect req or uri when submit order.");
			return new BasicIDValuePair(ResultCode.RC_CANT_FIND_REDIRECTS, "submit order");
		}
		URI redirectURI = finalReq.getURI();
		String expectedURI = MapUtils.getString(context, TdPNames.PARAM_CONFIRM_PASSENGER_URL, "");
		if(!StringUtils.equals(expectedURI, redirectURI.toString()))
		{
			String content = MiscUtils.toPlainText(submitOrderPage, charset);
			TdParams.getUI(context).info(content);
			
			String info = String.format(
				"SubmitOrder error: Exptected redirect URI=%s, actual=%s, content=%s", expectedURI,
				redirectURI.toString(), content);
			log.error(info);

			return new BasicIDValuePair(ResultCode.RC_UNEXPECTED_REDIRECTS, info);
		}
		//TODO
		HttpCommandParams.purgeRequestParams(context);
		
		return ResultCode.RC_OK;
	}

	private IDValuePair buildTargetRequest(Context context)
	{
		String url = MapUtils.getString(context, TdPNames.PARAM_SUBMIT_ORDER_URL);
		Map<String, String> paramMap = new HashMap<String, String>();
		
		IDValuePair rc = getParamMap(context, paramMap);
		if(rc != ResultCode.RC_OK)
		{
			return rc;
		}
		
		String encoding = HttpCommandParams.getDefaultResponseCharset(context);
		
		RequestBuilder rb = new RequestBuilder();
		rb.method(HttpPost.METHOD_NAME).uriStr(url);
		rb.paramMap(paramMap).encoding(encoding);
		
		context.put(HttpCommandPNames.TARGET_REQUEST, rb.create());
		
		return ResultCode.RC_OK;
	}

	private IDValuePair getParamMap(Context context, Map<String, String> paramMap)
	{
		// Refer from:
		// submitRequest@https://dynamic.12306.cn/otsweb/js/order/query_single_bindevent.js?version=2.4

		/**
		var selectStr_arr = selectStr.split("#");
		var station_train_code=selectStr_arr[0];
		var lishi=selectStr_arr[1];
		var starttime=selectStr_arr[2];
		var trainno=selectStr_arr[3];
		var from_station_telecode=selectStr_arr[4];
		var to_station_telecode=selectStr_arr[5];
		var arrive_time=selectStr_arr[6];
		var from_station_name=selectStr_arr[7];
		var to_station_name=selectStr_arr[8];
		var ypInfoDetail=selectStr_arr[9];
		
		$('#station_train_code').val(station_train_code);
		$('#lishi').val(lishi);
		$('#train_start_time').val(starttime);
		$('#trainno').val(trainno);
		$('#from_station_telecode').val(from_station_telecode);
		$('#to_station_telecode').val(to_station_telecode);
		$('#arrive_time').val(arrive_time);
		$('#from_station_name').val(from_station_name);
		$('#to_station_name').val(to_station_name);
		$('#ypInfoDetail').val(ypInfoDetail);
		$('#orderForm').attr("action",
				ctx+"/order/querySingleAction.do?method=submutOrderRequest");
		$('#orderForm').submit();
		 */
		String[] trainParams = { "station_train_code", "lishi", "train_start_time", "trainno",
			"from_station_telecode", "to_station_telecode", "arrive_time", "from_station_name",
			"to_station_name", "ypInfoDetail" };
		
		TrainTicketInfo ticketInfo = TdParams.getOrderTicketInfo(context);
		String bookButtonParamStr = ticketInfo.getBookButtonParameter();
		String[] bookButtonParams = bookButtonParamStr.split("#");
		
		if(bookButtonParams.length != trainParams.length)
		{
			log.error("Book Button Parameter length is unexpected. bookButtonParams.length="
				+ bookButtonParams.length);
			return ResultCode.RC_TD_SUBMIT_ORDER_BOOK_PARAMETER_UNEXPECTED;
		}
		
		for(int i = 0; i < trainParams.length; i++)
		{
			paramMap.put(trainParams[i], bookButtonParams[i]);
		}
		
		//Other parameters
		/**
		  	from_station_telecode_name	上海
		  	to_station_telecode_name	北京
		  	train_date	2012-04-09	
		  	start_time_str	00:00--24:00	
		  	
		  	round_start_time_str	00:00--24:00
		  	round_train_date	2012-03-30 //Today
		  	
		  	single_round_type	1	
		  	train_class_arr	QB#D#Z#T#K#QT#	
			train_pass_type	QB	
			
			include_student	00
			seattype_num		
		 */
		String fromStation = MapUtils.getString(context, TdPNames.PARAM_FROM_STATION);
		String toStation = MapUtils.getString(context, TdPNames.PARAM_TO_STATION);
		String ticketDate = MapUtils.getString(context, TdPNames.PARAM_TICKET_DATE);
		String ticketTimeRange = MapUtils.getString(context, TdPNames.PARAM_TICKET_TIME_RANGE);
		
		paramMap.put("from_station_telecode_name", fromStation);
		paramMap.put("to_station_telecode_name", toStation);
		paramMap.put("train_date", ticketDate);
		paramMap.put("start_time_str", ticketTimeRange);
		paramMap.put("round_start_time_str", ticketTimeRange);
		paramMap.put("round_train_date", MiscUtils.getDatePart(Calendar.getInstance()));
		paramMap.put("single_round_type", "1");
		paramMap.put("train_class_arr", "QB#D#Z#T#K#QT#");
		paramMap.put("train_pass_type", "QB");
		paramMap.put("include_student", "00");
		paramMap.put("seattype_num", "");
		
		//Try to do a check
		Map<String, String> submitFormParamMap = TdParams.getOrderFormParamMap(context);
		
		MapOperation<String, String> mapOper = new MapOperation<String, String>(paramMap, submitFormParamMap);
		MapXorResult<String, String> xorResult = mapOper.xor(MapOperation.XOR_KEY_ONLY);
		if(xorResult.getRelation() != MapRelation.EQUALS)
		{
			log.warn("SubmitOrder parameter is unexpected: " + xorResult.toString());
		}

		return ResultCode.RC_OK;
	}
}
