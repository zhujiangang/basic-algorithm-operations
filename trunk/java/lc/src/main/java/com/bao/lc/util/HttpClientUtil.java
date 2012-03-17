package com.bao.lc.util;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.Closeable;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.Header;
import org.apache.http.HttpEntity;
import org.apache.http.HttpHost;
import org.apache.http.HttpRequest;
import org.apache.http.NameValuePair;
import org.apache.http.ParseException;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.utils.URIUtils;
import org.apache.http.cookie.Cookie;
import org.apache.http.message.BasicHeader;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.ExecutionContext;
import org.apache.http.protocol.HttpContext;
import org.apache.http.util.EntityUtils;

import com.bao.lc.AppConfig;


public class HttpClientUtil
{	
	private static final Log log = LogFactory.getLog(HttpClientUtil.class); 
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
	
	public static String saveToString(HttpEntity entity, String charsetName)  throws IOException, ParseException
	{
		String result = EntityUtils.toString(entity, charsetName);
		if(AppConfig.getInstance().isDebug())
		{
			String tempFile = AppUtils.getTempFilePath("TempDebugFile.html");
			FileUtils.writeStringToFile(new File(tempFile), result, charsetName);
		}
		return result;
	}
	
	public static URI getRequestURI(final HttpRequest request, final HttpContext context)
	{
		URI uri = null;
		if(request instanceof HttpUriRequest)
		{
			uri = ((HttpUriRequest)request).getURI();
		}
		else
		{
			uri = URI.create(request.getRequestLine().getUri());
		}
		
		if(uri.isAbsolute())
		{
			return uri;
		}
		
		HttpHost targetHost = (HttpHost)context.getAttribute(ExecutionContext.HTTP_TARGET_HOST);
		if(targetHost == null)
		{
			log.error("Can't find the target host from URI: " + uri.toString());
			return null;
		}
		
		try
		{
			uri = URIUtils.rewriteURI(uri, targetHost);
		}
		catch(URISyntaxException e)
		{
			log.error("Failed to rewriteURI.", e);
			return null;
		}
		return uri;
	}
	
	public static HttpUriRequest createRequest(URI uri, String method, Map<String, String> params,
		String encoding, Map<String, String> headers)
	{
		if(HttpPost.METHOD_NAME.equalsIgnoreCase(method))
		{
			return createHttpPost(uri, params, encoding, headers);
		}
		else
		{
			return createHttpGet(uri, params, headers);
		}
	}

	public static HttpGet createHttpGet(URI uri, Map<String, String> params, Map<String, String> headers)
	{
		// Get
		HttpGet get = null;
		// parameters
		if(params != null && !params.isEmpty())
		{
			get = new HttpGet(uri.toString() + HttpClientUtil.assemblyParameter(params));
		}
		else
		{
			get = new HttpGet(uri);
		}

		// Headers
		if(null != headers)
		{
			get.setHeaders(HttpClientUtil.assemblyHeader(headers));
		}
		return get;
	}

	public static HttpPost createHttpPost(URI uri, Map<String, String> params, String encoding,
		Map<String, String> headers)
	{
		HttpPost post = new HttpPost(uri);

		// Post data
		List<NameValuePair> list = new ArrayList<NameValuePair>();
		for(String paramName : params.keySet())
		{
			list.add(new BasicNameValuePair(paramName, params.get(paramName)));
		}
		try
		{
			post.setEntity(new UrlEncodedFormEntity(list, encoding));
		}
		catch(UnsupportedEncodingException e)
		{
			IllegalArgumentException e2 = new IllegalArgumentException();
			e2.initCause(e);
			throw e2;
		}

		// Headers
		if(null != headers)
		{
			post.setHeaders(HttpClientUtil.assemblyHeader(headers));
		}

		return post;
	}
}
