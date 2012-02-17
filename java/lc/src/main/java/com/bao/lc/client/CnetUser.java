package com.bao.lc.client;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import net.sf.json.JSONObject;
import net.sf.json.JSONSerializer;

import org.apache.commons.io.IOUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.Header;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.cookie.Cookie;
import org.apache.http.impl.cookie.BasicClientCookie;
import org.apache.http.util.EntityUtils;

import com.bao.lc.util.HttpClientUtil;


public class CnetUser
{
	private static Log log = LogFactory.getLog(CnetUser.class);
	
	private HttpSession session = null;
	
	public CnetUser()
	{
		session = new HttpSession();
	}
	
	public void login(String user, String password)
	{
		try
		{
			prepareCookies();
			
			// 3.
			Map<String, String> headers = new HashMap<String, String>();
			headers.put("Referer", "http://www.cnet.com/");
			headers.put("Host", "www.cnet.com");

			Map<String, String> params = new HashMap<String, String>();
			params.put("appId", "223");
			params.put("email", user);
			params.put("password", password);
			params.put("rememberMe", "true");
			params.put("resource", "rps-authenticate");
			params.put("viewType", "json");
			HttpResponse rsp = session.post("http://www.cnet.com/8750-4_1-0.html", headers, params, "utf-8");

			String resultStr = EntityUtils.toString(rsp.getEntity(), "utf-8");
			log.info(resultStr);
			
			Map<String, String> resultMap = new HashMap<String, String>();
			if(parseLoginResult(resultStr, resultMap) != 0)
			{
				return;
			}
			
			updateCookie(resultMap);
			
			String loginCookie = toCookie(resultMap);
			log.info(loginCookie);
//			headers.put("Cookie", loginCookie);
			// 4.
			rsp = session.get("http://www.cnet.com/profile/?tag=hdr;brandnav", headers, null, "utf-8");
			HttpClientUtil.saveToFile(rsp.getEntity(), "profile.html");
			
			//5.
			rsp = session.get("http://www.cnet.com/profile/colinconger/?tag=contentMain;contentBody", headers, null, "utf-8");
			HttpClientUtil.saveToFile(rsp.getEntity(), "community-profile.html");
			
			
			//4.1 Modify Profile
//			String modifyProfileURL = "http://www.cnet.com" + getModfiyProfileURL("profile.html") + "&tag=contentMain;contentAux";
//			System.out.println(modifyProfileURL);
//			
//			rsp = session.get(modifyProfileURL, headers, null, "utf-8");
//			HttpClientUtil.saveToFile(rsp.getEntity(), "modifyProfile.html");
//			
//			
//			modifyProfile(modifyProfileURL, "Colin", "Conger", "MALE", "08/17/1983");
		}
		catch(Exception e)
		{
			log.error(e.toString(), e);
		}
	}
	
	private int modifyProfile(String modifyProfileURL, 
		String firstName, String lastName, String gender, String birthday) throws Exception
	{
		int rc = 0;
		Map<String, String> params = new HashMap<String, String>();
		do
		{
			params.put("COUNTRY", "US");
			params.put("DATEOFBIRTH", birthday);
			params.put("EMAILADDR", "aaaa");
			params.put("FIRSTNAME", firstName);
			params.put("LASTNAME", lastName);
			params.put("GENDER", gender);
			params.put("metaparameter_checkboxes", "REMEMBERME");
			params.put("POSTALCODE", "92131");
			params.put("REMEMBERME", "1");
			params.put("path", "/8732-1-0.html_siteid_=1");
			
			
			Map<String, String> headers = new HashMap<String, String>();
			headers.put("Referer", modifyProfileURL);
			headers.put("Host", "www.cnet.com");
			
			
			HttpResponse rsp = session.post("http://www.cnet.com/1345-4_1-103.html",
				headers, params, "utf-8");

			if(rsp.getStatusLine().getStatusCode() == 302)
			{
				Header[] locs = rsp.getHeaders("Location");
				if(locs != null)
				{
					for(int i = 0; i < locs.length; i++)
					{
						log.info(locs[i].getValue());
					}
				}
			}
		}
		while(false);
		
		return rc;
	}
	
	private static String getModfiyProfileURL(String fileName)
	{
		String result = null;
		try
		{
			String regex = "<ul class=\"settings\"> <li><a href=\"(.+)\">Update my e-mail address</a></li> <li>";
			InputStream is = new FileInputStream(fileName);
			String input = IOUtils.toString(is);
			
			Pattern pattern = Pattern.compile(regex);
			Matcher matcher = pattern.matcher(input);
			
			int matchCount = 0;
			while(matcher.find())
			{
				matchCount++;
				
				int count = matcher.groupCount();
				if(count < 1)
				{
					continue;
				}
				
				result = matcher.group(1);
			}
			
			if(matchCount > 1)
			{
				System.err.println("Too much match count: " + matchCount);
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
		return result;
	}
	
	private void updateCookie(Map<String, String> resultMap)
	{
		String[] names = {"urs_sessionId", "purs_1", "surs_1", 
			"curs_gigya_linked", "curs_gigya_appid", "curs_fb_linked"};
		
		for(int i = 0; i < names.length; i++)
		{
			updateCookie(names[i], resultMap);
			resultMap.remove(names[i]);
		}
		
		String key = null;
		for(Iterator<String> iter = resultMap.keySet().iterator(); iter.hasNext(); )
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
		session.hc.getCookieStore().addCookie(cookie);
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
	
	private static String toCookie(Map<String, String> resultMap)
	{
		StringBuilder sb = new StringBuilder();
		
		for(Iterator<String> iter = resultMap.keySet().iterator(); iter.hasNext(); )
		{
			String key = iter.next();
			String value = resultMap.get(key);
			
			sb.append(key + "=" + value + "; ");
		}
		
		if(sb.length() >= 2)
		{
			sb.delete(sb.length() - 2, sb.length());
		}
		
		return sb.toString();
	}
	
	private static int parseLoginResult(String jsonText, Map<String, String> resultMap)
	{
		int rc = 0;
		do
		{
			JSONObject root = (JSONObject) JSONSerializer.toJSON(jsonText);		
			JSONObject rsp = root.getJSONObject("RpsResponse");		
			JSONObject status = rsp.getJSONObject("Status");
			
			String state = status.getString("@state");
			//Failed
			if("failed".equals(state))
			{
				String errorMessage = status.getJSONObject("Error").getJSONObject("ErrorMessage").getString("$");
				System.err.println(errorMessage);
				
				rc = 1;
				break;
			}
			
			JSONObject user = rsp.getJSONObject("User");
			//Successfully
			if(!"processed".equals(state) || user == null)
			{
				System.err.println("Unkown state: " + state + ", user = " + user);
				
				rc = 2;
				break;
			}
			
			status = user.getJSONObject("Status");
			int code = status.getInt("@code");
			String codeStr = status.getString("$");
			if(code != 0 || !"Authenticated".equals(codeStr))
			{
				System.err.println("Unkown user status code: " + code + ", codeStr = " + codeStr);
				
				rc = 3;
				break;				
			}
			
			String name, value;
			//purs_1
			JSONObject permToken = user.getJSONObject("PermToken");
			name = permToken.getString("@name");
			value = permToken.getString("$");
			
			resultMap.put(name, value);
			
			//surs_1
			JSONObject sessToken = user.getJSONObject("SessToken");
			name = sessToken.getString("@name");
			value = sessToken.getString("$");
			
			resultMap.put(name, value);
			
			//urs_sessionId
			JSONObject sessionId = user.getJSONObject("SessionId");
			resultMap.put("urs_sessionId", sessionId.getString("$"));
			
			//RegistrationId
			String regId = user.getJSONObject("RegistrationId").getString("$");
			resultMap.put(regId + "_tt_machines", "false");
			
			resultMap.put("WRUID", "0");
			resultMap.put("MAD_INTERNAL", "0");
			resultMap.put("curs_gigya_linked", "false");
			resultMap.put("curs_gigya_appid", "259");
			resultMap.put("curs_fb_linked", "false");
			resultMap.put("cnet_joinCallout", "1");
			resultMap.put("mad_rsi_segs", "ASK05540_10619&ASK05540_10648");
		}
		while(false);

		return rc;
	}
	
	private void prepareCookies() throws ClientProtocolException, IOException
	{
		HttpResponse rsp = null;
		
		
		rsp = session.get("http://www.cnet.com/");
		HttpClientUtil.saveToFile(rsp.getEntity(), "cnet.html");
		
		rsp = session.get("http://dw.com.com/clear/c.gif");
		HttpClientUtil.saveToFile(rsp.getEntity(), "c.gif");
	}
}
