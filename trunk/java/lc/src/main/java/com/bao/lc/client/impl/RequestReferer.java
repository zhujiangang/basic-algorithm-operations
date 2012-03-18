package com.bao.lc.client.impl;

import java.io.IOException;
import java.net.InetAddress;
import java.net.URI;
import java.net.URISyntaxException;

import org.apache.http.HttpConnection;
import org.apache.http.HttpException;
import org.apache.http.HttpHost;
import org.apache.http.HttpInetConnection;
import org.apache.http.HttpRequest;
import org.apache.http.HttpRequestInterceptor;
import org.apache.http.HttpVersion;
import org.apache.http.ProtocolVersion;
import org.apache.http.client.utils.URIUtils;
import org.apache.http.protocol.ExecutionContext;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.client.params.MiscParams;

public class RequestReferer implements HttpRequestInterceptor
{
	public RequestReferer()
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

		ProtocolVersion ver = request.getRequestLine().getProtocolVersion();
		String method = request.getRequestLine().getMethod();
		if(method.equalsIgnoreCase("CONNECT") && ver.lessEquals(HttpVersion.HTTP_1_0))
		{
			return;
		}

		String referer = MiscParams.getReferer(request.getParams());
		if(referer != null && !referer.isEmpty())
		{
			URI refererURI = URI.create(referer);
			if(!refererURI.isAbsolute())
			{
				HttpHost host = getTargetHost(request, context);
				
				try
				{
					refererURI = URIUtils.rewriteURI(refererURI, host, false);
				}
				catch(URISyntaxException e)
				{
					HttpException y = new HttpException(e.getMessage(), e);
					throw y;
				}
			}
			request.addHeader("Referer", refererURI.toString());
		}
	}

	private HttpHost getTargetHost(final HttpRequest request, final HttpContext context)
		throws HttpException, IOException
	{
		HttpHost targethost = (HttpHost) context.getAttribute(ExecutionContext.HTTP_TARGET_HOST);
		if(targethost == null)
		{
			HttpConnection conn = (HttpConnection) context
				.getAttribute(ExecutionContext.HTTP_CONNECTION);
			if(conn instanceof HttpInetConnection)
			{
				// Populate the context with a default HTTP host based on the
				// inet address of the target host
				InetAddress address = ((HttpInetConnection) conn).getRemoteAddress();
				int port = ((HttpInetConnection) conn).getRemotePort();
				if(address != null)
				{
					targethost = new HttpHost(address.getHostName(), port);
				}
			}
		}
		return targethost;
	}
}
