package com.bao.examples.timertask;

import java.util.Calendar;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.log4j.Logger;

public class TimerTaskTest
{
	private static final Logger logger = Logger.getLogger(TimerTaskTest.class);

	public void action1()
	{
		Timer timer = new Timer(false);

		timer.schedule(new HelloWorldTimerTask("Hello World"), 2 * 1000);
		
		Calendar cal = Calendar.getInstance();
		cal.add(Calendar.SECOND, 10);
		
		timer.schedule(new HelloWorldTimerTask("Repeat Hello World"), cal.getTime(), 10 * 1000);
	}

	private static class HelloWorldTimerTask extends TimerTask
	{
		private String str = null;
		public HelloWorldTimerTask(String hello)
		{
			str = hello;
		}
		public void run()
		{
			logger.info(str);
		}
	}
	
	public static void main(String[] args)
	{
		TimerTaskTest test = new TimerTaskTest();
		test.action1();
	}
}
