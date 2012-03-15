package com.bao.lc.client;

import java.io.IOException;

import org.apache.http.HttpException;
import org.apache.http.HttpRequest;
import org.apache.http.HttpRequestInterceptor;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.impl.client.RequestWrapper;
import org.apache.http.protocol.HttpContext;

public class RequestResetRedirectChain implements HttpRequestInterceptor
{
	public static final String REDIRECT_STRATEGY = "lc.redir.strategy";
	
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
		
		Object obj = request.getParams().getParameter(REDIRECT_STRATEGY);
		if(obj instanceof PostRedirectStrategy)
		{
			PostRedirectStrategy strategy = (PostRedirectStrategy)obj;
			
			if(!strategy.isRedirected())
			{
				strategy.resetChain();
				return;
			}
			HttpUriRequest finalReq = strategy.getFinalRequest();
			
			HttpRequest theReq = request;
			if(request instanceof RequestWrapper)
			{
				theReq = ((RequestWrapper)request).getOriginal();
			}
			
			HttpUriRequest currReq = null;
			if(theReq instanceof HttpUriRequest)
			{
				currReq = (HttpUriRequest)theReq;
			}
			
			if(finalReq != null && currReq != null && !finalReq.getURI().equals(currReq.getURI()))
			{
				strategy.resetChain();
			}
		}
	}
}
