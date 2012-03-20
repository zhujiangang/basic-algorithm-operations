package com.bao.lc.httpcommand.impl;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.httpcommand.CommandRetryStrategy;

public class DefaultCommandRetryStrategy implements CommandRetryStrategy
{
	private static Log log = LogFactory.getLog(DefaultCommandRetryStrategy.class);
	
	/** the number of times a method will be retried */
    private final int retryCount;
    
    private List<IDValuePair> retryRCList = null;
    
	public DefaultCommandRetryStrategy(int retryCount)
	{
		this.retryCount = retryCount;
		
		retryRCList = new ArrayList<IDValuePair>();
	}
	public DefaultCommandRetryStrategy()
	{
		this(5);
	}
	
	public void addRetryResultCode(IDValuePair rc)
	{
		retryRCList.add(rc);
	}
	
	@Override
	public boolean shouldRetry(Context context, int executionCount, IDValuePair rc)
	{
		boolean result = false;
		do
		{
			if(executionCount >= retryCount)
			{
				log.info("Retry times over " + retryCount + ", executionCount=" + executionCount);
				break;
			}
			
			if(!retryRCList.contains(rc))
			{
				log.info("No available retry rc.");
				break;
			}
			
			result = true;
		}
		while(false);
		
		if(log.isDebugEnabled())
		{
			String msg = String.format("Retry result: [%b]. execCount=%d, rc=%s.", result,
				executionCount, rc.toString());
			log.debug(msg);
		}
		
		return result;
	}

	@Override
	public boolean shouldRetry(Context context, int executionCount, Exception e)
	{
		return false;
	}
}
