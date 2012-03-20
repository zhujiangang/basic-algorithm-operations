package com.bao.lc.site;

import java.util.Date;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class SleepCommand implements Command
{
	private static Log log = LogFactory.getLog(SleepCommand.class);
	
	private Date startTime;

	public SleepCommand(Date startTime)
	{
		this.startTime = startTime;
	}

	@Override
	public boolean execute(Context context) throws Exception
	{
		long sleepTime = startTime.getTime() - System.currentTimeMillis();
		log.info("sleepTime: " + sleepTime);
		if(sleepTime > 0)
		{
			log.debug("Start to sleep " + sleepTime + " ms");
			try
			{
				Thread.sleep(sleepTime);
			}
			catch(InterruptedException e)
			{
				log.warn("sleep InterruptedException.", e);
			}
			log.debug("Finished sleeping " + sleepTime + " ms");
		}
		
		return CONTINUE_PROCESSING;
	}

}
