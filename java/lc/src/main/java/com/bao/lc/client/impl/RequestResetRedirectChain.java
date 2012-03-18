package com.bao.lc.client.impl;

import java.io.IOException;
import java.net.URI;

import org.apache.http.HttpException;
import org.apache.http.HttpRequest;
import org.apache.http.HttpRequestInterceptor;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.client.params.MiscParams;
import com.bao.lc.client.utils.HttpClientUtils;

public class RequestResetRedirectChain implements HttpRequestInterceptor
{
	public RequestResetRedirectChain()
	{
		super();
	}

	public void process(final HttpRequest request, final HttpContext context) throws HttpException,
		IOException
	{
		if(request == null)
		{
			throw new IllegalArgumentException("HTTP request may not be null");
		}
		if(context == null)
		{
			throw new IllegalArgumentException("HTTP context may not be null");
		}
		
		PostRedirectStrategy strategy = MiscParams.getPostRedirectStrategy(request.getParams());
		if(strategy != null && strategy.isPostRedirected())
		{
			URI finalURI = strategy.getFinalRequest().getURI();
			URI currentURI = HttpClientUtils.getRequestURI(request, context);
			
			if(currentURI != null && finalURI != null && !currentURI.equals(finalURI))
			{
				strategy.resetChain();
			}
		}
	}
}
