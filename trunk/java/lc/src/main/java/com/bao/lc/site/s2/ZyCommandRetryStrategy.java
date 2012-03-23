package com.bao.lc.site.s2;

import java.util.Date;
import java.util.Calendar;

import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.httpcommand.impl.DefaultCommandRetryStrategy;
import com.bao.lc.util.MiscUtils;

public class ZyCommandRetryStrategy extends DefaultCommandRetryStrategy
{
	private static Log log = LogFactory.getLog(ZyCommandRetryStrategy.class);
	
	private static final long MAX_WAIT_TIME = 600000L; //10 minutes
	
	private Date startTime;
	private int fixedExecCount = 0;

	private long lastSleepTime = 0L;

	public ZyCommandRetryStrategy(int retryCount, Date startTime)
	{
		super(retryCount);

		this.startTime = startTime;
		this.fixedExecCount = 0;
		this.lastSleepTime = 0L;
		
		super.addRetryResultCode(ResultCode.RC_ZY_TIME_LIST_EMPTY);
		super.addRetryResultCode(ResultCode.RC_ZY_REG_TIME_NOT_REACH_YET);
		super.addRetryResultCode(ResultCode.RC_ZY_TARGET_DATE_NO_TICKET);
	}

	@Override
	public boolean shouldRetry(Context context, int execCount, IDValuePair rc)
	{
		log.debug("execCount=" + execCount + ", rc=" + rc);
		
		//Stop for these result code
		if(rc == ResultCode.RC_ZY_TARGET_DATE_STOP_TO_BOOK || rc == ResultCode.RC_DOCTOR_REG_LIST_FULL)
		{
			log.info("Stop to retry for rc: " + rc);
			return false;
		}
		
		//Sleep util the time before the startTime 2 seconds
		long diff = 0L;
		Calendar now = Calendar.getInstance();
		do
		{
			diff = this.startTime.getTime() - now.getTimeInMillis();
			if(diff <= 2000L)
			{
				break;
			}
			MiscUtils.sleep(diff - 2000L);
			
			now = Calendar.getInstance();
		}
		while(true);
		
		//decrease the frequency
		if(lastSleepTime > 0L)
		{
			MiscUtils.sleep(lastSleepTime);
		}
		
		//update the next sleep time
		now = Calendar.getInstance();
		diff = now.getTimeInMillis() - this.startTime.getTime();
		if(diff > 180000L)
		{
			lastSleepTime = 50L;
		}
		else
		{
			lastSleepTime = 5L;
		}
		
		//Only after the MAX_WAIT_TIME, increase the fixedExecCount
		if(diff > MAX_WAIT_TIME)
		{
			fixedExecCount++;
		}
		boolean ret = super.shouldRetry(context, fixedExecCount, rc);
		
		return ret;
	}
}
