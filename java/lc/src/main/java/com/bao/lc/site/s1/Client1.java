package com.bao.lc.site.s1;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ChainBase;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.BrowserClient;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.HttpCommandDirector;
import com.bao.lc.httpcommand.impl.DefaultHttpCommandDirector;
import com.bao.lc.httpcommand.impl.LogCompleteListener;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.httpcommand.utils.HttpCommandUtils;

public class Client1
{
	private static Log log = LogFactory.getLog(Client1.class);

	private BrowserClient session = null;

	public Client1()
	{
		session = new BrowserClient();
	}

	@SuppressWarnings("unchecked")
	public void action1()
	{
		// Init Command context
		HttpGet request = new HttpGet("http://www.downloads.com/");
		Context context = HttpCommandUtils.createContext(session, request);

		// Init Command chain
		Chain chain = new ChainBase();
		chain.addCommand(new SampleCommand());

		// Fire!
		try
		{
			HttpCommandDirector director = new DefaultHttpCommandDirector();

			director.execute(chain, context, new LogCompleteListener(log));
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
		Client1 client = new Client1();
		client.action1();
	}

}

class SampleCommand extends BasicHttpCommand
{
	protected IDValuePair postExecute(Context context) throws Exception
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);

		String content = HttpClientUtils.saveToString(rsp);
		System.out.println(content);

		context.remove(HttpCommandPNames.TARGET_REQUEST);
		context.remove(HttpCommandPNames.TARGET_REFERER);

		return ResultCode.RC_OK;
	}
}
