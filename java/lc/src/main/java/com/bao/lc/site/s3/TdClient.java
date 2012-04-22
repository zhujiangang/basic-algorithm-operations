package com.bao.lc.site.s3;

import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import javax.net.ssl.KeyManager;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ChainBase;
import org.apache.commons.chain.impl.ContextBase;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.client.params.ClientPNames;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.ssl.SSLSocketFactory;
import org.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.AppConfig;
import com.bao.lc.ResMgr;
import com.bao.lc.client.BrowserClient;
import com.bao.lc.httpcommand.CommandCompleteListener;
import com.bao.lc.httpcommand.impl.DefaultHttpCommandDirector;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.site.s3.bean.PassengerInfo;
import com.bao.lc.site.s3.bean.TicketFilterCondition;
import com.bao.lc.site.s3.commands.BookTicket;
import com.bao.lc.site.s3.commands.DoLogout;
import com.bao.lc.site.s3.commands.GetTicketBookInitPage;
import com.bao.lc.site.s3.commands.Login;
import com.bao.lc.site.s3.commands.WelComePage;
import com.bao.lc.site.s3.params.InputParameter;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.site.s3.params.TdParams;
import com.bao.lc.util.MiscUtils;

public class TdClient
{
	private static Log log = LogFactory.getLog(TdClient.class);

	private BrowserClient session = null;
	private Context clientContext = null;

	/**
	 * Default constructor
	 */
	public TdClient()
	{
		if(!initHttpClient())
		{
			throw new IllegalArgumentException("Failed to init http client");
		}
		
		clientContext = new ContextBase();
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
		BasicHttpParams params = new BasicHttpParams();
		params.setBooleanParameter(ClientPNames.ALLOW_CIRCULAR_REDIRECTS, Boolean.TRUE);
		
		session = new BrowserClient(tcm, params);

		return true;
	}
		
	private Context createContext(InputParameter parameter, Log uiLog)
	{		
		HttpContext httpContext = new BasicHttpContext();
		Context context = new ContextBase();
		context.put(HttpCommandPNames.HTTP_CLIENT, session);
		context.put(HttpCommandPNames.HTTP_CONTEXT, httpContext);

		// Input Parameters
		context.put(HttpCommandPNames.RESPONSE_DEFAULT_CHARSET, "UTF-8");
		
		if(uiLog == null)
		{
			uiLog = LogFactory.getLog("TdClient.UI");
		}
		context.put(TdPNames._USER_INTERFACE, uiLog);
		
		context.put(TdPNames.PARAM_USER, parameter.user);
		context.put(TdPNames.PARAM_PASSWORD, parameter.pwd);
		context.put(TdPNames.PARAM_FROM_STATION, parameter.fromStation);
		context.put(TdPNames.PARAM_TO_STATION, parameter.toStation);
		context.put(TdPNames.PARAM_TICKET_DATE, parameter.ticketDate);
		context.put(TdPNames.PARAM_PASSENGER_COUNT, parameter.passengers.size());
		context.put(TdPNames.PARAM_PASSENGER_LIST, parameter.passengers);
		context.put(TdPNames.PARAM_FILTER_CONDITION, parameter.filterCond);
		context.put(TdPNames.PARAM_TICKET_TIME_RANGE, parameter.ticketTimeRange);
		
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
	
	private boolean isContextInitialed()
	{
		if(clientContext != null)
		{
			Log uiLog = (Log)clientContext.get(TdPNames._USER_INTERFACE);
			return uiLog != null;
		}
		return false;
	}
	
	public boolean isLogin()
	{
		return TdParams.isLogin(clientContext);
	}
	
	public void initContext(InputParameter parameter, Log uiLog)
	{
		if(isContextInitialed())
		{
			if(isLogin())
			{
				log.error("Assert Failure: make sure the user has logout");
				logout();
			}
			clientContext.clear();
		}
		clientContext = createContext(parameter, uiLog);
	}
	
	public boolean login()
	{
		Chain chain = new ChainBase();
		chain.addCommand(new WelComePage());
		chain.addCommand(new Login());
		
		// 3. Execute
		TdParams.getUI(clientContext).info(ResMgr.getString("td.msg.start.login"));
		
		CommandCompleteListener listener = new TdWorkCompleteListener(TdParams.getUI(clientContext), log);
		DefaultHttpCommandDirector director = new DefaultHttpCommandDirector();
		try
		{
			director.execute(chain, clientContext, listener);
		}
		catch(Exception e)
		{
			TdParams.getUI(clientContext).error(e.getMessage(), e);
		}
		
		boolean isLogin = TdParams.isLogin(clientContext);
		
		return isLogin;
	}

	public void bookTicket()
	{
		// retry with newly input verification code
		clientContext.remove(TdPNames._CONFIRM_PASSENGER_VOCDE);
		
		Chain chain = new ChainBase();
		chain.addCommand(new GetTicketBookInitPage());
		chain.addCommand(new BookTicket());
		
		// 3. Execute
		TdParams.getUI(clientContext).info(ResMgr.getString("td.msg.start.book"));
		
		CommandCompleteListener listener = new TdWorkCompleteListener(TdParams.getUI(clientContext), log);
		DefaultHttpCommandDirector director = new DefaultHttpCommandDirector();
		try
		{
			director.execute(chain, clientContext, listener);
		}
		catch(Exception e)
		{
			TdParams.getUI(clientContext).error(e.getMessage(), e);
		}
	}
	
	public void logout()
	{		
		// 3. Execute
		TdParams.getUI(clientContext).info(ResMgr.getString("td.msg.start.logout"));
		
		CommandCompleteListener listener = new TdWorkCompleteListener(TdParams.getUI(clientContext), log);
		DefaultHttpCommandDirector director = new DefaultHttpCommandDirector();
		try
		{
			director.execute(new DoLogout(), clientContext, listener);
		}
		catch(Exception e)
		{
			TdParams.getUI(clientContext).error(e.getMessage(), e);
		}
	}
	
	public void shutdown()
	{
		// shutdown connections
		session.getConnectionManager().shutdown();
		
		clientContext.clear();
		clientContext = null;
	}
	
	private static List<PassengerInfo> getPassengerList(int passengerCount)
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
	
	private static TicketFilterCondition getFilterCond()
	{
		TicketFilterCondition cond = new TicketFilterCondition();
		
		String conditions = AppConfig.getInstance().getPropInput("td.ticket.sort.conditions");
		String[] args = conditions.split(",");

		for(String category : args)
		{
			String propName = "td.ticket.sort.cond." + category;
			String value = AppConfig.getInstance().getPropInput(propName);
			
			//Seat Class
			if("4".equalsIgnoreCase(category))
			{
				cond.seatClassList = Arrays.asList(value.split(","));
			}
			//Train Class
			else if("2".equalsIgnoreCase(category))
			{
				cond.trainClassList = Arrays.asList(value.split(","));
			}
		}
		
		return cond;
	}
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		String user = AppConfig.getInstance().getPropInput("td.user");
		String pwd = AppConfig.getInstance().getPropInput("td.password");
		String fromStation = AppConfig.getInstance().getPropInput("td.from_station");
		String toStation = AppConfig.getInstance().getPropInput("td.to_station");
		String ticketDate = AppConfig.getInstance().getPropInput("td.ticket.date");
		
		String ticketTimeRange = AppConfig.getInstance().getPropInput("td.ticket.time_range");
		Integer passengerCount = MiscUtils.toInt(AppConfig.getInstance().getPropInput("td.user.count"));
		
		InputParameter param = new InputParameter();
		param.user = user;
		param.pwd = pwd;
		param.fromStation = fromStation;
		param.toStation = toStation;
		param.ticketDate = ticketDate;
		param.passengers = getPassengerList(passengerCount);
		param.ticketTimeRange = ticketTimeRange;
		param.filterCond = getFilterCond();
		
		TdClient client = new TdClient();
		
		try
		{
			client.initContext(param, null);
			client.login();
			client.bookTicket();
		}
		finally
		{
			if(client.isLogin())
			{
				client.logout();
			}
			client.shutdown();
		}
		
		System.exit(0);
	}
}
