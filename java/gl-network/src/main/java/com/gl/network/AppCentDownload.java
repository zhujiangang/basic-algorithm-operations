package com.gl.network;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gl.common.HttpResult;
import com.gl.util.CommonUtil;
import com.gl.util.HttpClientUtil;

public class AppCentDownload
{
	private static Log log = LogFactory.getLog(App.class);
	
	private String url;
	
	public AppCentDownload(String url)
	{
		this.url = url;
		log.info(url);
	}
	
	public void download()
	{
		HttpResult result = null;
		String content = null;
		try
		{
			//1. Get Direct download link web page
			result = HttpClientUtil.sendGet(url);
			content = HttpClientUtil.saveToString(result.rsp.getEntity());
			
			String directLinkUrl = extractDirectLink(content);
			if(directLinkUrl == null)
			{
				System.err.println("Failed to extract downloadLink");
				return;
			}
			System.out.println(directLinkUrl);
			
			
			//2. Get direct download link URL
			result = HttpClientUtil.sendGet(directLinkUrl);
			content = HttpClientUtil.saveToString(result.rsp.getEntity());

			String srcFileURL = extraceSrcFileURL(content);
			if(srcFileURL == null)
			{
				System.err.println("Failed to extract src file url");
				return;
			}
			System.out.println(srcFileURL);
			
			
			//3. Download file
			String fileName = extraceFileName(srcFileURL);
			System.out.println(fileName);
			HttpClientUtil.downloadFile(srcFileURL, fileName);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	private String extractDirectLink(String content)
	{
		String regex = "<div class=\"dlLinkWrapper\"> <a href=\"(.+)\" id=(.+)>Direct Download Link</a>";
		
		String url = CommonUtil.getRegexValue(content, regex, 1);
		return url;
	}
	
	private String extraceSrcFileURL(String content)
	{
		String regex = "src:'(.+)'";
		
		String url = CommonUtil.getRegexValue(content, regex, 1);
		return url;
	}
	
	private String extraceFileName(String url)
	{
		int beginIndex = url.lastIndexOf('/');
		if(beginIndex == -1)
		{
			return null;
		}
		
		int endIndex = url.indexOf('?', beginIndex + 1);
		if(endIndex == -1)
		{
			return null;
		}
		
		return url.substring(beginIndex + 1, endIndex);
	}
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		String url = "http://download.example.com/aaa.html";
		AppCentDownload cnetDownloader = new AppCentDownload(url);
		cnetDownloader.download();
	}

}
