package com.bao.lc.client;

import java.io.FileInputStream;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import net.sf.json.JSONObject;
import net.sf.json.JSONSerializer;

import org.apache.commons.io.IOUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.Header;
import org.apache.http.HttpHost;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.utils.URIUtils;
import org.apache.http.cookie.Cookie;
import org.apache.http.impl.cookie.BasicClientCookie;
import org.apache.http.params.HttpProtocolParams;
import org.apache.http.util.EntityUtils;

import com.bao.lc.AppConfig;
import com.bao.lc.common.exception.ParseException;
import com.bao.lc.util.AppUtils;
import com.bao.lc.util.CommonUtil;
import com.bao.lc.util.HttpClientUtil;

public class CnetUser
{
	private static Log log = LogFactory.getLog(CnetUser.class);

	private BrowserClient session = null;
	private boolean isLogin = false;

	private String user = null;
	private String password = null;
	
	private String profileContent = null;
	private HttpHost host = null;

	public CnetUser(String user, String password)
	{
		session = new BrowserClient();
		HttpProtocolParams.setUserAgent(session.getParams(), BrowserClient.AGENT_CHROME);

		isLogin = false;
		this.user = user;
		this.password = password;
		profileContent = null;
		
		getDefaultHost();
	}
	
	private void getDefaultHost()
	{
		String url = AppConfig.getInstance().getPropInternal("cnet.url.main");
		try
		{
			URI uri = new URI(url);
			host = URIUtils.extractHost(uri);
		}
		catch(URISyntaxException e)
		{
			log.error(String.format("Failed to convert url=[%s] to URI", url), e);
		}
	}
	
	public int get()
	{
		int rc = 0;
		
		try
		{
			do
			{
				if(!login())
				{
					rc = -1;
					break;
				}
				
				getComminityProfile();
				
				modifyProfile("John", "Smith", "MALE", "01/01/1980");
			}
			while(false);
		}
		catch(Exception e)
		{
			log.error(e.toString(), e);
		}
		finally
		{
			if(isLogin)
			{
				try
				{
					logout();
				}
				catch(Exception e2)
				{
					log.error(e2.toString(), e2);
				}
			}
			
			session.getConnectionManager().shutdown();
		}
		
		return rc;
	}

	public boolean login() throws ClientProtocolException, IOException,
		ParseException
	{
		isLogin = false;
		
		HttpResponse rsp = null;
		String url = null;
		String content = null;

		// 1. main page for referer
		url = AppConfig.getInstance().getPropInternal("cnet.url.main");
		rsp = session.get(url);

		// 2. login page
		url = AppConfig.getInstance().getPropInternal("cnet.url.login");
		
		Map<String, String> params = new HashMap<String, String>();
		params.put("appId", "223");
		params.put("email", user);
		params.put("password", password);
		params.put("rememberMe", "true");
		params.put("resource", "rps-authenticate");
		params.put("viewType", "json");
		
		rsp = session.post(url, params, "UTF-8");

		// parse result
		content = EntityUtils.toString(rsp.getEntity(), "UTF-8");
		
		Map<String, String> resultMap = new HashMap<String, String>();
		String errorMessage = parseLoginResult(content, resultMap);
		if(errorMessage != null)
		{
			log.info(String.format("User [%s] login failed: %s.", user, errorMessage));
			return false;
		}
		updateCookie(resultMap);
		
		//3. try to get user profile page to check if we are really successfully login
		url = AppConfig.getInstance().getPropInternal("cnet.url.main_profile");
		
		params.clear();
		params.put("tag", "hdr;brandnav");
		rsp = session.get(url, null, null, host);
		HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("profile.html"));
		
		profileContent = IOUtils.toString(
			new FileInputStream(AppUtils.getTempFilePath("profile.html")), "UTF-8");
		
		String userName = getLoginUserName();
		isLogin = (userName != null);
		
		if(!isLogin)
		{
			log.info(String.format("User [%s] login failed.", user));
			return false;
		}
		
		log.info(String.format("User [%s] login succeeded. userName=[%s].", user, userName));
		return isLogin;
	}
	
	public boolean logout() throws ClientProtocolException, IOException, ParseException
	{
		String logoutPageRegex = "<a class=\"logOut\" href=\"(.+?)\">Log out</a>";
		String logoutPage = CommonUtil.getRegexValueOnce(profileContent, logoutPageRegex, 1);

		HttpResponse rsp = session.get(logoutPage, null, null, host);
		HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("logout.html"));
		
		boolean result = (rsp.getStatusLine().getStatusCode() == 200);
		if(result)
		{
			isLogin = false;
		}
		
		log.info(String.format("User [%s] logout %s.", user, (result ? "succeeded" : "failed")));
		return result;
	}
	
	public boolean getComminityProfile() throws ClientProtocolException, IOException, ParseException
	{
		String regex = "<a class=\"itemName\" href=\"(.+?)\">Community Profile</a>";
		String url = CommonUtil.getRegexValueOnce(profileContent, regex, 1);
		
		Map<String, String> params = new HashMap<String, String>(1);
		params.put("tag", "contentMain;contentBody");
		HttpResponse rsp = session.get(url, null, params, host);
		HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("community-profile.html"));
		
		return (rsp.getStatusLine().getStatusCode() == 200);
	}
	
	public boolean modifyProfile(String firstName, String lastName, String gender, String birthday)
		throws ClientProtocolException, IOException, ParseException
	{		
		//1. First get update url
		String regex = "<ul class=\"settings\"> <li><a href=\"(.+?)\">Update my e-mail address</a></li> <li>";
		String url = CommonUtil.getRegexValueOnce(profileContent, regex, 1);
		
		Map<String, String> params = new HashMap<String, String>();
		HttpResponse rsp = session.get(url, null, params, host);
		HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("modifyProfile.html"));
		
		String content = IOUtils.toString(
			new FileInputStream(AppUtils.getTempFilePath("modifyProfile.html")), "UTF-8");
		regex = "<input type=\"hidden\" name=\"path\" value=\"(.+?)\"(.*?)/>";
		String path = CommonUtil.getRegexValueOnce(content, regex, 1);
		
		regex = "<form onSubmit=\"(.+?)\" name=\"mainForm\" method=\"POST\" action=\"(.+?)\">";
		url = CommonUtil.getRegexValueOnce(content, regex, 2);
		
		//2. Post modify request
		params.clear();
		params.put("COUNTRY", "US");
		params.put("DATEOFBIRTH", birthday);
		params.put("EMAILADDR", user);
		params.put("FIRSTNAME", firstName);
		params.put("LASTNAME", lastName);
		params.put("GENDER", gender);
		params.put("metaparameter_checkboxes", "REMEMBERME");
		params.put("POSTALCODE", "92131");
		params.put("REMEMBERME", "1");
		params.put("path", path);
		
		rsp = session.post(url, null, params, "UTF-8", host);
		HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("modifyProfileResult.html"));
		
		String location = null;
		if(rsp.getStatusLine().getStatusCode() == 302)
		{
			Header locHeader = rsp.getLastHeader("Location");
			if(locHeader != null)
			{
				location = locHeader.getValue();
			}
		}
		log.info(String.format("Modify profile result: StatusCode=[%d], Location=[%s]", rsp
			.getStatusLine().getStatusCode(), location));
		
		if(location != null)
		{
			rsp = session.get(location, null, null, host);
			HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("modifyProfileRedir.html"));
		}

		return (rsp.getStatusLine().getStatusCode() == 302) && (location != null);
	}
	
	private String getLoginUserName() throws IOException, ParseException
	{
		String userNameRegex = "userName: '(.*?)',";
		String userName = CommonUtil.getRegexValueOnce(profileContent, userNameRegex, 1);
		if(userName.trim().isEmpty())
		{
			log.info("userName is empty");
			return null;
		}
		
		String loggedInRegex = "loggedIn: '(.*?)',";
		String loggedIn = CommonUtil.getRegexValueOnce(profileContent, loggedInRegex, 1);
		if(loggedIn.trim().isEmpty())
		{
			log.info("loggedIn is empty");
			return null;
		}
		
		if(!"1".equals(loggedIn.trim()))
		{
			log.info("invalid loggedIn: [" + loggedIn + "].");
			return null;
		}
		
		return userName.trim();
	}

	private void updateCookie(Map<String, String> resultMap)
	{
		String[] names = { "urs_sessionId", "purs_1", "surs_1", "curs_gigya_linked",
			"curs_gigya_appid", "curs_fb_linked" };

		for(int i = 0; i < names.length; i++)
		{
			updateCookie(names[i], resultMap);
			resultMap.remove(names[i]);
		}

		String key = null;
		for(Iterator<String> iter = resultMap.keySet().iterator(); iter.hasNext();)
		{
			key = iter.next();

			if(key.contains("_tt_machines"))
			{
				break;
			}
		}

		if(key != null)
		{
			updateCookie(key, resultMap);
			resultMap.remove(key);
		}
	}

	private void updateCookie(String cookieName, Map<String, String> resultMap)
	{
		Cookie cookie = createCookie(cookieName, resultMap.get(cookieName));
		session.getCookieStore().addCookie(cookie);
	}

	private BasicClientCookie createCookie(String name, String value)
	{
		BasicClientCookie cookie = new BasicClientCookie(name, value);
		cookie.setVersion(0);
		cookie.setDomain(".cnet.com");
		cookie.setPath("/");
		cookie.setExpiryDate(null);
		return cookie;
	}

	private String parseLoginResult(String jsonText, Map<String, String> resultMap)
	{		
		JSONObject root = (JSONObject) JSONSerializer.toJSON(jsonText);
		JSONObject rsp = root.getJSONObject("RpsResponse");
		JSONObject status = rsp.getJSONObject("Status");

		String state = status.getString("@state");
		// Failed
		if("failed".equals(state))
		{
			return status.getJSONObject("Error").getJSONObject("ErrorMessage").getString("$");
		}

		JSONObject user = rsp.getJSONObject("User");
		// Successfully
		if(!"processed".equals(state) || user == null)
		{
			return "Unkown state: " + state + ", user = " + user;
		}

		status = user.getJSONObject("Status");
		int code = status.getInt("@code");
		String codeStr = status.getString("$");
		if(code != 0 || !"Authenticated".equals(codeStr))
		{
			return "Unkown user status code: " + code + ", codeStr = " + codeStr;
		}

		String name, value;
		// purs_1
		JSONObject permToken = user.getJSONObject("PermToken");
		name = permToken.getString("@name");
		value = permToken.getString("$");

		resultMap.put(name, value);

		// surs_1
		JSONObject sessToken = user.getJSONObject("SessToken");
		name = sessToken.getString("@name");
		value = sessToken.getString("$");

		resultMap.put(name, value);

		// urs_sessionId
		JSONObject sessionId = user.getJSONObject("SessionId");
		resultMap.put("urs_sessionId", sessionId.getString("$"));

		// RegistrationId
		String regId = user.getJSONObject("RegistrationId").getString("$");
		resultMap.put(regId + "_tt_machines", "false");

		resultMap.put("WRUID", "0");
		resultMap.put("MAD_INTERNAL", "0");
		resultMap.put("curs_gigya_linked", "false");
		resultMap.put("curs_gigya_appid", "259");
		resultMap.put("curs_fb_linked", "false");
		resultMap.put("cnet_joinCallout", "1");
		resultMap.put("mad_rsi_segs", "ASK05540_10619&ASK05540_10648");
		
		return null;
	}
	
	public static void main(String[] args)
	{
		String user = AppConfig.getInstance().getPropInput("cnet.user");
		String password = AppConfig.getInstance().getPropInput("cnet.password");
		
		for(int i = 0; args != null && i < args.length; i++)
		{
			if(args[i].startsWith("-user="))
			{
				user = args[i].substring("-user=".length());
			}
			else if(args[i].startsWith("-password="))
			{
				password = args[i].substring("-password=".length());
			}
		}
		log.info(String.format("user=%s, password=%s", user, password));
		
		CnetUser cnet = new CnetUser(user, password);
		cnet.get();
	}
}
