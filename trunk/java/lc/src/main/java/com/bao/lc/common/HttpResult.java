package com.bao.lc.common;

import org.apache.http.HttpResponse;
import org.apache.http.impl.client.DefaultHttpClient;

public class HttpResult
{
	public DefaultHttpClient hc;
	public HttpResponse rsp;
	
	public HttpResult()
	{
		hc = null;
		rsp = null;
	}
	
	public HttpResult(DefaultHttpClient hc, HttpResponse rsp)
	{
		this.hc = hc;
		this.rsp = rsp;
	}
}
