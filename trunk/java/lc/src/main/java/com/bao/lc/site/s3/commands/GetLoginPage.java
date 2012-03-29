package com.bao.lc.site.s3.commands;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.http.HttpResponse;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.params.TdPNames;

public class GetLoginPage extends BasicHttpCommand
{
	public GetLoginPage()
	{
		super();
	}

	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{		
		parse(context);
		
		return ResultCode.RC_OK;
	}

	private void parse(Context context) throws Exception
	{
		// 1. parse response
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		String charsetName = HttpCommandParams.getCharset(context);

		// Get input
		String content = HttpClientUtils.saveToString(rsp.getEntity(), charsetName);

		context.put(TdPNames.PARAM_INPUT_CONTENT, content);
		context.put(TdPNames.PARAM_INPUT_ENCODING, charsetName);

		// Do execute GetVerificationCode
		Command childCommand = new ParseLoginPage();
		childCommand.execute(context);
	}
}
