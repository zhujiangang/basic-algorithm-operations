package com.bao.lc.client;

import java.io.FileInputStream;

import org.apache.commons.io.IOUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.params.HttpProtocolParams;
import org.apache.http.util.EntityUtils;

import com.bao.lc.AppConfig;
import com.bao.lc.common.exception.ParseException;
import com.bao.lc.util.AppUtils;
import com.bao.lc.util.CommonUtil;
import com.bao.lc.util.HttpClientUtil;

public class CnetDownload
{
	private static Log log = LogFactory.getLog(CnetDownload.class);

	private String url;
	private BrowserClient session;

	public CnetDownload(String url)
	{
		this.url = url;
		
		session = new BrowserClient();
		HttpProtocolParams.setUserAgent(session.getParams(), BrowserClient.AGENT_CHROME);
	}

	public int download()
	{
		int rc = 0;
		
		HttpResponse rsp = null;
		String content = null;

		String nextUrl = null;
		try
		{
			// 1. Get url user page content
			rsp = session.get(url);
			content = EntityUtils.toString(rsp.getEntity(), "UTF-8");
			
			nextUrl = extractDirectLink(content);

			// 2. Get direct download link page
			rsp = session.get(nextUrl);
			content = EntityUtils.toString(rsp.getEntity(), "UTF-8");
			nextUrl = extraceSrcFileURL(content);
			log.info(nextUrl);

			// 3. Download file
			rsp = session.get(nextUrl);
			String fileName = extraceFileName(nextUrl);
			HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getOutputFilePath(fileName));
		}
		catch(Exception e)
		{
			log.error(e.toString(), e);
			
			rc = -1;
		}
		finally
		{
			session.getConnectionManager().shutdown();
		}
		
		return rc;
	}

	private String extractDirectLink(String content) throws ParseException
	{
		String regex = "<div class=\"dlLinkWrapper\"> <a href=\"(.+?)\" id=(.+?)>Direct Download Link</a>";
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
	
	public static void grab(String url, int count, int interval)
	{
		int nSucc = 0, nFail = 0;

		int rc = 0;
		for(int i = 0; i < count; i++)
		{
			CnetDownload grabber = new CnetDownload(url);
			rc = grabber.download();

			if(rc == 0)
			{
				nSucc++;
			}
			else
			{
				nFail++;
			}
			
			//Sleep if needed
			if(i < (count - 1) && interval != 0)
			{
				long sleepTime = 0;
				if(interval < 0)
				{
					sleepTime = (int)(Math.random() * 1000);
					sleepTime *= 1000;
				}
				else
				{
					sleepTime = interval * 1000;
				}
				
				try
				{
					Thread.sleep(sleepTime);
				}
				catch(Exception e)
				{
					//ignore
				}
			}
		}

		String result = String.format(
				"Result of grab:\n\turl=[%s]\n\tcount=[%d]\n\tinterval=[%d]\n\n\tOK=[%02d], Fail=[%02d]\n",
				url, count, interval, nSucc, nFail);
		log.info(result);
	}
	
	@SuppressWarnings("unused")
	private void unitTest()
	{
		try
		{
			String content = IOUtils.toString(new FileInputStream("cnet.html"), "UTF-8");
			String url = extractDirectLink(content);
			log.info(url);
		}
		catch(Exception e)
		{
			log.error(e.toString(), e);
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		String url = AppConfig.getInstance().getPropInput("cnet.dl.url");
		int count = CommonUtil.toInt(AppConfig.getInstance().getPropInput("cnet.dl.count"));
		int interval = CommonUtil.toInt(AppConfig.getInstance().getPropInput("cnet.dl.interval"));
		
		String value = null;
		for(int i = 0; args != null && i < args.length; i++)
		{
			if(args[i].startsWith("-url="))
			{
				value = args[i].substring("-url=".length());
				url = value;
			}
			else if(args[i].startsWith("-count="))
			{
				value = args[i].substring("-count=".length());
				count = Integer.parseInt(value);
			}
			else if(args[i].startsWith("-interval="))
			{
				value = args[i].substring("-interval=".length());
				interval = Integer.parseInt(value);
			}
		}
		log.info(String.format("url=%s, count=%d, interval=%d", url, count, interval));
		
		grab(url, count, interval);
	}

}
