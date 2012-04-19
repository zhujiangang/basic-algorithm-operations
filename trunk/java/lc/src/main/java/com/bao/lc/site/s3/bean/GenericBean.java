package com.bao.lc.site.s3.bean;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class GenericBean
{
	private static Log log = LogFactory.getLog(GenericBean.class);
	
	private List<String> fields = null;
	private int primaryKey = 0;
	
	public GenericBean(List<String> fieldList, int primaryKey)
	{
		if(fieldList == null || fieldList.isEmpty())
		{
			fields = new ArrayList<String>(0);
		}
		else
		{
			fields = new ArrayList<String>(fieldList.size());
			fields.addAll(fieldList);
		}
		this.primaryKey = primaryKey;
	}
	public GenericBean(List<String> fieldList)
	{
		this(fieldList, 0);
	}
	
	public GenericBean(String[] a)
	{
		this(Arrays.asList(a));
	}
	
	public String getField(int index)
	{
		if(index < 0 || index >= fields.size())
		{
			fail(index, fields.size());
		}
		return fields.get(index);
	}
	
	public void setPrimaryKey(int index)
	{
		this.primaryKey = index;
	}
	
	public String toString()
	{
		return getField(primaryKey);
	}
	
	public boolean equals(Object obj)
	{
		if(!(obj instanceof GenericBean))
		{
			return false;
		}
		GenericBean other = (GenericBean)obj;
		if(fields.size() != other.fields.size())
		{
			return false;
		}
		for(int i = 0, size = fields.size(); i < size; i++)
		{
			String a = fields.get(i);
			String b = other.fields.get(i);
			if(!a.equals(b))
			{
				return false;
			}
		}
		return true;
	}
	
	public int hashCode()
	{
		return fields.hashCode();
	}
	
	public static int indexOf(String field, int keyIndex, List<GenericBean> dataList)
	{		
		for(int i = 0, size = dataList.size(); i < size; i++)
		{
			GenericBean bean = dataList.get(i);
			
			if(keyIndex < 0 || keyIndex >= bean.fields.size())
			{
				fail(keyIndex, bean.fields.size());
			}
			
			if(field.equals(bean.fields.get(keyIndex)))
			{
				return i;
			}
		}
		return -1;
	}
	
	public static GenericBean get(String field, int keyIndex, List<GenericBean> dataList)
	{
		int index = indexOf(field, keyIndex, dataList);
		if(index == -1)
		{
			return null;
		}
		return dataList.get(index);
	}
	
	public static List<String> getFieldList(int keyIndex, List<GenericBean> dataList)
	{
		List<String> fieldList = new ArrayList<String>(dataList.size());
		for(int i = 0, size = dataList.size(); i < size; i++)
		{
			GenericBean bean = dataList.get(i);
			
			if(keyIndex < 0 || keyIndex >= bean.fields.size())
			{
				fail(keyIndex, bean.fields.size());
			}
			
			fieldList.add(bean.getField(keyIndex));
		}
		
		return fieldList;
	}
	
	public static String getField(String key, int keyIndex, int valueIndex, List<GenericBean> beans)
	{
		GenericBean bean = GenericBean.get(key, keyIndex, beans);
		if(bean == null)
		{
			log.error("The key: " + key + " doesn't exist.");
			throw new IllegalArgumentException("The key: " + key + " doesn't exist.");
//			return "null";
		}
		return bean.getField(valueIndex);
	}
	
	private static void fail(int rangeHigh, int value)
	{
		throw new IllegalArgumentException("index [" + value + "] is out of range [0-" + rangeHigh + "]" );
	}
}
