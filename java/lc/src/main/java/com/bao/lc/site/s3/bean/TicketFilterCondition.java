package com.bao.lc.site.s3.bean;

import java.util.List;
import java.util.Properties;

import com.bao.lc.site.s3.TdUtils;

public class TicketFilterCondition
{
	private static final String COND_BASE = "td.ticket.sort.cond.";
	
	private static final String CAT_SEAT_CLASS = "seat_class";
	private static final String CAT_TRAIN_CLASS = "train_class";
	private static final String CAT_TRAIN_NO = "train_no";
	private static final String CAT_TRAIN_NO_FIXED = "train_no.fixed";
	
	public List<String> seatClassList = null;
	public List<String> trainClassList = null;
	public List<String> trainNoList = null;
	public boolean fixedTrainNo = false;
	
	public void save(Properties prop)
	{
		prop.put(COND_BASE + CAT_SEAT_CLASS,
			TdUtils.toString(seatClassList, TdUtils.DEFAULT_FIELD_DELIM));
		
		prop.put(COND_BASE + CAT_TRAIN_CLASS,
			TdUtils.toString(trainClassList, TdUtils.DEFAULT_FIELD_DELIM));
		
		prop.put(COND_BASE + CAT_TRAIN_NO,
			TdUtils.toString(trainNoList, TdUtils.DEFAULT_FIELD_DELIM));
		
		prop.put(COND_BASE + CAT_TRAIN_NO_FIXED, String.valueOf(fixedTrainNo));
	}
	public void load(Properties prop)
	{
		String content = prop.getProperty(COND_BASE + CAT_SEAT_CLASS);
		this.seatClassList = TdUtils.toList(content, TdUtils.DEFAULT_FIELD_DELIM);
		
		content = prop.getProperty(COND_BASE + CAT_TRAIN_CLASS);
		this.trainClassList = TdUtils.toList(content, TdUtils.DEFAULT_FIELD_DELIM);
		
		content = prop.getProperty(COND_BASE + CAT_TRAIN_NO);
		this.trainNoList = TdUtils.toList(content, TdUtils.DEFAULT_FIELD_DELIM);
		
		content = prop.getProperty(COND_BASE + CAT_TRAIN_NO_FIXED);
		this.fixedTrainNo = new Boolean(content);
	}
}
