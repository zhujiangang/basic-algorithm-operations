package com.bao.lc.site.s3.commands;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpUriRequest;

import com.bao.lc.AppConfig;
import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.RequestBuilder;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.util.AppUtils;

public class DoLogout extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(DoLogout.class);
	
	@Override
	protected IDValuePair preExecute(Context context) throws Exception
	{
		super.preExecute(context);

		// Check if the user login?
		if(!MapUtils.getBooleanValue(context, TdPNames.LOGIN_STATE, false))
		{
			return ResultCode.RC_USER_NOT_LOGIN;
		}

		String logoutURI = AppConfig.getInstance().getPropInternal("td.logout.url");
		String logoutReferer = AppConfig.getInstance().getPropInternal("td.logout.referer");

		RequestBuilder rb = new RequestBuilder();
		rb.uriStr(logoutURI);
		HttpUriRequest request = rb.create();

		context.put(HttpCommandPNames.TARGET_REQUEST, request);
		context.put(HttpCommandPNames.TARGET_REFERER, logoutReferer);

		return ResultCode.RC_OK;
	}

	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		//Set login state
		context.put(TdPNames.LOGIN_STATE, Boolean.FALSE);
		
		//Save result
		HttpClientUtils.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("logout.html"));

		//No next hop
		context.remove(HttpCommandPNames.TARGET_REQUEST);
		context.remove(HttpCommandPNames.TARGET_REFERER);
		
		String user = MapUtils.getString(context, TdPNames.PARAM_USER);
		log.info("User [" + user + "] logout successfully.");
		
		return ResultCode.RC_OK;
	}
}
