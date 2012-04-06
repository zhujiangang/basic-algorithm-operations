package com.bao.lc.site.s3.commands;

import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.httpcommand.CommandRetryStrategy;
import com.bao.lc.site.s3.params.TdPNames;

public class BookTicketRetryStrategy implements CommandRetryStrategy
{
	private static Log log = LogFactory.getLog(BookTicketRetryStrategy.class);

	@Override
	public boolean shouldRetry(Context context, int executionCount, IDValuePair rc)
	{
		boolean ret = false;

		if(rc == ResultCode.RC_TD_CONFIRM_PASSENGER_CANCEL_TOO_MUCH
			|| rc == ResultCode.RC_TD_CONFIRM_PASSENGER_REPEAT_BUY_TICKET
			|| rc == ResultCode.RC_TD_CONFIRM_PASSENGER_CARD_NUMBER_ERROR)
		{
			ret = false;
		}
		else if(rc == ResultCode.RC_TD_CONFIRM_PASSENGER_RAND_CODE_ERROR)
		{
			// retry with newly input verification code
			context.remove(TdPNames._CONFIRM_PASSENGER_VOCDE);
			
			ret = true;
		}
		else if(rc == ResultCode.RC_TD_CONFIRM_PASSENGER_OTHER_ERROR
			|| rc == ResultCode.RC_TD_CONFIRM_PASSENGER_UNKOWN_ERROR)
		{
			// start from QueryLeftTicket
			context.remove(TdPNames._CONFIRM_PASSENGER_CONTENT);
			context.remove(TdPNames._CONFIRM_PASSENGER_ENCODING);
			context.remove(TdPNames._CONFIRM_PASSENGER_VOCDE);

			ret = true;
		}
		// other cases, stop to retry
		else
		{
			ret = false;
		}

		log.debug("[BookTicketRetryStrategy]: retry=" + ret + ", rc=" + rc);
		return ret;
	}

	@Override
	public boolean shouldRetry(Context context, int executionCount, Exception e)
	{
		return false;
	}

}
