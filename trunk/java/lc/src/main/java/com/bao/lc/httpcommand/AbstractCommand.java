package com.bao.lc.httpcommand;

import org.apache.commons.chain.Context;
import org.apache.commons.chain.Filter;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.httpcommand.params.HttpCommandPNames;

public abstract class AbstractCommand implements Filter
{
	@Override
	public boolean execute(Context context) throws Exception
	{
		// Default: complete the current command
		boolean result = PROCESSING_COMPLETE;

		IDValuePair rc = ResultCode.RC_OK;

		do
		{
			// 0. Pre Execute
			rc = preExecute(context);
			if(rc != ResultCode.RC_OK)
			{
				break;
			}

			// 1. Do Execute
			rc = doExecute(context);
			if(rc != ResultCode.RC_OK)
			{
				break;
			}

			// -1. Post Execute
			rc = postExecute(context);
			if(rc != ResultCode.RC_OK)
			{
				break;
			}

			// Done, everything is OK
			rc = ResultCode.RC_OK;
			result = CONTINUE_PROCESSING;
		}
		while(false);

		// Save the command result
		context.put(HttpCommandPNames.HTTP_COMMAND_RESULT_CODE, rc);

		return result;
	}

	@Override
	public boolean postprocess(Context context, Exception exception)
	{
		// save the exception command
		if(!context.containsKey(HttpCommandPNames.EXCEPTION_COMMAND))
		{
			context.put(HttpCommandPNames.EXCEPTION_COMMAND, this);
		}

		return false;
	}

	protected abstract IDValuePair preExecute(Context context) throws Exception;

	protected abstract IDValuePair doExecute(Context context) throws Exception;

	protected abstract IDValuePair postExecute(Context context) throws Exception;
}
