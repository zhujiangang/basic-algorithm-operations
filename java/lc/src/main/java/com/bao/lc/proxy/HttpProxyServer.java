package com.bao.lc.proxy;

import net.grinder.TCPProxy;

public class HttpProxyServer extends Thread
{
	private String parameter = null;

	public HttpProxyServer(String parameter)
	{
		super("HttpProxyServer");
		this.parameter = parameter;
	}

	public void run()
	{
		String proxyParam = "-requestfilter " + RequestCookieFilter.class.getName() + " "
			+ "-responsefilter NONE";
		if(parameter != null && !parameter.isEmpty())
		{
			proxyParam += " " + parameter;
		}

		String[] args = proxyParam.split(" ");

		TCPProxy.main(args);
	}
}
