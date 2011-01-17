package com.e2u.thirdparty.log4j;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.log4j.RollingFileAppender;

/**
 * An enhanced RollingFileAppender to add time-stamp suffix for log file name.
 * 
 * At system startup, this appender always re-create a new log file.
 *
 */
public class SystemStartRollingFileAppender extends RollingFileAppender
{
	private static final String LOG_FILE_NAME_SUFFIX = SystemStartRollingFileAppender.class.getName() + ".FILE_NAME_SUFFIX";
	
	private String timeStampPattern = null;

	public String getTimeStampPattern()
	{
		return timeStampPattern;
	}

	/**
	 * Suggested pattern: _yyyy_MM_dd_HH_mm_ss_SSS
	 * @param timeStampPattern
	 */
	public void setTimeStampPattern(String timeStampPattern)
	{
		this.timeStampPattern = timeStampPattern;
	}

	public void activateOptions()
	{
		setFileName();
		super.activateOptions();
	}
	
	private void setFileName()
	{
		if(timeStampPattern == null)
		{
			return;
		}
		
		String dateStr;
		if(System.getProperty(LOG_FILE_NAME_SUFFIX) != null)
		{
			dateStr = System.getProperty(LOG_FILE_NAME_SUFFIX);
		}
		else
		{
			SimpleDateFormat sdf = new SimpleDateFormat(timeStampPattern);
			dateStr = sdf.format(new Date());
			
			System.setProperty(LOG_FILE_NAME_SUFFIX, dateStr);
		}
		
		String var = fileName;
		//The fileName has extension name
		if(hasExtName(var))
		{
			int index = var.lastIndexOf('.');
			var = var.substring(0, index) + dateStr + var.substring(index);
		}
		else
		{
			var = var + dateStr;
		}

		fileName = var;		
	}
	
	private boolean hasExtName(String name)
	{
		File file = new File(name);
		name = file.getName();
		return name.indexOf('.') != -1;
	}
}

