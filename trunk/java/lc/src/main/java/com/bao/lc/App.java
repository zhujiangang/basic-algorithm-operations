package com.bao.lc;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
/**
 * Hello world!
 * 
 */
public class App
{
//	static
//	{		
//		PropertyConfigurator.configure(App.class.getResource("/log4j.properties"));
//	}

	private static Log log = LogFactory.getLog(App.class);

	public static void main(String[] args)
	{
		DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS Z");
		log.info("App started at: " + dateFormat.format(new Date()));
		
		//TODO

		log.info("App stopped at: " + dateFormat.format(new Date()));
	}
}
