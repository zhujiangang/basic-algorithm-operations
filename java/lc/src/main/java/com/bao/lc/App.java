package com.bao.lc;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.PropertyConfigurator;

import com.bao.lc.client.CnetUser;

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
		
		CnetUser cnetUser = new CnetUser();
		cnetUser.login("colinconger77@yahoo.com", "3562835");
	}
}
