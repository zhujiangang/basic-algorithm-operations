package com.bao.lc.client.impl;

import java.net.URI;
import java.util.List;
import java.util.Vector;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.Header;
import org.apache.http.HttpRequest;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.ProtocolException;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.impl.client.DefaultRedirectStrategy;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.client.utils.HttpClientUtils;

public class PostRedirectStrategy extends DefaultRedirectStrategy
{
	private static final Log log = LogFactory.getLog(PostRedirectStrategy.class);
	
	private List<HttpUriRequest> redirectChain = new Vector<HttpUriRequest>(2);

	public PostRedirectStrategy()
	{
		super();
	}

	public boolean isRedirected(final HttpRequest request, final HttpResponse response,
		final HttpContext context) throws ProtocolException
	{
		boolean ret = super.isRedirected(request, response, context);

		if(!ret)
		{
			int statusCode = response.getStatusLine().getStatusCode();
			Header locationHeader = response.getFirstHeader("location");

			if((locationHeader != null)
				&& (statusCode == HttpStatus.SC_MOVED_TEMPORARILY || statusCode == HttpStatus.SC_MOVED_PERMANENTLY))
			{
				ret = true;
			}
		}
		return ret;
	}

	public HttpUriRequest getRedirect(final HttpRequest request, final HttpResponse response,
		final HttpContext context) throws ProtocolException
	{
		HttpUriRequest redirectReq = super.getRedirect(request, response, context);
		
		//record the redirect request
		redirectChain.add(redirectReq);
		
		//log if possible
		if(log.isDebugEnabled())
		{
			String redirectLog = getRedirectLog(request, redirectReq, context);
			if(redirectLog != null)
			{
				log.debug(redirectLog);
			}
		}
		
		return redirectReq;
	}
	
	private String getRedirectLog(final HttpRequest request, final HttpUriRequest redirectReq,
		final HttpContext context)
	{
		// 1. From URI
		URI fromURI = HttpClientUtils.getRequestURI(request, context);
		if(fromURI == null)
		{
			return null;
		}

		// 2. To URI
		URI toURI = redirectReq.getURI();

		StringBuilder sb = new StringBuilder();
		sb.append("[Redirect]: count=").append(redirectChain.size());
		sb.append(", ").append(fromURI.toString()).append("==>").append(toURI.toString());
		return sb.toString();
	}
	
	public List<HttpUriRequest> getRedirectChain()
	{
		return this.redirectChain;
	}
	
	public boolean isPostRedirected()
	{
		return !redirectChain.isEmpty();
	}
	
	public void resetChain()
	{
		redirectChain.clear();
	}
	
	public HttpUriRequest getFinalRequest()
	{
		if(!isPostRedirected())
		{
			return null;
		}
		return redirectChain.get(redirectChain.size() - 1);
	}
}
