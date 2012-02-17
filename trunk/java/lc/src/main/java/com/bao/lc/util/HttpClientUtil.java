package com.bao.lc.util;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.Closeable;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.http.Header;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.cookie.Cookie;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicHeader;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import com.bao.lc.common.*;

public class HttpClientUtil
{
	/**
	 * GET Request
	 * 
	 * @param url
	 * @param headers
	 * @param params
	 * @param encoding
	 * @param isAbort
	 * @return
	 * @throws ClientProtocolException
	 * @throws IOException
	 */
	public static HttpResult sendGet(String url, Map<String, String> headers,
		Map<String, String> params, String encoding, boolean isAbort)
		throws ClientProtocolException, IOException
	{
		// 实例化一个Httpclient的
		DefaultHttpClient client = new DefaultHttpClient();
		// 如果有参数的就拼装起来
		if(params != null && !params.isEmpty())
		{
			url = url + assemblyParameter(params);
		}
		// 这是实例化一个get请求
		HttpGet hp = new HttpGet(url);
		// 如果需要头部就组装起来
		if(null != headers)
			hp.setHeaders(assemblyHeader(headers));
		// 执行请求后返回一个HttpResponse
		HttpResponse response = client.execute(hp);
		// 如果为true则断掉这个get请求
		if(isAbort)
			hp.abort();

		HttpResult result = new HttpResult(client, response);
		return result;
	}

	public static HttpResult sendGet(String url, Map<String, String> headers,
		Map<String, String> params, String encoding)
		throws ClientProtocolException, IOException
	{
		return sendGet(url, headers, params, encoding, false);
	}

	public static HttpResult sendGet(String url)
		throws ClientProtocolException, IOException
	{
		return sendGet(url, null, null, "UTF-8", false);
	}

	// 这是模拟post请求
	public static HttpResult sendPost(String url, Map<String, String> headers,
		Map<String, String> params, String encoding)
		throws ClientProtocolException, IOException
	{
		// 实例化一个Httpclient的
		DefaultHttpClient client = new DefaultHttpClient();
		// 实例化一个post请求
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
			post.setHeaders(assemblyHeader(headers));

		// 实行请求并返回
		HttpResponse response = client.execute(post);

		HttpResult result = new HttpResult(client, response);
		return result;
	}

	/**
	 * Assembly Cookies
	 * 
	 * @param cookies
	 * @return
	 */
	public static String assemblyCookie(List<Cookie> cookies)
	{
		StringBuilder sb = new StringBuilder();
		for(Cookie cookie : cookies)
		{
			sb.append(cookie.getName()).append("=").append(cookie.getValue());
			sb.append("; ");
		}
		if(sb.length() > 2)
		{
			return sb.substring(0, sb.length() - 2);
		}
		else
		{
			return "";
		}
	}

	/**
	 * Assembly Headers
	 * 
	 * @param headers
	 * @return
	 */
	public static Header[] assemblyHeader(Map<String, String> headers)
	{
		Header[] allHeader = new BasicHeader[headers.size()];
		int i = 0;
		for(String str : headers.keySet())
		{
			allHeader[i] = new BasicHeader(str, headers.get(str));
			i++;
		}
		return allHeader;
	}

	/**
	 * Assembly Parameters
	 * 
	 * @param paramMap
	 * @return
	 */
	public static String assemblyParameter(Map<String, String> paramMap)
	{
		StringBuilder sb = new StringBuilder("?");
		for(String str : paramMap.keySet())
		{
			sb.append(str).append("=").append(paramMap.get(str));
			sb.append("&");
		}
		return sb.substring(0, sb.length() - 1);
	}

	public static void closeStream(Closeable stream)
	{
		if(stream != null)
		{
			try
			{
				stream.close();
			}
			catch(IOException e)
			{
				e.printStackTrace();
			}
		}
	}

	public static void saveToFile(HttpEntity entity, String fileName)
	{
		BufferedInputStream bis = null;
		BufferedOutputStream bos = null;
		try
		{
			bis = new BufferedInputStream(entity.getContent());
			bos = new BufferedOutputStream(new FileOutputStream(fileName));

			byte[] bytes = new byte[1024];
			int nBytes = -1;
			while((nBytes = bis.read(bytes)) != -1)
			{
				bos.write(bytes, 0, nBytes);
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		finally
		{
			closeStream(bis);
			closeStream(bos);
		}
	}

	public static String saveToString(HttpEntity entity)
	{
		String str = null;
		try
		{
			str = EntityUtils.toString(entity, "utf-8");
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}

		if(str == null)
		{
			str = "";
		}

		return str;
	}

	public static void downloadFile(String URL, String fileName)
		throws ClientProtocolException, IOException
	{
		HttpResult result = sendGet(URL);
		saveToFile(result.rsp.getEntity(), fileName);
	}
}
