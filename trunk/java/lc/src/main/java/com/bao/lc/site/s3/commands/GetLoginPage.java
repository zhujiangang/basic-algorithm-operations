package com.bao.lc.site.s3.commands;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.http.HttpResponse;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.RequestBuilder;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.common.URI2NameBuilder;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.params.TdPNames;

public class GetLoginPage extends BasicHttpCommand
{
	public GetLoginPage()
	{
		super();
	}

	@Override
	protected IDValuePair preExecute(Context context) throws Exception
	{
		// Remove the response if it exists
		HttpCommandParams.purgeResponseParams(context);

		// Request
		RequestBuilder rb = new RequestBuilder();
		String uriString = MapUtils.getString(context, TdPNames.PARAM_LOGIN_URL);
		String encoding = HttpCommandParams.getDefaultResponseCharset(context);
		rb.uriStr(uriString).encoding(encoding);
		
		context.put(HttpCommandPNames.TARGET_REQUEST, rb.create());
		// Referrer
		String referer = MapUtils.getString(context, TdPNames.PARAM_LOGIN_REFERER);
		context.put(HttpCommandPNames.TARGET_REFERER, referer);
		
		return ResultCode.RC_OK;
	}
	
	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{		
		return parse(context);
	}

	private IDValuePair parse(Context context) throws Exception
	{
		// 1. parse response
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		String charsetName = HttpCommandParams.getCharset(rsp, context);

		// Get input
		URI2NameBuilder ub = new URI2NameBuilder();
		ub.uri(HttpCommandParams.getTargetRequestURI(context));
		ub.addParamName("method").encoding(charsetName);
		
		String content = HttpClientUtils.saveToString(rsp.getEntity(), charsetName, ub);

		context.put(TdPNames._LOGIN_PAGE_CONTENT, content);
		context.put(TdPNames._LOGIN_PAGE_ENCODING, charsetName);

		// Do execute GetVerificationCode
		Command childCommand = new ParseLoginPage();
		childCommand.execute(context);
		
		return HttpCommandParams.getResultCode(context);
	}
}
