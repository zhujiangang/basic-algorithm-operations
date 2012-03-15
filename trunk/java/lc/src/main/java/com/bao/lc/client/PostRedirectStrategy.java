package com.bao.lc.client;

import java.util.List;
import java.util.Vector;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.Header;
import org.apache.http.HttpRequest;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.ProtocolException;
import org.apache.http.RequestLine;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.impl.client.DefaultRedirectStrategy;
import org.apache.http.protocol.HttpContext;

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

				log.info("Redirect: statusCode=" + statusCode + ", location="
					+ locationHeader.getValue());
			}
		}
		return ret;
	}

	public HttpUriRequest getRedirect(final HttpRequest request, final HttpResponse response,
		final HttpContext context) throws ProtocolException
	{
		HttpUriRequest redirectReq = super.getRedirect(request, response, context);
		if(redirectChain.isEmpty())
		{
			redirectChain.add(toUriRequest(request));
		}
		redirectChain.add(redirectReq);
		
		return redirectReq;
	}
	
	public static HttpUriRequest toUriRequest(final HttpRequest request)
	{
		RequestLine rawRequestLine = request.getRequestLine();
		if(rawRequestLine.getMethod().equalsIgnoreCase(HttpPost.METHOD_NAME))
		{
			return new HttpPost(rawRequestLine.getUri());
		}
		else
		{
			return new HttpGet(rawRequestLine.getUri());
		}
	}
	
	public List<HttpUriRequest> getRedirectChain()
	{
		return this.redirectChain;
	}
	
	public boolean isRedirected()
	{
		return this.redirectChain.size() >= 2;
	}
	
	public void resetChain()
	{
		redirectChain.clear();
	}
	
	public HttpUriRequest getFinalRequest()
	{
		if(!isRedirected())
		{
			return null;
		}
		return redirectChain.get(redirectChain.size() - 1);
	}
}
