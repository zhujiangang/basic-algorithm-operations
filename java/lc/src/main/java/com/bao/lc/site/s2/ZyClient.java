package com.bao.lc.site.s2;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ChainBase;
import org.apache.commons.collections.MapUtils;

import org.apache.commons.lang.BooleanUtils;
import org.apache.commons.lang.math.NumberUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.AppConfig;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.BrowserClient;
import com.bao.lc.httpcommand.CommandCompleteListener;
import com.bao.lc.httpcommand.HttpCommandDirector;
import com.bao.lc.httpcommand.RetryHttpCommand;
import com.bao.lc.httpcommand.impl.DefaultCommandRetryStrategy;
import com.bao.lc.httpcommand.impl.DefaultHttpCommandDirector;
import com.bao.lc.httpcommand.impl.DirectorBuilder;
import com.bao.lc.httpcommand.impl.LogCompleteListener;
import com.bao.lc.httpcommand.utils.HttpCommandUtils;
import com.bao.lc.site.SleepCommand;
import com.bao.lc.site.s2.commands.DoBookTicket;
import com.bao.lc.site.s2.commands.DoLogin;
import com.bao.lc.site.s2.commands.DoLogout;
import com.bao.lc.site.s2.commands.GetLoginPage;
import com.bao.lc.site.s2.commands.GetTicketDetail;
import com.bao.lc.site.s2.commands.GetTimeDiff;
import com.bao.lc.site.s2.commands.QueryTicketNumResult;
import com.bao.lc.site.s2.commands.SelectTicketDate;
import com.bao.lc.util.MiscUtils;

public class ZyClient
{
	private static Log log = LogFactory.getLog(ZyClient.class);
	
	private static final DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS"); 

	private BrowserClient session = null;

	public ZyClient()
	{
		session = new BrowserClient();
	}

	public void execute(Chain chain, Context context)
	{
		// 3. Fire!
		CommandCompleteListener listener = new LogCompleteListener(log);

		DirectorBuilder builder = new DirectorBuilder();
		builder.mainCommand(chain).mainContext(context).mainListener(listener);
		builder.finalCommand(new DoLogout()).finalContext(context).finalListener(listener);

		try
		{
			builder.execute();
		}
		finally
		{
			// free context space
			context.clear();
			context = null;

			// shutdown connections
			session.getConnectionManager().shutdown();
		}
	}

	private Context createContext()
	{
		// Startup URI
		HttpGet request = new HttpGet(AppConfig.getInstance().getPropInternal("zy.login.url"));

		HttpContext httpContext = new BasicHttpContext();
		Context context = HttpCommandUtils.createContext(session, httpContext, request);

		// Parameters
		String user = AppConfig.getInstance().getPropInput("zy.user");
		String pwd = AppConfig.getInstance().getPropInput("zy.password");
		String doctor = AppConfig.getInstance().getPropInput("zy.doctor");
		String hos = AppConfig.getInstance().getPropInput("zy.hospital");
		String dayOfWeek = AppConfig.getInstance().getPropInput("zy.dayOfWeek");
		String dayFixed = AppConfig.getInstance().getPropInput("zy.day.fixed");
		String consumer = AppConfig.getInstance().getPropInput("zy.consumer.name");
		String timeDiff = AppConfig.getInstance().getPropInput("zy.time.diff");

		String msg = String.format(
			"User=[%s], pwd=[%s], hos=[%s], doctor=[%s], dayOfWeek=[%s], dayFixed=[%s], consumer=[%s], timeDiff=[%s]", user, pwd,
			hos, doctor, dayOfWeek, dayFixed, consumer, timeDiff);
		log.info(msg);
		
		Boolean bDayFixed = BooleanUtils.toBooleanObject(dayFixed);
		long lTimeDiff = NumberUtils.toLong(timeDiff);

		context.put(ZyConstants.PARAM_USER, user);
		context.put(ZyConstants.PARAM_PASSWORD, pwd);
		context.put(ZyConstants.PARAM_DOCTOR, doctor);
		context.put(ZyConstants.PARAM_HOSPITAL, hos);
		context.put(ZyConstants.PARAM_DAY_OF_WEEK, dayOfWeek);
		context.put(ZyConstants.PARAM_FIXED_DAY, bDayFixed);
		context.put(ZyConstants.PARAM_CONSUMER_NAME, consumer);
		context.put(ZyConstants.PARAM_TIME_DIFF, Long.valueOf(lTimeDiff));
		
		lTimeDiff = getTimeDiff();
		if(lTimeDiff != 0)
		{
			context.put(ZyConstants.PARAM_TIME_DIFF, Long.valueOf(lTimeDiff));
		}
		
		//other non-input parameters
		context.put(ZyConstants.PARAM_RSP_ENCODING, "UTF-8");
		context.put(ZyConstants.PARAM_TARGET_DAY, getTargetDay(context));
		
		return context;
	}

	private Chain createChain1()
	{
		// 2. Init Command chain
		Chain chain = new ChainBase();
		chain.addCommand(new GetLoginPage());
		chain.addCommand(new DoLogin());
		chain.addCommand(new QueryTicketNumResult());
		chain.addCommand(new GetTicketDetail());
		chain.addCommand(new SelectTicketDate());
		chain.addCommand(new DoBookTicket());

		return chain;
	}

	@SuppressWarnings("unchecked")
	public void action1()
	{
		// 1. Init Command context
		Context context = createContext();

		// 2. Init Command chain
		Chain chain = createChain1();
		
		// 3. Execute
		execute(chain, context);
	}
	private Calendar getTargetDay(Context context)
	{
		Calendar now = Calendar.getInstance();
		
		String dayOfWeek = MapUtils.getString(context, ZyConstants.PARAM_DAY_OF_WEEK, "6");
		int iDayOfWeek = MiscUtils.toInt(String.valueOf(dayOfWeek.charAt(0)));
		MiscUtils.updateCalendar(now, 3, iDayOfWeek);
		now.set(Calendar.HOUR_OF_DAY, 18);
		now.set(Calendar.MINUTE, 0);
		now.set(Calendar.SECOND, 0);
		now.set(Calendar.MILLISECOND, 0);
		
		long lTimeDiff = MapUtils.getLongValue(context, ZyConstants.PARAM_TIME_DIFF, 0);
		now.add(Calendar.MILLISECOND, (int)(-lTimeDiff));
		
		return now;
	}
	private Calendar getSiteStartTime(Context context)
	{
		Calendar target = getTargetDay(context);
		target.add(Calendar.DAY_OF_MONTH, -15);
		
		//DEBUG
//		target.set(Calendar.MINUTE, 33);
		
		return target;
	}
	
	private long getTimeDiff()
	{
		// Startup URI
		HttpGet request = new HttpGet(AppConfig.getInstance().getPropInternal("zy.systime.url"));
		Context context = HttpCommandUtils.createContext(session, request);
		
		Chain chain = new ChainBase();
		chain.addCommand(new GetTimeDiff());
		
		HttpCommandDirector director = new DefaultHttpCommandDirector();
		director.execute(chain, context, new LogCompleteListener(log));
		
		Long diff = MapUtils.getLong(context, ZyConstants.PARAM_TIME_DIFF);
		if(diff != null)
		{
			return diff.longValue();
		}
		return 0L;
	}
	private Chain createChain2(Context context, String[] consumers)
	{
		// 2. Init Command chain
		Chain chain = new ChainBase();
		
		Calendar siteStartTime = getSiteStartTime(context);
		
		Calendar localStartTime = (Calendar)siteStartTime.clone();
		localStartTime.add(Calendar.SECOND, -2);
		
		StringBuilder sb = new StringBuilder();
		sb.append("Site Start Time: ").append(dateFormat.format(siteStartTime.getTime()));
		sb.append(", Local Start Time: ").append(dateFormat.format(localStartTime.getTime()));
		log.info(sb.toString());
		
		chain.addCommand(new SleepCommand(localStartTime.getTime()));
		chain.addCommand(new GetLoginPage());
		chain.addCommand(new DoLogin());
		
		for(int i = 0; i < consumers.length; i++)
		{
			String consumer = consumers[i].trim();
			
			chain.addCommand(new QueryTicketNumResult());
			chain.addCommand(new GetTicketDetail(consumer));

			DefaultCommandRetryStrategy retryStrategy = new ZyCommandRetryStrategy(3, siteStartTime.getTime());
			retryStrategy.addRetryResultCode(ResultCode.RC_DOCTOR_REG_LIST_FULL);
			Command selectDate = new RetryHttpCommand(new SelectTicketDate(), retryStrategy);
			chain.addCommand(selectDate);

			chain.addCommand(new DoBookTicket());
		}

		return chain;
	}

	public void action2()
	{
		// 1. Init Command context
		Context context = createContext();

		String consumerName = MapUtils.getString(context, ZyConstants.PARAM_CONSUMER_NAME);
		if(consumerName == null || consumerName.isEmpty())
		{
			throw new IllegalArgumentException("consumerName: " + consumerName);
		}
		String[] consumers = consumerName.split(",");
		
		// 2. Init Command chain
		Chain chain = createChain2(context, consumers);

		// 3. Execute
		execute(chain, context);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		ZyClient client = new ZyClient();
		client.action2();
	}
}
