package com.bao.lc.proxy;

import java.io.UnsupportedEncodingException;

import org.apache.commons.lang.math.NumberUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import net.grinder.tools.tcpproxy.ConnectionDetails;
import net.grinder.tools.tcpproxy.EndPoint;
import net.grinder.tools.tcpproxy.TCPProxyFilter;

public class RequestCookieFilter implements TCPProxyFilter
{
	private static Log log = LogFactory.getLog(RequestCookieFilter.class);
	
	private String host = null;
	private int port = 0;
	private String cookie = null;
	
	public RequestCookieFilter()
	{
	}
	
	private boolean updateParameters()
	{
		host = System.getProperty("RequestCookieFilter.host");
		port = NumberUtils.toInt(System.getProperty("RequestCookieFilter.port"), 443);
		cookie = System.getProperty("RequestCookieFilter.cookie");
		
		if(host == null || host.isEmpty())
		{
			log.error("Can't find the key 'RequestCookieFilter.host'");
			return false;
		}
		
		if(cookie == null || cookie.isEmpty())
		{
			log.error("Can't find the key 'RequestCookieFilter.cookie'");
			return false;
		}
		
		return true;
	}
	
	@Override
	public byte[] handle(ConnectionDetails connectionDetails, byte[] buffer, int bytesRead)
		throws FilterException
	{
		if(updateParameters())
		{
			if(log.isDebugEnabled())
			{
				StringBuilder sb = new StringBuilder("parameter:");
				sb.append(host).append(":").append(port).append(" - ").append(cookie);
				log.debug(sb.toString());
			}
			
			byte[] replacement = null;
			try
			{
				replacement = replaceCookie(connectionDetails, buffer, bytesRead);
				if(replacement != null)
				{
					return replacement;
				}
			}
			catch(UnsupportedEncodingException e)
			{
				log.error("replaceCookie failed.", e);
			}
		}
		return null;
	}
	
	private byte[] replaceCookie(ConnectionDetails connectionDetails, byte[] buffer, int bytesRead) throws UnsupportedEncodingException
	{
		EndPoint target = new EndPoint(host, port);
		if(connectionDetails.getRemoteEndPoint().equals(target))
		{
			String str = new String(buffer, 0, bytesRead, "ISO-8859-1");
			
			String regex = "Cookie: (.*?)(\\r\\n){1}";
			String replacement = "Cookie: " + cookie + "$2";
			String fake = str.replaceAll(regex, replacement);
			
			return fake.getBytes("ISO-8859-1");
		}
		
		return null;
	}

	@Override
	public void connectionOpened(ConnectionDetails connectionDetails) throws FilterException
	{
		log.debug("--- " + connectionDetails + " opened --");

	}

	@Override
	public void connectionClosed(ConnectionDetails connectionDetails) throws FilterException
	{
		log.debug("--- " + connectionDetails + " closed --");
	}

}
