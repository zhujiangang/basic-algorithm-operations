package com.bao.lc.site.s3.gui;

import javax.swing.JTextArea;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.Appender;
import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.apache.log4j.Priority;
import org.apache.log4j.spi.LoggingEvent;

public class TextAreaLog implements Log
{
	private static Log log = LogFactory.getLog("TdClient.UI");

	private static final String FQCN = TextAreaLog.class.getName();

	private Appender appender = null;

	private transient Logger logger = null;

	public TextAreaLog(JTextArea textArea)
	{
		this.appender = new TextAreaAppender(textArea);
		this.logger = Logger.getLogger(FQCN);
	}

	@Override
	public boolean isDebugEnabled()
	{
		return log.isDebugEnabled();
	}

	@Override
	public boolean isErrorEnabled()
	{
		return log.isErrorEnabled();
	}

	@Override
	public boolean isFatalEnabled()
	{
		return log.isFatalEnabled();
	}

	@Override
	public boolean isInfoEnabled()
	{
		return log.isInfoEnabled();
	}

	@Override
	public boolean isTraceEnabled()
	{
		return log.isTraceEnabled();
	}

	@Override
	public boolean isWarnEnabled()
	{
		return log.isWarnEnabled();
	}

	@Override
	public void trace(Object message)
	{
		if(isTraceEnabled())
		{
			log(FQCN, Level.TRACE, message, null);
			log.trace(message);
		}
	}

	@Override
	public void trace(Object message, Throwable t)
	{
		if(isTraceEnabled())
		{
			log(FQCN, Level.TRACE, message, t);
			log.trace(message, t);
		}
	}

	@Override
	public void debug(Object message)
	{
		if(isDebugEnabled())
		{
			log(FQCN, Level.DEBUG, message, null);
			log.debug(message);
		}
	}

	@Override
	public void debug(Object message, Throwable t)
	{
		if(isDebugEnabled())
		{
			log(FQCN, Level.DEBUG, message, t);
			log.debug(message, t);
		}
	}

	@Override
	public void info(Object message)
	{
		if(isInfoEnabled())
		{
			log(FQCN, Level.INFO, message, null);
			log.info(message);
		}
	}

	@Override
	public void info(Object message, Throwable t)
	{
		if(isInfoEnabled())
		{
			log(FQCN, Level.INFO, message, t);
			log.info(message, t);
		}
	}

	@Override
	public void warn(Object message)
	{
		if(isWarnEnabled())
		{
			log(FQCN, Level.WARN, message, null);
			log.warn(message);
		}
	}

	@Override
	public void warn(Object message, Throwable t)
	{
		if(isWarnEnabled())
		{
			log(FQCN, Level.WARN, message, t);
			log.warn(message, t);
		}
	}

	@Override
	public void error(Object message)
	{
		if(isErrorEnabled())
		{
			log(FQCN, Level.ERROR, message, null);
			log.error(message);
		}
	}

	@Override
	public void error(Object message, Throwable t)
	{
		if(isErrorEnabled())
		{
			log(FQCN, Level.ERROR, message, t);
			log.error(message, t);
		}
	}

	@Override
	public void fatal(Object message)
	{
		if(isFatalEnabled())
		{
			log(FQCN, Level.FATAL, message, null);
			log.fatal(message);
		}
	}

	@Override
	public void fatal(Object message, Throwable t)
	{
		if(isFatalEnabled())
		{
			log(FQCN, Level.FATAL, message, t);
			log.fatal(message, t);
		}
	}

	public void log(String callerFQCN, Priority level, Object message, Throwable t)
	{
		LoggingEvent event = new LoggingEvent(callerFQCN, logger, level, message, t);
		appender.doAppend(event);
	}
}
