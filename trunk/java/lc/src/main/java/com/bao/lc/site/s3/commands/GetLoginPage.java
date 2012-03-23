package com.bao.lc.site.s3.commands;

import java.io.IOException;

import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.ParseException;
import org.htmlparser.util.ParserException;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.HttpCommandDirector;
import com.bao.lc.httpcommand.impl.DefaultHttpCommandDirector;
import com.bao.lc.httpcommand.impl.LogCompleteListener;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.TdUtils;
import com.bao.lc.site.s3.params.TdPNames;

public class GetLoginPage extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(GetLoginPage.class);

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

	private void parse(Context context) throws IOException, ParseException, ParserException
	{
		// 1. parse response
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		String charsetName = TdUtils.getCharset(rsp, context);

		// Get input
		String content = HttpClientUtils.saveToString(rsp.getEntity(), charsetName);

		context.put(TdPNames.PARAM_INPUT_CONTENT, content);
		context.put(TdPNames.PARAM_INPUT_ENCODING, charsetName);
		context.put(TdPNames.PARAM_IS_LOGIN_FIRST, Boolean.TRUE);

		// Do execute GetVerificationCode
		HttpCommandDirector director = new DefaultHttpCommandDirector();
		director.execute(new ParseLoginPage(), context, new LogCompleteListener(log));
	}
}
