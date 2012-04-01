package com.bao.lc.httpcommand.params;

import java.net.URI;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.RedirectStrategy;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.client.BrowserClient;
import com.bao.lc.client.impl.PostRedirectStrategy;
import com.bao.lc.client.utils.HttpClientUtils;

public class HttpCommandParams
{
	private HttpCommandParams()
	{
	}

	public static HttpUriRequest getTargetRequest(Context context)
	{
		HttpUriRequest request = (HttpUriRequest) context.get(HttpCommandPNames.TARGET_REQUEST);
		return request;
	}

	public static URI getTargetRequestURI(Context context)
	{
		HttpUriRequest request = getTargetRequest(context);
		if(request == null)
		{
			return null;
		}
		return request.getURI();
	}

	public static HttpResponse getResponse(Context context)
	{
		HttpResponse rsp = (HttpResponse) context.get(HttpCommandPNames.TARGET_RESPONSE);
		if(rsp == null)
		{
			throw new IllegalStateException("Empty HTTP_RESPONSE.");
		}
		return rsp;
	}
	
	public static String getCharset(HttpResponse rsp, Context context)
	{
		String charsetName = HttpClientUtils.getCharset(rsp);
		if(charsetName == null)
		{
			charsetName = HttpCommandParams.getDefaultResponseCharset(context);
		}
		return charsetName;
	}
	
	public static String getDefaultResponseCharset(Context context)
	{
		String charset = MapUtils.getString(context, HttpCommandPNames.RESPONSE_DEFAULT_CHARSET);
		if(charset == null)
		{
			return "UTF-8";
		}
		return charset;
	}

	public static IDValuePair getResultCode(Context context)
	{
		IDValuePair rc = (IDValuePair) context.get(HttpCommandPNames.HTTP_COMMAND_RESULT_CODE);
		if(rc == null)
		{
			throw new IllegalStateException("Empty HTTP_COMMAND_RESULT_CODE.");
		}
		return rc;
	}

	public static BrowserClient getBrowserClient(Context context)
	{
		HttpClient httpClient = getHttpClient(context);
		if(!(httpClient instanceof BrowserClient))
		{
			throw new IllegalStateException("httpClient is not BrowserClient. class="
				+ httpClient.getClass());
		}

		return (BrowserClient) httpClient;
	}
	
	public static PostRedirectStrategy getRedirectStrategy(Context context)
	{
		RedirectStrategy redirectStrategy = getBrowserClient(context).getRedirectStrategy();
		if(redirectStrategy instanceof PostRedirectStrategy)
		{
			return (PostRedirectStrategy) redirectStrategy;
		}
		return null;
	}

	public static HttpClient getHttpClient(Context context)
	{
		HttpClient httpClient = (HttpClient) context.get(HttpCommandPNames.HTTP_CLIENT);
		if(httpClient == null)
		{
			throw new IllegalStateException("Empty HTTP_CLIENT.");
		}
		return httpClient;
	}

	public static HttpContext getHttpContext(Context context)
	{
		HttpContext httpContext = (HttpContext) context.get(HttpCommandPNames.HTTP_CONTEXT);
		return httpContext;
	}

	public static Command getExceptionCommand(Context context)
	{
		return (Command) context.get(HttpCommandPNames.EXCEPTION_COMMAND);
	}
	
	public static void purgeRequestParams(Context context)
	{
		context.remove(HttpCommandPNames.TARGET_REQUEST);
		context.remove(HttpCommandPNames.TARGET_REFERER);
	}
	public static void purgeResponseParams(Context context)
	{
		context.remove(HttpCommandPNames.TARGET_RESPONSE);
		context.remove(HttpCommandPNames.HTTP_COMMAND_RESULT_CODE);
		context.remove(HttpCommandPNames.EXCEPTION_COMMAND);
	}
}
