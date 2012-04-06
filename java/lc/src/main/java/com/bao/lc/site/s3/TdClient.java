package com.bao.lc.site.s3;

import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.List;

import javax.net.ssl.KeyManager;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ChainBase;
import org.apache.commons.chain.impl.ContextBase;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
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
import com.bao.lc.site.s3.bean.PassengerInfo;
import com.bao.lc.site.s3.commands.BookTicket;
import com.bao.lc.site.s3.commands.DoLogout;
import com.bao.lc.site.s3.commands.GetTicketBookInitPage;
import com.bao.lc.site.s3.commands.Login;
import com.bao.lc.site.s3.commands.WelComePage;
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
	
	private List<PassengerInfo> getPassengerList(int passengerCount)
	{
		List<PassengerInfo> passengerList = new ArrayList<PassengerInfo>(passengerCount);
		for(int i = 0; i < passengerCount; i++)
		{
			String content = AppConfig.getInstance().getPropInput("td.passenger" + (i + 1));
			String[] fields = content.split(",");
			if(fields.length < 6)
			{
				log.fatal("passenger info is incomplete. content=" + content);
				System.exit(-2);
			}
			PassengerInfo passenger = new PassengerInfo();
			passenger.name = fields[0];
			passenger.cardType = TdUtils.getCardTypeValue(fields[1]);
			passenger.cardNo = fields[2];
			passenger.tiketType = TdUtils.getTicketTypeValue(fields[3]);
			passenger.phone = fields[4];
			passenger.isSave = fields[5];
			
			passengerList.add(passenger);
		}
		
		return passengerList;
	}
	
	private Context createContext()
	{		
		HttpContext httpContext = new BasicHttpContext();
//		String loginUrl = AppConfig.getInstance().getPropInternal("td.login.url");
//		String refererUrl = AppConfig.getInstance().getPropInternal("td.login.referer");
//		Context context = HttpCommandUtils.createContext(session, httpContext, new HttpGet(loginUrl));
//		context.put(HttpCommandPNames.TARGET_REFERER, refererUrl);
		Context context = new ContextBase();
		context.put(HttpCommandPNames.HTTP_CLIENT, session);
		context.put(HttpCommandPNames.HTTP_CONTEXT, httpContext);

		// Input Parameters
		context.put(HttpCommandPNames.RESPONSE_DEFAULT_CHARSET, "UTF-8");
		context.put(TdPNames._USER_INTERFACE, LogFactory.getLog("TdClient.UI"));
		
		String user = AppConfig.getInstance().getPropInput("td.user");
		String pwd = AppConfig.getInstance().getPropInput("td.password");
		String fromStation = AppConfig.getInstance().getPropInput("td.from_station");
		String toStation = AppConfig.getInstance().getPropInput("td.to_station");
		String ticketDate = AppConfig.getInstance().getPropInput("td.ticket.date");
		String ticketTimeRange = AppConfig.getInstance().getPropInput("td.ticket.time_range");
		Integer passengerCount = MiscUtils.toInt(AppConfig.getInstance().getPropInput("td.user.count"));
		
		context.put(TdPNames.PARAM_USER, user);
		context.put(TdPNames.PARAM_PASSWORD, pwd);
		context.put(TdPNames.PARAM_FROM_STATION, fromStation);
		context.put(TdPNames.PARAM_TO_STATION, toStation);
		context.put(TdPNames.PARAM_TICKET_DATE, ticketDate);
		context.put(TdPNames.PARAM_TICKET_TIME_RANGE, ticketTimeRange);
		context.put(TdPNames.PARAM_PASSENGER_COUNT, passengerCount);
		context.put(TdPNames.PARAM_PASSENGER_LIST, getPassengerList(passengerCount));
		
		// Internal parameters
		//Welcome page
		String welComeURL = AppConfig.getInstance().getPropInternal("td.welcome.url");
		String welComeReferer = AppConfig.getInstance().getPropInternal("td.welcome.referer");
		context.put(TdPNames.PARAM_WELCOME_URL, welComeURL);
		context.put(TdPNames.PARAM_WELCOME_REFERER, welComeReferer);
		
		//Login page
		String loginURL = AppConfig.getInstance().getPropInternal("td.login.url");
		String loginReferer = AppConfig.getInstance().getPropInternal("td.login.referer");
		context.put(TdPNames.PARAM_LOGIN_URL, loginURL);
		context.put(TdPNames.PARAM_LOGIN_REFERER, loginReferer);
				
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
		
		//confirm passenger init page
		String confirmPassengerURL = AppConfig.getInstance().getPropInternal("td.confirmPassenger.init.url");
		String confirmPassengerReferer = AppConfig.getInstance().getPropInternal("td.confirmPassenger.init.referer");
		context.put(TdPNames.PARAM_CONFIRM_PASSENGER_INIT_URL, confirmPassengerURL);
		context.put(TdPNames.PARAM_CONFIRM_PASSENGER_INIT_REFERER, confirmPassengerReferer);
		
		return context;
	}
	
	private Chain createChain1()
	{
		// 2. Init Command chain
		Chain chain = new ChainBase();
		chain.addCommand(new WelComePage());
		chain.addCommand(new Login());
		chain.addCommand(new GetTicketBookInitPage());
		chain.addCommand(new BookTicket());
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
