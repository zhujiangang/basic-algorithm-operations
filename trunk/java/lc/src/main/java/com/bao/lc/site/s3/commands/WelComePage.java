package com.bao.lc.site.s3.commands;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.http.HttpResponse;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.RequestBuilder;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.util.AppUtils;

public class WelComePage extends BasicHttpCommand
{
	protected IDValuePair preExecute(Context context) throws Exception
	{		
		// Remove the response if it exists
		HttpCommandParams.purgeResponseParams(context);

		// Request
		RequestBuilder rb = new RequestBuilder();
		String uriString = MapUtils.getString(context, TdPNames.PARAM_WELCOME_URL);
		String encoding = HttpCommandParams.getDefaultResponseCharset(context);
		rb.uriStr(uriString).encoding(encoding);
		
		context.put(HttpCommandPNames.TARGET_REQUEST, rb.create());
		// Referrer
		String referer = MapUtils.getString(context, TdPNames.PARAM_WELCOME_REFERER);
		context.put(HttpCommandPNames.TARGET_REFERER, referer);
		
		return ResultCode.RC_OK;
	}

	protected IDValuePair postExecute(Context context) throws Exception
	{
		// 1. parse response
		HttpResponse rsp = HttpCommandParams.getResponse(context);

		String fileName = AppUtils.getTempFilePath("welcome.html");
		HttpClientUtils.saveToFile(rsp.getEntity(), fileName);
		
		HttpCommandParams.purgeRequestParams(context);

		return ResultCode.RC_OK;
	}
}
