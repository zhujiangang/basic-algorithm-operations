package com.bao.lc.client.params;

import org.apache.http.client.RedirectStrategy;
import org.apache.http.params.HttpParams;

import com.bao.lc.client.impl.PostRedirectStrategy;

public class MiscParams
{
	public static String getReferer(final HttpParams params)
	{
		return (String)params.getParameter(MiscPNames.REQUEST_REFERER);
	}

	public static void setReferer(final HttpParams params, String referer)
	{
		params.setParameter(MiscPNames.REQUEST_REFERER, referer);
	}

	public static PostRedirectStrategy getPostRedirectStrategy(final HttpParams params)
	{
		RedirectStrategy rs = getRedirectStrategy(params);
		if(rs instanceof PostRedirectStrategy)
		{
			return (PostRedirectStrategy)rs;
		}
		return null;
	}

	public static RedirectStrategy getRedirectStrategy(final HttpParams params)
	{
		RedirectStrategy rs = (RedirectStrategy)params.getParameter(MiscPNames.REDIRECT_STRATEGY);
		return rs;
	}

	public static void setRedirectStrategy(final HttpParams params,
		RedirectStrategy redirectStrategy)
	{
		params.setParameter(MiscPNames.REDIRECT_STRATEGY, redirectStrategy);
	}
}
