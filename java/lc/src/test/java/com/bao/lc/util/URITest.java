package com.bao.lc.util;

import java.net.URI;

import org.apache.http.HttpHost;
import org.apache.http.client.utils.URIUtils;

import junit.framework.TestCase;

public class URITest extends TestCase
{
	private static String[][] datas = {
		{"www.example.com", "http://www.example.com/"},
		{"www.example.com/", "http://www.example.com/"},
		{"/abc.html", "http://www.example.com/"},
		{"/abc", "http://www.example.com/"},
		{"abc/123.html", "http://www.example.com/"},
		{"abc/123.html/", "http://www.example.com/"},
		{"abc.html", "http://www.example.com/"},
		{"abc", "http://www.example.com/"},
		{"http://www.example.com", "http://www.example.com/"},
		{"http://www.example.com/", "http://www.example.com/"},
		{"www.example.com/help/abc.html", "http://www.example.com/"},
	};

	public void testURI()
	{
		HttpHost[] hosts = new HttpHost[2];
		hosts[0] = new HttpHost("www.example.com");
		hosts[1] = null;

		for(int j = 0; j < hosts.length; j++)
		{
			for(int i = 0; i < datas.length; i++)
			{
				try
				{
					toURI(datas[i][0], hosts[j]);
				}
				catch(Exception e)
				{
					e.printStackTrace();
				}
			}
		}
	}
	
	private URI toURI(String url, HttpHost host) throws Exception
	{
		URI uriOld = new URI(url);

		URI uriNew = URIUtils.rewriteURI(uriOld, host);

		System.out.println(String.format("Old=[%s]:[%b], New=[%s]:[%b]", uriOld,
			uriOld.isAbsolute(), uriNew, uriNew.isAbsolute()));

		return uriNew;
	}
}
