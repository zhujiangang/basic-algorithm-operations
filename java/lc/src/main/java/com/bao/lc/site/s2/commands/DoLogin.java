package com.bao.lc.site.s2.commands;

import java.net.URI;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.utils.URIUtils;
import org.htmlparser.Parser;
import org.htmlparser.tags.LinkTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.lc.AppConfig;
import com.bao.lc.client.PostRedirectStrategy;
import com.bao.lc.common.IDValuePair;
import com.bao.lc.common.LinkTextRegexFilter;
import com.bao.lc.common.ResultCode;
import com.bao.lc.httpcommand.DefaultHttpCommand;
import com.bao.lc.httpcommand.HttpCommandPNames;
import com.bao.lc.httpcommand.HttpCommandParams;
import com.bao.lc.site.s2.ZyContants;
import com.bao.lc.util.CommonUtil;
import com.bao.lc.util.HttpClientUtil;

public class DoLogin extends DefaultHttpCommand
{
	private static Log log = LogFactory.getLog(DoLogin.class);

	public DoLogin()
	{
		super();
	}

	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		
		String encoding = "UTF-8";
		// save the member center page
		String userPage = HttpClientUtil.saveToString(rsp.getEntity(), encoding);
		context.put(ZyContants.USER_PAGE_CONTENT_KEY, userPage);
		
		PostRedirectStrategy redirectStrategy = HttpCommandParams.getRedirectStrategy(context);
		if(redirectStrategy == null)
		{
			throw new IllegalStateException("Can't find the redirectStrategy.");
		}
		if(!redirectStrategy.isRedirected())
		{
			throw new IllegalStateException("Login doesn't find any redirect.");
		}

		HttpUriRequest finalRequest = redirectStrategy.getFinalRequest();
		context.put(ZyContants.USER_PAGE_URI, finalRequest.getURI().toString());

		// set login state
		context.put(ZyContants.LOGIN_STATE_KEY, Boolean.TRUE);
		
		//Parse logout URI request
		parseLogoutRequest(context, userPage, finalRequest.getURI(), encoding);
		
		//No next hop
		context.remove(HttpCommandPNames.TARGET_REQUEST);
		context.remove(HttpCommandPNames.TARGET_REFERER);
		
		String user = MapUtils.getString(context, ZyContants.PARAM_USER);
		log.info("User [" + user + "] login successfully.");

		return ResultCode.RC_OK;
	}

	private void parseLogoutRequest(Context context, String userPage, URI userPageURI,
		String encoding) throws ParserException
	{
		Parser parser = CommonUtil.createParser(userPage, encoding, log);

		NodeList logoutList = parser.parse(new LinkTextRegexFilter(AppConfig.getInstance()
			.getPropInternal("zy.logout.link.mark")));
		if(logoutList.size() <= 0)
		{
			throw new ParserException("No logout link found!");
		}

		if(logoutList.size() > 1)
		{
			log.warn("Found more than 1 logout link. size = " + logoutList.size());
		}
		LinkTag logoutLink = (LinkTag) logoutList.elementAt(0);

		String link = logoutLink.getLink();
		URI logoutURI = URIUtils.resolve(userPageURI, link);
		HttpGet get = new HttpGet(logoutURI);

		context.put(ZyContants.LOGOUT_URI_REQUEST, get);
	}
}
