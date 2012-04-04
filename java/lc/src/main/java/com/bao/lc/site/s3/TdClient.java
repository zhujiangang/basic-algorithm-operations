package com.bao.lc.site.s3;

import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;

import javax.net.ssl.KeyManager;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ChainBase;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.ssl.SSLSocketFactory;
import org.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.AppConfig;
import com.bao.lc.client.BrowserClient;
import com.bao.lc.httpcommand.CommandCompleteListener;
import com.bao.lc.httpcommand.impl.DirectorBuilder;
import com.bao.lc.httpcommand.impl.LogCompleteListener;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.utils.HttpCommandUtils;
import com.bao.lc.site.s3.commands.DoLogout;
import com.bao.lc.site.s3.commands.GetTicketBookInitPage;
import com.bao.lc.site.s3.commands.Login;
import com.bao.lc.site.s3.commands.QueryLeftTicket;
import com.bao.lc.site.s3.commands.SubmitOrder;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.util.MiscUtils;

public class TdClient
{
	private static Log log = LogFactory.getLog(TdClient.class);

	private BrowserClient session = null;

	/**
	 * Default constructor
	 */
	public TdClient()
	{
		if(!initHttpClient())
		{
			System.exit(-1);
		}
	}

	/**
	 * Init HttpClient
	 * @return true if init successfully
	 */
	private boolean initHttpClient()
	{
		SSLContext sslContext = null;
		
		//1. Create SSLContext instance
		try
		{
			sslContext = SSLContext.getInstance("TLS");
		}
		catch(NoSuchAlgorithmException e)
		{
			log.error("SSLContext getInstance failed.", e);
			return false;
		}
		
		//2. Init
		try
		{
			TrustManager[] trustMgrs = new TrustManager[] { new TrustAllManager() };
			sslContext.init((KeyManager[]) null, trustMgrs, (SecureRandom) null);
		}		
		catch(KeyManagementException e)
		{
			log.error("SSLContext init failed.", e);
			return false;
		}

		//3. Create HTTPS scheme
		SSLSocketFactory ssf = new SSLSocketFactory(sslContext,
			SSLSocketFactory.ALLOW_ALL_HOSTNAME_VERIFIER);
		Scheme sch = new Scheme("https", 443, ssf);

		//4. Register with Connection Manager
		ThreadSafeClientConnManager tcm = new ThreadSafeClientConnManager();
		tcm.getSchemeRegistry().register(sch);
		
		//5. Create client
		session = new BrowserClient(tcm);

		return true;
	}
	
	public void execute(Command mainCommand, Context context) throws Exception
	{
		// 3. Fire!
		CommandCompleteListener listener = new LogCompleteListener(log);

		DirectorBuilder builder = new DirectorBuilder();
		builder.mainCommand(mainCommand).mainContext(context).mainListener(listener);
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
		String loginUrl = AppConfig.getInstance().getPropInternal("td.login.url");
		String refererUrl = AppConfig.getInstance().getPropInternal("td.login.referer");
		
		HttpContext httpContext = new BasicHttpContext();
		Context context = HttpCommandUtils.createContext(session, httpContext, new HttpGet(loginUrl));
		context.put(HttpCommandPNames.TARGET_REFERER, refererUrl);

		// Input Parameters
		context.put(HttpCommandPNames.RESPONSE_DEFAULT_CHARSET, "UTF-8");
		context.put(TdPNames._USER_INTERFACE, LogFactory.getLog("TdClient.UI"));
		
		String user = AppConfig.getInstance().getPropInput("td.user");
		String pwd = AppConfig.getInstance().getPropInput("td.password");
		String fromStation = AppConfig.getInstance().getPropInput("td.from_station");
		String toStation = AppConfig.getInstance().getPropInput("td.to_station");
		String ticketDate = AppConfig.getInstance().getPropInput("td.ticket.date");
		String ticketTimeRange = AppConfig.getInstance().getPropInput("td.ticket.time_range");
		Integer userCount = MiscUtils.toInt(AppConfig.getInstance().getPropInput("td.user.count"));
		
		context.put(TdPNames.PARAM_USER, user);
		context.put(TdPNames.PARAM_PASSWORD, pwd);
		context.put(TdPNames.PARAM_FROM_STATION, fromStation);
		context.put(TdPNames.PARAM_TO_STATION, toStation);
		context.put(TdPNames.PARAM_TICKET_DATE, ticketDate);
		context.put(TdPNames.PARAM_TICKET_TIME_RANGE, ticketTimeRange);
		context.put(TdPNames.PARAM_USER_COUNT, userCount);
		
		// Internal parameters
		//Init page
		String ticketBookInitURL = AppConfig.getInstance().getPropInternal("td.ticket_booking.init.url");
		String ticketBookInitReferer = AppConfig.getInstance().getPropInternal("td.ticket_booking.init.referer");
		context.put(TdPNames.PARAM_TICKET_BOOK_INIT_URL, ticketBookInitURL);
		context.put(TdPNames.PARAM_TICKET_BOOK_INIT_REFERER, ticketBookInitReferer);
		
		//queryLeftTicket page
		String queryLeftTicketBaseURL = AppConfig.getInstance().getPropInternal("td.queryLeftTicket.url");
		String queryLeftTicketReferer = AppConfig.getInstance().getPropInternal("td.queryLeftTicket.referer");
		context.put(TdPNames.PARAM_QUERY_LEFT_TICKET_BASE_URL, queryLeftTicketBaseURL);
		context.put(TdPNames.PARAM_QUERY_LEFT_TICKET_REFERER, queryLeftTicketReferer);

		//submit order page
		String submitOrderURL = AppConfig.getInstance().getPropInternal("td.submitOrder.url");
		String submitOrderReferer = AppConfig.getInstance().getPropInternal("td.submitOrder.referer");
		context.put(TdPNames.PARAM_SUBMIT_ORDER_URL, submitOrderURL);
		context.put(TdPNames.PARAM_SUBMIT_ORDER_REFERER, submitOrderReferer);
		
		//confirm passenger page
		String confirmPassengerURL = AppConfig.getInstance().getPropInternal("td.confirmPassenger.url");
		String confirmPassengerReferer = AppConfig.getInstance().getPropInternal("td.confirmPassenger.referer");
		context.put(TdPNames.PARAM_CONFIRM_PASSENGER_URL, confirmPassengerURL);
		context.put(TdPNames.PARAM_CONFIRM_PASSENGER_REFERER, confirmPassengerReferer);
		
		return context;
	}
	
	private Chain createChain1()
	{
		// 2. Init Command chain
		Chain chain = new ChainBase();
		chain.addCommand(new Login());
		chain.addCommand(new GetTicketBookInitPage());
		chain.addCommand(new QueryLeftTicket());
		chain.addCommand(new SubmitOrder());
		return chain;
	}
	
	public void action1()
	{
		Context context = createContext();
		Chain chain = createChain1();
		
		// 3. Execute
		try
		{
			execute(chain, context);
		}
		catch(Exception e)
		{
			log.error(e.getMessage(), e);
		}
	}
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		TdClient client = new TdClient();
		client.action1();
		
		System.exit(0);
	}
}
