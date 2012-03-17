package com.bao.lc.site.s2.commands;

import org.apache.commons.chain.Context;
import org.apache.http.HttpResponse;

import com.bao.lc.client.PostRedirectStrategy;
import com.bao.lc.common.IDValuePair;
import com.bao.lc.common.ResultCode;
import com.bao.lc.httpcommand.DefaultHttpCommand;
import com.bao.lc.httpcommand.HttpCommandPNames;
import com.bao.lc.httpcommand.HttpCommandParams;
import com.bao.lc.util.AppUtils;
import com.bao.lc.util.HttpClientUtil;

public class DoBookTicket extends DefaultHttpCommand
{
	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		PostRedirectStrategy redirectStrategy = HttpCommandParams.getRedirectStrategy(context);
		if(redirectStrategy == null)
		{
			throw new IllegalStateException("Can't find the redirectStrategy.");
		}
		if(!redirectStrategy.isRedirected())
		{
			throw new IllegalStateException("Book doesn't find any redirect.");
		}

		// save the result page to consume the content
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		HttpClientUtil.saveToFile(rsp.getEntity(),
			AppUtils.getTempFilePath("BookTicketResult.html"));

		context.remove(HttpCommandPNames.TARGET_REQUEST);
		context.remove(HttpCommandPNames.TARGET_REFERER);

		return ResultCode.RC_OK;
	}
}
