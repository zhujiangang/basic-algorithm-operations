package com.bao.lc.site.s2.commands;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpUriRequest;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s2.ZyContants;
import com.bao.lc.util.AppUtils;

public class DoLogout extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(DoLogout.class);

	public DoLogout()
	{
		super();
	}
	
	@Override
	protected IDValuePair preExecute(Context context) throws Exception
	{
		//Check if the user login?
		if(!MapUtils.getBooleanValue(context, ZyContants.LOGIN_STATE_KEY, false))
		{
			return ResultCode.RC_USER_NOT_LOGIN;
		}
		
		//Logout request
		HttpUriRequest logoutRequest = (HttpUriRequest)context.get(ZyContants.LOGOUT_URI_REQUEST);
		if(logoutRequest == null)
		{
			throw new IllegalStateException("Can't find logout URI request");
		}
		
		//User page URI as referrer
		String userPageURI = MapUtils.getString(context, ZyContants.USER_PAGE_URI);
		
		context.put(HttpCommandPNames.TARGET_REQUEST, logoutRequest);
		context.put(HttpCommandPNames.TARGET_REFERER, userPageURI);
		
		return ResultCode.RC_OK;
	}
	
	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		//Set login state
		context.put(ZyContants.LOGIN_STATE_KEY, Boolean.FALSE);
		
		//Save result
		HttpClientUtils.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("logout.html"));

		//No next hop
		context.remove(HttpCommandPNames.TARGET_REQUEST);
		context.remove(HttpCommandPNames.TARGET_REFERER);
		
		String user = MapUtils.getString(context, ZyContants.PARAM_USER);
		log.info("User [" + user + "] logout successfully.");
		
		return ResultCode.RC_OK;
	}
}
