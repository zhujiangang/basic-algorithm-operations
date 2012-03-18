package com.bao.lc.httpcommand.utils;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ContextBase;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.client.BrowserClient;
import com.bao.lc.httpcommand.params.HttpCommandPNames;

public class HttpCommandUtils
{

	public static Context createContext(BrowserClient httpClient, HttpContext httpContext,
		HttpUriRequest request)
	{
		Context context = new ContextBase();
	
		context.put(HttpCommandPNames.TARGET_REQUEST, request);
		context.put(HttpCommandPNames.HTTP_CLIENT, httpClient);
		if(httpContext != null)
		{
			context.put(HttpCommandPNames.HTTP_CONTEXT, httpContext);
		}
	
		return context;
	}

	public static Context createContext(BrowserClient httpClient, HttpUriRequest request)
	{
		return createContext(httpClient, null, request);
	}

	public static String getCommandResultText(boolean result)
	{
		if(result == Command.PROCESSING_COMPLETE)
		{
			return "COMPLETE";
		}
		else
		{
			return "CONTINUE";
		}
	}

}
