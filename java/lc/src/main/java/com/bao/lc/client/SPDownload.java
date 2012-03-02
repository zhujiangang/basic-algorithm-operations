package com.bao.lc.client;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.util.EntityUtils;

import com.bao.lc.AppConfig;
import com.bao.lc.util.CommonUtil;

public class SPDownload
{
	private static Log log = LogFactory.getLog(SPDownload.class);
	
	private String url;
	private String referer;
	
	public SPDownload(String url, String referer)
	{
		this.url = url;
		this.referer = referer;
	}
	
	public int download()
	{
		int rc = 0;
		
		BrowserClient session = new BrowserClient();
		session.addReferer(referer);
		
		HttpResponse rsp = null;
		try
		{
			rsp = session.get(url);
			
			EntityUtils.consume(rsp.getEntity());
			
			if(rsp.getStatusLine().getStatusCode() != 200)
			{
				return -1;
			}
		}
		catch(Exception e)
		{
			log.error("error happened.", e);
			return -2;
		}
		finally
		{
			session.getConnectionManager().shutdown();
		}
		
		return rc;
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		String url = AppConfig.getInstance().getPropInput("sp.dl.url");
		String referer = AppConfig.getInstance().getPropInput("sp.dl.referer");
		int count = CommonUtil.toInt(AppConfig.getInstance().getPropInput("sp.dl.count"));
		
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
			else if(args[i].startsWith("-referer="))
			{
				value = args[i].substring("-referer=".length());
				referer = value;
			}
		}
		log.info(String.format("url=%s, referer=%s, count=%d", url, referer, count));
		
		int fail = 0;
		for(int i = 0; i < count; i++)
		{
			SPDownload sp = new SPDownload(url, referer);
			if( sp.download() != 0)
			{
				fail++;
			}
			
			long sleepTime = (long)(Math.random() * 100);
			sleepTime *= 1000;
			
			try
			{
				Thread.sleep(sleepTime);
			}
			catch(Exception e)
			{
				log.error("sleep interrupted.", e);
			}
		}
		
		String result = String.format(
			"Result of grab:\n\turl=[%s]\n\tcount=[%d]\n\n\tOK=[%02d], Fail=[%02d]\n", url, count,
			count - fail, fail);
		log.info(result);
	}

}
