package com.bao.lc.site.s3.commands;

import java.util.HashMap;
import java.util.Map;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpUriRequest;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.RequestBuilder;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.common.URI2NameBuilder;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.util.MiscUtils;

public class GetTicketBookInitPage extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(GetTicketBookInitPage.class);

	public GetTicketBookInitPage()
	{
		super();
	}

	protected IDValuePair preExecute(Context context) throws Exception
	{
		// Remove the response if it exists
		HttpCommandParams.purgeResponseParams(context);

		// Request
		context.put(HttpCommandPNames.TARGET_REQUEST, buildTargetRequest(context));
		// Referrer
		String referer = MapUtils.getString(context, TdPNames.PARAM_TICKET_BOOK_INIT_REFERER);
		context.put(HttpCommandPNames.TARGET_REFERER, referer);

		return ResultCode.RC_OK;
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

		//getSelected@https://dynamic.12306.cn/otsweb/js/order/query_common_func.js?version=2.4
		//submitRequest@https://dynamic.12306.cn/otsweb/js/order/query_single_bindevent.js?version=2.4
		//orderForm
		// 2. Parse
		IDValuePair rc = parseContent(context, content, charset);
		
		HttpCommandParams.purgeRequestParams(context);

		return rc;
	}

	private HttpUriRequest buildTargetRequest(Context context)
	{
		RequestBuilder rb = new RequestBuilder();

		String uriString = MapUtils.getString(context, TdPNames.PARAM_TICKET_BOOK_INIT_URL);
		String encoding = HttpCommandParams.getDefaultResponseCharset(context);

		rb.uriStr(uriString).encoding(encoding);

		return rb.create();
	}
	
	private IDValuePair parseContent(Context context, String content, String charset) throws ParserException
	{
		Parser parser = MiscUtils.createParser(content, charset, log);

		NodeFilter filter = new AndFilter(new NodeClassFilter(FormTag.class),
			new HasAttributeFilter("id", "orderForm"));
		NodeList nodeList = parser.parse(filter);
		
		if(nodeList.size() <= 0)
		{
			log.fatal("Can't find the orderForm.");
			return ResultCode.RC_UNKOWN;
		}
		
		Map<String, String> orderParamMap = new HashMap<String, String>();
		
		FormTag orderForm = (FormTag)nodeList.elementAt(0);
		NodeList inputs = (NodeList)orderForm.getFormInputs();
		for(int i = 0, size = inputs.size(); i < size; i++)
		{
			InputTag input = (InputTag)inputs.elementAt(i);
			
			String name = input.getAttribute("name");
			String value = input.getAttribute("value");
			
			if(value == null)
			{
				value = "";
			}
			orderParamMap.put(name, value);
		}
		
		context.put(TdPNames._ORDER_FORM_PARAMETER_MAP, orderParamMap);
		
		if(log.isDebugEnabled())
		{
			log.debug("orderParamMap: " + MiscUtils.toString(orderParamMap));
		}

		return ResultCode.RC_OK;
	}
}
