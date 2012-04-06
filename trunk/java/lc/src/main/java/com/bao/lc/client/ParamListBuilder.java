package com.bao.lc.client;

import java.util.ArrayList;
import java.util.List;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;

import com.bao.lc.common.Builder;

public class ParamListBuilder implements Builder<List<NameValuePair>>
{
	private List<NameValuePair> paramList = null;

	public ParamListBuilder()
	{
		reset();
	}

	private void reset()
	{
		if(paramList != null)
		{
			paramList.clear();
		}
		else
		{
			paramList = new ArrayList<NameValuePair>();
		}
	}
	
	public void append(String name, String value)
	{
		paramList.add(new BasicNameValuePair(name, value));
	}

	public List<NameValuePair> build()
	{
		List<NameValuePair> result = null;
		try
		{
			result = new ArrayList<NameValuePair>(paramList.size());
			result.addAll(this.paramList);
		}
		finally
		{
			// reset the properties
			reset();
		}
		return result;
	}
}
