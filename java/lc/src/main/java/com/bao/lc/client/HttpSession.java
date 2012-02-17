package com.bao.lc.client;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.cookie.Cookie;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import com.bao.lc.util.HttpClientUtil;

public class HttpSession
{
	private static Log log = LogFactory.getLog(HttpSession.class);

	public DefaultHttpClient hc = null;

	public HttpSession()
	{
		hc = new DefaultHttpClient();
	}

	public HttpResponse get(String url, Map<String, String> headers,
		Map<String, String> params, String encoding)
		throws ClientProtocolException, IOException
	{
		// parameters
		if(params != null && !params.isEmpty())
		{
			url = url + HttpClientUtil.assemblyParameter(params);
		}

		// Get
		HttpGet hp = new HttpGet(url);
		// Headers
		if(null != headers)
		{
			hp.setHeaders(HttpClientUtil.assemblyHeader(headers));
		}

		HttpResponse rsp = hc.execute(hp);

		if(log.isDebugEnabled())
		{
			log.debug(debugString(rsp));
		}
		
//		hc.getCookieStore().clear();

		return rsp;
	}

	public HttpResponse get(String url) throws ClientProtocolException,
		IOException
	{
		return get(url, null, null, "UTF-8");
	}

	public HttpResponse post(String url, Map<String, String> headers,
		Map<String, String> params, String encoding)
		throws ClientProtocolException, IOException
	{
		HttpPost post = new HttpPost(url);

		// 设置需要提交的参数
		List<NameValuePair> list = new ArrayList<NameValuePair>();
		for(String temp : params.keySet())
		{
			list.add(new BasicNameValuePair(temp, params.get(temp)));
		}
		post.setEntity(new UrlEncodedFormEntity(list, encoding));

		// 设置头部
		if(null != headers)
			post.setHeaders(HttpClientUtil.assemblyHeader(headers));

		// 实行请求并返回
		HttpResponse rsp = hc.execute(post);

		if(log.isDebugEnabled())
		{
			log.debug(debugString(rsp));
		}

		return rsp;
	}

	private String debugString(HttpResponse rsp)
	{
		StringBuilder sb = new StringBuilder();

		sb.append("StatusCode=").append(rsp.getStatusLine().getStatusCode());

		List<Cookie> cookies = hc.getCookieStore().getCookies();
		sb.append(", CookieCount=").append(cookies.size());
		sb.append("\r\n");

		for(Iterator<Cookie> iter = cookies.iterator(); iter.hasNext();)
		{
			Cookie cookie = iter.next();

			sb.append(cookie).append("\r\n");
		}

		return sb.toString();
	}
}
