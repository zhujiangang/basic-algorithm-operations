package com.bao.lc.site.s2.commands;

import org.apache.commons.chain.Context;
import org.apache.http.HttpResponse;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.impl.PostRedirectStrategy;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.util.AppUtils;

public class DoBookTicket extends BasicHttpCommand
{
	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		PostRedirectStrategy redirectStrategy = HttpCommandParams.getRedirectStrategy(context);
		if(redirectStrategy == null)
		{
			throw new IllegalStateException("Can't find the redirectStrategy.");
		}
		if(!redirectStrategy.isPostRedirected())
		{
			throw new IllegalStateException("Book doesn't find any redirect.");
		}

		// save the result page to consume the content
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		HttpClientUtils.saveToFile(rsp.getEntity(),
			AppUtils.getTempFilePath("BookTicketResult.html"));

		context.remove(HttpCommandPNames.TARGET_REQUEST);
		context.remove(HttpCommandPNames.TARGET_REFERER);

		return ResultCode.RC_OK;
	}
}
