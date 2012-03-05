package com.bao.examples;

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
		init();
	}

	private static Log log = LogFactory.getLog(App.class);
	
	private static void init()
	{
		PropertyConfigurator.configure(App.class.getResource("/log4j.properties"));
	}

	public static void main(String[] args)
	{
		log.info("Hello World!");
	}
}
