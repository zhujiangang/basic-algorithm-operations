package com.bao.lc.common;

import org.apache.commons.lang.ObjectUtils;
import org.apache.commons.lang.builder.HashCodeBuilder;

public class HttpResult implements IDValuePair
{
	private int code;
	private String url;
	private HttpMethod method;
	
	public HttpResult(int code, String url, HttpMethod method)
	{
		this.code = code;
		this.url = url;
		this.method = method;
	}
	
	public HttpResult(int code, String url)
	{
		this(code, url, HttpMethod.GET);
	}
	
	@Override
	public int getID()
	{
		return ResultCode.RC_HTTP_ERROR.getID();
	}

	@Override
	public String getValue()
	{
		StringBuilder sb = new StringBuilder("Code=");
		sb.append(code);
		sb.append(", ").append(method.name());
		sb.append(" ").append(url);
		
		return sb.toString();
	}
	
	@Override
	public String toString()
	{
		return ResultCode.RC_HTTP_ERROR + ": " + getValue();
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if(this == obj)
		{
			return true;
		}
		if(!(obj instanceof HttpResult))
		{
			return false;
		}
		HttpResult that = (HttpResult) obj;
		return (code == that.code) && ObjectUtils.equals(url, that.url) && (method == that.method);
	}
	
	@Override
	public int hashCode()
	{
		HashCodeBuilder hb = new HashCodeBuilder();
		hb.append(code).append(url).append(method.ordinal());
		return hb.toHashCode();
	}

	
	public static enum HttpMethod
	{
		GET,
		POST;
	}
}
