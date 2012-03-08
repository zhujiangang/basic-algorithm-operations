package com.bao.lc.util;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.Closeable;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.apache.http.Header;
import org.apache.http.HttpEntity;
import org.apache.http.ParseException;
import org.apache.http.cookie.Cookie;
import org.apache.http.message.BasicHeader;
import org.apache.http.util.EntityUtils;

import com.bao.lc.AppConfig;


public class HttpClientUtil
{	
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
}
