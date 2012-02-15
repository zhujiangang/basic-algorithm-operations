package com.bao.hc;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.PropertyConfigurator;

/**
 * Hello world!
 * 
 */
public class App
{
	static
	{		
		PropertyConfigurator.configure(App.class.getResource("/commons-logging.properties"));
	}

	private static Log log = LogFactory.getLog(App.class);

	public static void main(String[] args)
	{
		log.info("Hello World!");
	}
}
