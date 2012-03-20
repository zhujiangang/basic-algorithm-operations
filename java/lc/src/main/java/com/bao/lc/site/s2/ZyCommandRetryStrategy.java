package com.bao.lc.site.s2;

import java.util.Date;
import java.util.Calendar;

import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.httpcommand.impl.DefaultCommandRetryStrategy;
import com.bao.lc.util.MiscUtils;

public class ZyCommandRetryStrategy extends DefaultCommandRetryStrategy
{
	private static Log log = LogFactory.getLog(ZyCommandRetryStrategy.class);
	
	private Date startTime;
	private int invalidRetryTimes = 0;
	
	private Date lastRetryTime = null;

	public ZyCommandRetryStrategy(int retryCount, Date startTime)
	{
		super(retryCount);

		this.startTime = startTime;
		this.invalidRetryTimes = 0;
		
		this.lastRetryTime = null;
	}

	@Override
	public boolean shouldRetry(Context context, int execCount, IDValuePair rc)
	{
		//1. sleep
		Calendar now = Calendar.getInstance();
		for( ; now.getTime().before(this.startTime); now = Calendar.getInstance())
		{
			this.invalidRetryTimes = execCount;
			long diff = this.startTime.getTime() - now.getTimeInMillis();
			if(diff <= 50)
			{
				break;
			}
			MiscUtils.sleep(diff - 50);
		}
		log.debug("invalidRetryTimes=" + invalidRetryTimes + ", execCount=" + execCount);
		
		if(lastRetryTime != null)
		{
			long diff = now.getTimeInMillis() - this.lastRetryTime.getTime();
			if(diff < 30)
			{
				MiscUtils.sleep(Math.abs(30 - diff));
			}
		}
		
		int fixedExecCount = 0;
		
		long diff = now.getTimeInMillis() - this.startTime.getTime();
		if(diff > 5000)
		{
			fixedExecCount = execCount - this.invalidRetryTimes;
		}
		boolean ret = super.shouldRetry(context, fixedExecCount, rc);
		
		if(ret)
		{
			lastRetryTime = now.getTime();
		}
		
		return ret;
	}
}
