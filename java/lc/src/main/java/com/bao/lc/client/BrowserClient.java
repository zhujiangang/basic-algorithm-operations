package com.bao.lc.client;

import java.io.IOException;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.RedirectStrategy;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.params.CookiePolicy;
import org.apache.http.client.params.HttpClientParams;
import org.apache.http.conn.ClientConnectionManager;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager;
import org.apache.http.params.HttpParams;
import org.apache.http.params.HttpProtocolParams;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.client.impl.PostRedirectStrategy;
import com.bao.lc.client.impl.RequestReferer;
import com.bao.lc.client.impl.RequestResetRedirectChain;
import com.bao.lc.client.params.MiscParams;

public class BrowserClient extends DefaultHttpClient
{
	public static final String AGENT_IE7 = "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.1; Trident/5.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; TEN)";
	public static final String AGENT_IE8 = "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; InfoPath.2; MS-RTC LM 8; FDM; .NET4.0C)";
	public static final String AGENT_CHROME = "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/535.2 (KHTML, like Gecko) Chrome/15.0.874.106 Safari/535.2";
	public static final String AGENT_FIREFOX = "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6 (.NET CLR 3.5.30729)";

	/**
	 * Creates a new HTTP client from parameters and a connection manager.
	 * 
	 * @param params
	 *            the parameters
	 * @param conman
	 *            the connection manager
	 */
	public BrowserClient(final ClientConnectionManager conman, final HttpParams params)
	{
		super(conman, params);
		init();
	}

	/**
	 * @since 4.1
	 */
	public BrowserClient(final ClientConnectionManager conman)
	{
		super(conman, null);
		init();
	}

	public BrowserClient(final HttpParams params)
	{
		super(null, params);
		init();
	}

	public BrowserClient()
	{
		super(null, null);
		init();
	}
	
	protected void init()
	{
		//Parameters
		HttpParams params = getParams();
		
		//Set Agent
		HttpProtocolParams.setUserAgent(params, AGENT_IE8);
		HttpClientParams.setCookiePolicy(params, CookiePolicy.BROWSER_COMPATIBILITY);
		
		//Set redirect strategy
		RedirectStrategy redirectStrategy = new PostRedirectStrategy();
		setRedirectStrategy(redirectStrategy);
		MiscParams.setRedirectStrategy(params, redirectStrategy);
		
		//Interceptors
		addRequestInterceptor(new RequestReferer());
		addRequestInterceptor(new RequestResetRedirectChain());
	}

	protected ClientConnectionManager createClientConnectionManager()
	{
		return new ThreadSafeClientConnManager();
	}

	public HttpResponse execute(RequestBuilder builder, HttpContext context)
		throws ClientProtocolException, IOException
	{
		HttpUriRequest request = builder.create();

		HttpResponse rsp = execute(request, context);

		MiscParams.setReferer(getParams(), request.getURI().toString());

		return rsp;
	}

	public HttpResponse execute(RequestBuilder builder) throws ClientProtocolException, IOException
	{
		return execute(builder, (HttpContext) null);
	}

	public HttpResponse execute(String uriString, HttpContext context)
		throws ClientProtocolException, IOException
	{
		RequestBuilder builder = new RequestBuilder();
		builder.uriStr(uriString);

		return execute(builder, context);
	}

	public HttpResponse execute(String uriString) throws ClientProtocolException, IOException
	{
		return execute(uriString, (HttpContext) null);
	}
}
