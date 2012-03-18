package com.bao.lc.site.s2;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ChainBase;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.AppConfig;
import com.bao.lc.client.BrowserClient;
import com.bao.lc.httpcommand.CommandCompleteListener;
import com.bao.lc.httpcommand.impl.DirectorBuilder;
import com.bao.lc.httpcommand.impl.LogCompleteListener;
import com.bao.lc.httpcommand.utils.HttpCommandUtils;
import com.bao.lc.site.s2.commands.DoBookTicket;
import com.bao.lc.site.s2.commands.DoLogin;
import com.bao.lc.site.s2.commands.DoLogout;
import com.bao.lc.site.s2.commands.GetLoginPage;
import com.bao.lc.site.s2.commands.GetTicketDetail;
import com.bao.lc.site.s2.commands.QueryTicketNumResult;
import com.bao.lc.site.s2.commands.SelectTicketDate;

public class ZyClient
{
	private static Log log = LogFactory.getLog(ZyClient.class);

	private BrowserClient session = null;

	public ZyClient()
	{
		session = new BrowserClient();
	}

	@SuppressWarnings("unchecked")
	public void action1()
	{
		// 1. Init Command context

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
		String consumer = AppConfig.getInstance().getPropInput("zy.consumer.name");

		String msg = String.format(
			"User=[%s], pwd=[%s], hos=[%s], doctor=[%s], dayOfWeek=[%s], consumer=[%s]", user, pwd,
			hos, doctor, dayOfWeek, consumer);
		log.info(msg);

		context.put(ZyContants.PARAM_USER, user);
		context.put(ZyContants.PARAM_PASSWORD, pwd);
		context.put(ZyContants.PARAM_DOCTOR, doctor);
		context.put(ZyContants.PARAM_HOSPITAL, hos);
		context.put(ZyContants.PARAM_DAY_OF_WEEK, dayOfWeek);
		context.put(ZyContants.PARAM_CONSUMER_NAME, consumer);

		context.put(ZyContants.PARAM_RSP_ENCODING, "UTF-8");

		// 2. Init Command chain
		Chain chain = new ChainBase();
		chain.addCommand(new GetLoginPage());
		chain.addCommand(new DoLogin());
		chain.addCommand(new QueryTicketNumResult());
		chain.addCommand(new GetTicketDetail());
		chain.addCommand(new SelectTicketDate());
		chain.addCommand(new DoBookTicket());

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

	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		ZyClient client = new ZyClient();
		client.action1();
	}
}
