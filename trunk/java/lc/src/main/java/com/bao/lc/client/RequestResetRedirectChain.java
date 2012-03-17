package com.bao.lc.client;

import java.io.IOException;
import java.net.URI;

import org.apache.http.HttpException;
import org.apache.http.HttpRequest;
import org.apache.http.HttpRequestInterceptor;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.util.HttpClientUtil;

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
				return;
			}
			URI finalURI = strategy.getFinalRequest().getURI();
			URI currentURI = HttpClientUtil.getRequestURI(request, context);
			
			if(currentURI != null && finalURI != null && !currentURI.equals(finalURI))
			{
				strategy.resetChain();
			}
		}
	}
}
