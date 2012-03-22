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

	private static void showInteger(char l, char h)
	{
		for(char c = l; c <= h; c++)
		{
			System.out.printf("char=%c, int=%3d(%02X)\n", c, (int)c, (int)c);
		}
	}
	public static void main(String[] args)
	{
		log.info("Hello World!");
		
		showInteger('0', '9');
		showInteger('a', 'z');
		showInteger('A', 'Z');
		showInteger('%', '%');
	}
}
