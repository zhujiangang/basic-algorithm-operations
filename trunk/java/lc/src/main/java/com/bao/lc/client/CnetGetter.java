package com.bao.lc.client;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.common.HttpResult;
import com.bao.lc.common.exception.ParseException;
import com.bao.lc.util.CommonUtil;
import com.bao.lc.util.HttpClientUtil;

public class CnetGetter implements Getter
{
	private static Log log = LogFactory.getLog(CnetGetter.class);

	private String url;

	public CnetGetter(String url)
	{
		this.url = url;
		log.info(url);
	}

	public int get()
	{
		int rc = 0;
		HttpResult result = null;
		String content = null;
		try
		{
			// 1. Get Direct download link web page
			result = HttpClientUtil.sendGet(url);
			content = HttpClientUtil.saveToString(result.rsp.getEntity());

			String directLinkUrl = extractDirectLink(content);
			if(directLinkUrl == null)
			{
				System.err.println("Failed to extract downloadLink");
				return rc;
			}
			System.out.println(directLinkUrl);

			// 2. Get direct download link URL
			result = HttpClientUtil.sendGet(directLinkUrl);
			content = HttpClientUtil.saveToString(result.rsp.getEntity());

			String srcFileURL = extraceSrcFileURL(content);
			if(srcFileURL == null)
			{
				System.err.println("Failed to extract src file url");
				return rc;
			}
			System.out.println(srcFileURL);

			// 3. Download file
			String fileName = extraceFileName(srcFileURL);
			System.out.println(fileName);
			HttpClientUtil.downloadFile(srcFileURL, fileName);
		}
		catch(Exception e)
		{
			e.printStackTrace();
			
			rc = -1;
		}

		return rc;
	}

	private String extractDirectLink(String content) throws ParseException
	{
		String regex = "<div class=\"dlLinkWrapper\"> <a href=\"(.+)\" id=(.+)>Direct Download Link</a>";

		String url = CommonUtil.getRegexValueOnce(content, regex, 1);
		return url;
	}

	private String extraceSrcFileURL(String content) throws ParseException
	{
		String regex = "src:'(.+)'";

		String url = CommonUtil.getRegexValueOnce(content, regex, 1);
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
		Getter cnetDownloader = new CnetGetter(url);
		cnetDownloader.get();
	}
}
