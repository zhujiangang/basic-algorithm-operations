package com.bao.lc.common;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.htmlparser.util.ParserException;
import org.htmlparser.util.ParserFeedback;


public class LoggerFeedback implements ParserFeedback
{
	private static Log log = LogFactory.getLog(LoggerFeedback.class);
	
	private Log targetLog = null;
	
	public LoggerFeedback()
	{
		this(null);
	}
	public LoggerFeedback(Log targetLog)
	{
		if(targetLog == null)
		{
			this.targetLog = log;
		}
		else
		{
			this.targetLog = targetLog;
		}
	}

	@Override
	public void info(String message)
	{
		targetLog.info(message);
	}

	@Override
	public void warning(String message)
	{
		targetLog.warn(message);
	}

	@Override
	public void error(String message, ParserException e)
	{
		targetLog.error(message, e);
	}
}
