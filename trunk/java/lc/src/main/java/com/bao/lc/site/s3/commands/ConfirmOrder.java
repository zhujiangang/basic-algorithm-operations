package com.bao.lc.site.s3.commands;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

import org.apache.commons.chain.Context;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.math.NumberUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.filters.OrFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.ScriptTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.lc.AppConfig;
import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.common.ScriptCodeFilter;
import com.bao.lc.common.URI2NameBuilder;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.site.s3.params.TdParams;
import com.bao.lc.util.MiscUtils;

public class ConfirmOrder extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(ConfirmOrder.class);

	private static final String JS_SUCCESS_REGEX = "var requiredNum=(\\d+?);(\\s+?)var successTicketNum=(\\d+?);";
	private static final String JS_ERROR_MSG_REGEX = "var\\s+message\\s+=\\s+\"(.+?)\"";
	
	private boolean isConfirmPassengerFormExist = false;

	protected IDValuePair preExecute(Context context) throws Exception
	{
		isConfirmPassengerFormExist = false;
		
		return super.preExecute(context);
	}
	protected IDValuePair postExecute(Context context) throws Exception
	{
		// 1. Save the response content
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		String charset = HttpCommandParams.getCharset(rsp, context);

		URI2NameBuilder ub = new URI2NameBuilder();
		ub.uri(HttpCommandParams.getTargetRequestURI(context));
		ub.addParamName("method").encoding(charset);

		String content = HttpClientUtils.saveToString(rsp.getEntity(), charset, ub);
		
		context.put(TdPNames._CONFIRM_PASSENGER_CONTENT, content);
		context.put(TdPNames._CONFIRM_PASSENGER_ENCODING, charset);

		IDValuePair rc = parseResult(context, content, charset);

		return rc;
	}

	private IDValuePair parseResult(Context context, String content, String charset)
		throws ParserException
	{
		if(isSuccess(context, content, charset))
		{
			TdParams.getUI(context).info("Book tickets result: Success");
			return ResultCode.RC_OK;
		}
		
		
		IDValuePair rc = parseFailReason(context, content, charset);
		
		TdParams.getUI(context).info("Book tickets result: " + rc);
		
		return rc;
	}

	private boolean isSuccess(Context context, String pageContent, String charset)
		throws ParserException
	{
		boolean ret = false;

		do
		{
			if(!isSuccessByJS(context, pageContent, charset))
			{
				log.info("isSuccessByJS failed");
				break;
			}
			if(!isSuccessByForm(context, pageContent, charset))
			{
				log.info("isSuccessByForm failed");
				break;
			}
			ret = true;
		}
		while(false);

		return ret;
	}
	private boolean isSuccessByJS(Context context, String pageContent, String charset)
		throws ParserException
	{
		Parser parser = MiscUtils.createParser(pageContent, charset, log);
		int flags = Pattern.MULTILINE | Pattern.DOTALL;

		NodeFilter scriptFilter = new ScriptCodeFilter(JS_SUCCESS_REGEX, flags);

		// parse
		NodeList nodeList = parser.parse(scriptFilter);
		if(nodeList.size() < 1)
		{
			log.info("Can't find the 'successTicketNum' script. Assume confirm order failed.");
			return false;
		}
		ScriptTag js = (ScriptTag) nodeList.elementAt(0);
		List<String> valueList = new ArrayList<String>();
		int matchCount = MiscUtils.getRegexValue(js.getScriptCode().trim(), JS_SUCCESS_REGEX,
			valueList, true, flags);
		if(matchCount != 1)
		{
			log.fatal("isSuccessByJS Unexpected: matchCount=" + matchCount);
			return false;
		}

		String requiredNum = valueList.get(1).trim();
		String successTicketNum = valueList.get(3).trim();
		log.info("Javascript code: requiredNum=" + requiredNum + ", successTicketNum=" + successTicketNum);
		
		int iTotalNum = NumberUtils.toInt(requiredNum);
		int iSusscessNum = NumberUtils.toInt(successTicketNum);
		
		if(iTotalNum != iSusscessNum)
		{
			log.error("iTotalNum=" + iTotalNum + " != iSusscessNum=" + iSusscessNum);
			return false;
		}

		return true;
	}
	
	private boolean isSuccessByForm(Context context, String pageContent, String charset)
		throws ParserException
	{
		Parser parser = MiscUtils.createParser(pageContent, charset, log);
		
		NodeFilter[] a = new NodeFilter[0];

		List<NodeFilter> filters = new ArrayList<NodeFilter>();
		
		//FAILED form
		filters.add(new NodeClassFilter(FormTag.class));
		filters.add(new HasAttributeFilter("id", "confirmPassenger"));
		filters.add(new HasAttributeFilter("name", "save_passenger_single"));
		NodeFilter confirmPassengerFilter = new AndFilter(filters.toArray(a));
		filters.clear();
		
		//SUCCESS form
		filters.add(new NodeClassFilter(FormTag.class));
		filters.add(new HasAttributeFilter("id", "epayForm"));
		NodeFilter epayFormFilter = new AndFilter(filters.toArray(a));
		filters.clear();
		
		//Final filter
		NodeFilter filter = new OrFilter(confirmPassengerFilter, epayFormFilter);
		
		NodeList nodeList = parser.parse(filter);

		FormTag failForm = null, okForm = null;
		for(int i = 0, size = nodeList.size(); i < size; i++)
		{
			FormTag form = (FormTag)nodeList.elementAt(i);
			
			String id = form.getAttribute("id");
			
			log.info("Found the form '" + id + "'");
			if("confirmPassenger".equals(id))
			{
				failForm = form;
				isConfirmPassengerFormExist = true;
			}
			else if("epayForm".equals(id))
			{
				okForm = form;
			}
		}
		if(failForm != null)
		{
			return false;
		}
		if(okForm != null)
		{
			return true;
		}
		log.warn("Can't find the epayForm. Assume success!");
		
		return true;
	}
	
	private IDValuePair parseFailReason(Context context, String pageContent, String charset)
		throws ParserException
	{
		Parser parser = MiscUtils.createParser(pageContent, charset, log);
		int flags = Pattern.MULTILINE | Pattern.DOTALL;

		NodeFilter scriptFilter = new ScriptCodeFilter(JS_ERROR_MSG_REGEX, flags);

		// parse
		NodeList nodeList = parser.parse(scriptFilter);
		if(nodeList.size() < 1)
		{
			log.info("Can't find the 'message' script.");
			return ResultCode.RC_TD_CONFIRM_PASSENGER_UNKOWN_ERROR;
		}
		ScriptTag js = (ScriptTag) nodeList.elementAt(0);
		List<String> valueList = new ArrayList<String>();
		int matchCount = MiscUtils.getRegexValue(js.getScriptCode().trim(), JS_ERROR_MSG_REGEX,
			valueList, true, flags);
		if(matchCount != 1)
		{
			log.fatal("parseFailReason Unexpected: matchCount=" + matchCount);
			return ResultCode.RC_ASSERT_FAILURE;
		}

		String message = valueList.get(1).trim();
		log.info("Javascript code: error message=" + message);
		
		IDValuePair rc = ResultCode.RC_TD_CONFIRM_PASSENGER_UNKOWN_ERROR;
		if(!StringUtils.isEmpty(message))
		{
			if(message.contains(AppConfig.getInstance().getPropInternal("td.order.nok.rand_code_error")))
			{
				rc = ResultCode.RC_TD_CONFIRM_PASSENGER_RAND_CODE_ERROR;
				if(!isConfirmPassengerFormExist)
				{
					log.info("Rand Code error, but the Confirm Passenger Form doesn't exist");
					context.remove(TdPNames._CONFIRM_PASSENGER_CONTENT);
					context.remove(TdPNames._CONFIRM_PASSENGER_ENCODING);
				}
			}
			else if(message.contains(AppConfig.getInstance().getPropInternal("td.order.nok.cancel_too_much")))
			{
				rc = ResultCode.RC_TD_CONFIRM_PASSENGER_CANCEL_TOO_MUCH;
			}
			else if(message.contains(AppConfig.getInstance().getPropInternal("td.order.nok.repeat_buy_ticket")))
			{
				rc = ResultCode.RC_TD_CONFIRM_PASSENGER_REPEAT_BUY_TICKET;
			}
			else if(message.contains(AppConfig.getInstance().getPropInternal("td.order.nok.card_number_error")))
			{
				rc = ResultCode.RC_TD_CONFIRM_PASSENGER_CARD_NUMBER_ERROR;
			}
			else
			{
				rc = ResultCode.RC_TD_CONFIRM_PASSENGER_OTHER_ERROR;
			}
		}

		return rc;
	}
}
