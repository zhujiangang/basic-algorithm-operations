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
import com.bao.lc.site.s3.commands.DoLogin;
import com.bao.lc.site.s3.commands.GetLoginPage;
import com.bao.lc.site.s3.commands.DoLogout;
import com.bao.lc.site.s3.params.TdPNames;

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
	
	public void execute(Command mainCommand, Context context)
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

		// Parameters
		context.put(TdPNames.PARAM_RSP_ENCODING, "UTF-8");
		context.put(TdPNames.PARAM_UI, log);
		
		String user = AppConfig.getInstance().getPropInput("td.user");
		String pwd = AppConfig.getInstance().getPropInput("td.password");
		
		
		context.put(TdPNames.PARAM_USER, user);
		context.put(TdPNames.PARAM_PASSWORD, pwd);
		
		return context;
	}
	
	private Chain createChain1()
	{
		// 2. Init Command chain
		Chain chain = new ChainBase();
		chain.addCommand(new GetLoginPage());
		chain.addCommand(new DoLogin());

		return chain;
	}
	
	public void action1()
	{
		Context context = createContext();
		Chain chain = createChain1();
		
		// 3. Execute
		execute(chain, context);
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
