package com.bao.lc.client;

import java.util.Random;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.GnuParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
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
	
	@SuppressWarnings("static-access")
	private static Options buildOptions()
	{
		Options options = new Options();

		options.addOption("u", "url", true, "URL address");
		options.addOption("r", "referer", true, "referer");
		options.addOption("c", "count", true, "times to get");
		options.addOption("h", "help", false, "print this help message");
		options.addOption(OptionBuilder.withLongOpt("interval")
			.withDescription("how much time to rest between 2 actions").hasArg()
			.withArgName("time in seconds").create("i"));
		return options;
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		String url = AppConfig.getInstance().getPropInput("sp.dl.url");
		String referer = AppConfig.getInstance().getPropInput("sp.dl.referer");
		int count = CommonUtil.toInt(AppConfig.getInstance().getPropInput("sp.dl.count"));
		int interval = CommonUtil.toInt(AppConfig.getInstance().getPropInput("sp.dl.interval"));
		
		Options options = buildOptions();
		CommandLineParser parser = new GnuParser();
		try
		{
			// parse the command line arguments
			CommandLine line = parser.parse(options, args);
			
			if(line.hasOption('u'))
			{
				url = line.getOptionValue('u');
			}
			if(line.hasOption('r'))
			{
				referer = line.getOptionValue('r');
			}
			if(line.hasOption('c'))
			{
				count = CommonUtil.toInt(line.getOptionValue('c'));
			}
			if(line.hasOption('i'))
			{
				interval = CommonUtil.toInt(line.getOptionValue('i'));
			}
			if(line.hasOption('h'))
			{
				HelpFormatter formatter = new HelpFormatter();
				formatter.printHelp("SPDownload", options, true);
				
				System.exit(0);
			}
		}
		catch(Exception e)
		{
			System.err.println("ommand line options error:" + e.getMessage());
			
			HelpFormatter formatter = new HelpFormatter();
			formatter.printHelp("SPDownload", options, true);
			
			log.error("Command line options error.", e);
			System.exit(-1);
		}
		log.info(String.format("url=%s, referer=%s, count=%d, interval=%d", url, referer, count, interval));
		
		Random rand = new Random();
		int fail = 0;
		for(int i = 0; i < count; i++)
		{
			SPDownload sp = new SPDownload(url, referer);
			if( sp.download() != 0)
			{
				fail++;
			}
			
			CommonUtil.sleep(interval, rand);
		}
		
		String result = String.format(
			"Result of grab:\n\turl=[%s]\n\tcount=[%d]\n\n\tOK=[%02d], Fail=[%02d]\n", url, count,
			count - fail, fail);
		log.info(result);
	}

}
