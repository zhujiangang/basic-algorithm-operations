package com.bao.lc.client;

import java.io.IOException;
import java.net.InetAddress;

import org.apache.http.HttpConnection;
import org.apache.http.HttpException;
import org.apache.http.HttpHost;
import org.apache.http.HttpInetConnection;
import org.apache.http.HttpRequest;
import org.apache.http.HttpRequestInterceptor;
import org.apache.http.HttpVersion;
import org.apache.http.ProtocolVersion;
import org.apache.http.protocol.ExecutionContext;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.util.CommonUtil;

public class RequestReferer implements HttpRequestInterceptor
{
	public static final String REFERER_INTERNAL = "lc.referer";

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

		Object refererInternal = request.getParams().getParameter(REFERER_INTERNAL);
		if(refererInternal != null && !refererInternal.toString().isEmpty())
		{
			HttpHost host = getTargetHost(request, context);
			String referer = CommonUtil.getAbsoluteURI(refererInternal.toString(), host);
			request.addHeader("Referer", referer);
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
