package com.bao.lc.site.s3.commands;

import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.httpcommand.CommandRetryStrategy;
import com.bao.lc.site.s3.params.TdPNames;

public class LoginRetryStrategy implements CommandRetryStrategy
{
	private static Log log = LogFactory.getLog(LoginRetryStrategy.class);
	
	@Override
	public boolean shouldRetry(Context context, int executionCount, IDValuePair rc)
	{
		boolean ret = false;
		
		if(rc == ResultCode.RC_TD_LOGIN_ACCOUNT_ERROR || rc == ResultCode.RC_TD_LOGIN_PASSWORD_ERROR)
		{
			ret = false;
		}
		else if(rc == ResultCode.RC_TD_LOGIN_VCODE_ERROR)
		{
			//retry with verification code
			context.remove(TdPNames.PARAM_LOGIN_VCODE);
			
			ret = true;
		}
		else if(rc == ResultCode.RC_TD_LOGIN_NO_SUBMIT_FORM_ERROR)
		{
			//start from GetLoginPage
			context.remove(TdPNames.PARAM_INPUT_CONTENT);
			context.remove(TdPNames.PARAM_INPUT_ENCODING);
			
			ret = true;
		}
		else if(rc == ResultCode.RC_TD_LOGIN_OTHER_MSG_ERROR)
		{
			//retry without user's input
			ret = true;
		}
		//retry with all the other results
		else
		{
			ret = true;
		}

		if(ret)
		{
			context.put(TdPNames.PARAM_IS_LOGIN_FIRST, Boolean.FALSE);
		}
		
		log.debug("[LoginRetryStrategy]: retry=" + ret + ", rc=" + rc);
		return ret;
	}

	@Override
	public boolean shouldRetry(Context context, int executionCount, Exception e)
	{
		return false;
	}

}
