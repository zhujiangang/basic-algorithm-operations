package com.bao.lc.client.utils;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.Closeable;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.List;

import org.apache.commons.io.FileUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.Header;
import org.apache.http.HeaderElement;
import org.apache.http.HttpEntity;
import org.apache.http.HttpHost;
import org.apache.http.HttpRequest;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.ParseException;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.utils.URIUtils;
import org.apache.http.cookie.Cookie;
import org.apache.http.protocol.ExecutionContext;
import org.apache.http.protocol.HTTP;
import org.apache.http.protocol.HttpContext;
import org.apache.http.util.EntityUtils;

import com.bao.lc.AppConfig;
import com.bao.lc.common.Builder;
import com.bao.lc.util.AppUtils;

public class HttpClientUtils
{
	private static final Log log = LogFactory.getLog(HttpClientUtils.class);

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

	public static String saveToString(HttpEntity entity, String charsetName) throws IOException,
		ParseException
	{
		return saveToString(entity, charsetName, null);
	}

	public static String saveToString(HttpEntity entity, String charsetName,
		Builder<String> nameBuilder) throws IOException, ParseException
	{
		String result = EntityUtils.toString(entity, charsetName);
		if(AppConfig.getInstance().isDebug())
		{
			String fileName = ((nameBuilder != null) ? nameBuilder.build() : "TempDebugFile.html");
			String fullFileName = AppUtils.getTempFilePath(fileName);
			FileUtils.writeStringToFile(new File(fullFileName), result, charsetName);
		}
		return result;
	}

	public static String saveToString(HttpResponse rsp, String defaultCharset) throws IOException,
		ParseException
	{
		String charsetName = getCharset(rsp);
		if(charsetName == null)
		{
			charsetName = defaultCharset;
		}
		return saveToString(rsp.getEntity(), charsetName);
	}

	public static String saveToString(HttpResponse rsp) throws IOException, ParseException
	{
		return saveToString(rsp, "UTF-8");
	}

	public static String getCharset(HttpResponse rsp)
	{
		Header header = rsp.getLastHeader(HTTP.CONTENT_TYPE);
		if(header == null)
		{
			return null;
		}

		HeaderElement[] elements = header.getElements();
		for(int i = 0; i < elements.length; i++)
		{
			NameValuePair entry = elements[i].getParameterByName("charset");
			if(entry != null)
			{
				return entry.getValue();
			}
		}
		return null;
	}

	public static URI getRequestURI(final HttpRequest request, final HttpContext context)
	{
		URI uri = null;
		if(request instanceof HttpUriRequest)
		{
			uri = ((HttpUriRequest) request).getURI();
		}
		else
		{
			uri = URI.create(request.getRequestLine().getUri());
		}

		if(uri.isAbsolute())
		{
			return uri;
		}

		HttpHost targetHost = (HttpHost) context.getAttribute(ExecutionContext.HTTP_TARGET_HOST);
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
}
