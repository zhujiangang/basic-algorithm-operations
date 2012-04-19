package com.bao.lc.site.s3.params;

import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import com.bao.lc.site.s3.bean.PassengerInfo;
import com.bao.lc.site.s3.bean.TicketFilterCondition;

public class InputParameter
{
	private static final String KEY_USER = "td.user";
	private static final String KEY_PWD = "td.password";
	private static final String KEY_FROM = "td.from_station";
	private static final String KEY_TO = "td.to_station";
	private static final String KEY_DATE = "td.ticket.date";
	private static final String KEY_TIME_RANGE = "td.ticket.time_range";
	private static final String KEY_PASSENGER_BASE = "td.passenger";
	
	
	public String user = null;
	public String pwd = null;
	public String fromStation = null;
	public String toStation = null;
	public String ticketDate = null;
	public String ticketTimeRange = "00:00--24:00";
	public List<PassengerInfo> passengers = null;
	
	public TicketFilterCondition filterCond = null;
	
	
	public InputParameter()
	{
		ticketTimeRange = "00:00--24:00";
	}
	
	public void save(Properties prop)
	{
		prop.put(KEY_USER, user);
		prop.put(KEY_PWD, pwd);
		prop.put(KEY_FROM, fromStation);
		prop.put(KEY_TO, toStation);
		prop.put(KEY_DATE, ticketDate);
		prop.put(KEY_TIME_RANGE, ticketTimeRange);
		
		if(passengers != null && !passengers.isEmpty())
		{
			for(int i = 0, size = passengers.size(); i < size; i++)
			{
				PassengerInfo passenger = passengers.get(i);
				
				prop.put(KEY_PASSENGER_BASE + String.valueOf(i + 1), passenger.saveToString());
			}
		}
		
		if(filterCond != null)
		{
			filterCond.save(prop);
		}
	}
	public void load(Properties prop)
	{
		this.user = prop.getProperty(KEY_USER, "");
		this.pwd = prop.getProperty(KEY_PWD, "");
		this.fromStation = prop.getProperty(KEY_FROM, "");
		this.toStation = prop.getProperty(KEY_TO, "");
		this.ticketDate = prop.getProperty(KEY_DATE, "");
		this.ticketTimeRange = prop.getProperty(KEY_TIME_RANGE, "");
		
		this.passengers = new ArrayList<PassengerInfo>();
		for(int i = 0; true; i++)
		{
			String value = prop.getProperty(KEY_PASSENGER_BASE + String.valueOf(i + 1));
			if(value == null)
			{
				break;
			}
			
			PassengerInfo passenger = new PassengerInfo();
			passenger.loadFromString(value);
			
			this.passengers.add(passenger);
		}
		
		this.filterCond = new TicketFilterCondition();
		this.filterCond.load(prop);
	}
}
