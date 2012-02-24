package com.bao.lc.client;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Pattern;

import org.apache.commons.io.IOUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.util.EntityUtils;

import com.bao.lc.AppConfig;
import com.bao.lc.common.exception.ParseException;
import com.bao.lc.util.AppUtils;
import com.bao.lc.util.CommonUtil;
import com.bao.lc.util.HttpClientUtil;

public class RjLogin
{
	private static Log log = LogFactory.getLog(RjLogin.class);

	private BrowserClient session = null;
	private boolean isLogin = false;
	
	private String user = null;
	private String password = null;

	public RjLogin(String user, String password)
	{
		session = new BrowserClient();
		isLogin = false;
		
		this.user = user;
		this.password = password;
	}

	public int get()
	{
		int rc = 0;

		try
		{
			do
			{
				// 1. login
				if(!login(user, password))
				{
					break;
				}
				
				// 2. query reglist
				queryRegList();
			}
			while(false);
		}
		catch(Exception e)
		{
			log.error(e.toString(), e);
		}
		finally
		{
			try
			{
				logout();
			}
			catch(Exception e2)
			{
				log.error("failed to logout", e2);
			}
			
			session.getConnectionManager().shutdown();
		}
		return rc;
	}

	public boolean login(String user, String password) throws ClientProtocolException, IOException,
		ParseException
	{
		HttpResponse rsp = null;
		Map<String, String> paramMap = new HashMap<String, String>();
		String url = null;

		// 1. main page
		url = AppConfig.getInstance().getPropInternal("rjh.url.main");
		rsp = session.get(url);

		// 2. login page
		url = AppConfig.getInstance().getPropInternal("rjh.url.login");
		rsp = session.get(url);
		String content = EntityUtils.toString(rsp.getEntity(), "GB2312");
		parseLoginParameters(content, paramMap);

		// 3. validation code
		String validateToken = paramMap.get("ValidateToken");
		url = AppConfig.getInstance().getPropInternal("rjh.url.validation_code") + validateToken;
		rsp = session.get(url);

		String validationCodeImage = AppUtils.getTempFilePath("vcode.png");
		HttpClientUtil.saveToFile(rsp.getEntity(), validationCodeImage);

		// 4. show validation code
		String validationCode = CommonUtil.getValidationCode(validationCodeImage);
		if(validationCode == null)
		{
			return false;
		}

		// 5. post login
		paramMap.put("TxtCode", validationCode);
		paramMap.put("TxtLoginName", user);
		paramMap.put("TxtPwd", password);
		paramMap.remove("ValidateToken");

		url = AppConfig.getInstance().getPropInternal("rjh.url.login");
		session.addReferer(url);
		rsp = session.post(url, paramMap, "GB2312");

		// Check login result
		isLogin = (rsp.getStatusLine().getStatusCode() == 302);

		log.info(String.format("User [%s] login %s.", user, (isLogin ? "succeeded" : "failed")));
		HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("login_result.html"));

		if(isLogin)
		{
			url = AppConfig.getInstance().getPropInternal("rjh.url.user_panel");
			rsp = session.get(url);
			HttpClientUtil.saveToFile(rsp.getEntity(),
				AppUtils.getTempFilePath("login_user_panel.html"));
		}
		else
		{
			String errorReason = parseLoginErrorReason(AppUtils
				.getTempFilePath("login_result.html"));
			log.info(errorReason);
		}

		return isLogin;
	}

	public void queryRegList() throws ClientProtocolException, IOException, ParseException
	{
		String url = null, content = null;
		HttpResponse rsp = null;

		// 1. open query page
		url = AppConfig.getInstance().getPropInternal("rjh.url.query_reg_page");
		rsp = session.get(url);
		content = EntityUtils.toString(rsp.getEntity(), "GB2312");

		Map<String, String> paramMap = new HashMap<String, String>();
		parseQueryRegListPage(content, paramMap);

		// 2. do query
		rsp = session.post(url, paramMap, "GB2312");
		HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("reg_result.html"));

		List<RegInfo> regList = new ArrayList<RegInfo>();
		parseRegListResult(AppUtils.getTempFilePath("reg_result.html"), regList);
		log.info(regList);
	}
	
	public void register(String department, String name) throws ClientProtocolException, IOException, ParseException
	{
		
	}

	public void logout() throws ClientProtocolException, IOException
	{
		if(!isLogin)
		{
			log.info("User [" + user + "] doesn't login yet.");
			return;
		}

		session.addReferer(AppConfig.getInstance().getPropInternal("rjh.url.user_panel"));

		String url = AppConfig.getInstance().getPropInternal("rjh.url.logout");
		HttpResponse rsp = session.get(url);
		HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("logout.html"));

		isLogin = false;
		log.info("User [" + user + "] logout succeeded.");
	}

	private String parseLoginErrorReason(String fileName) throws IOException, ParseException
	{
		String content = IOUtils.toString(new FileInputStream(fileName), "GB2312");

		String regex = "<span id=\"LblErr\" (.+?)>(.+?)</span>";
		String result = CommonUtil.getRegexValueOnce(content, regex, 2);

		return result;
	}

	private void parseRegListResult(String fileName, List<RegInfo> regList) throws ParseException
	{
		regList.clear();
		try
		{
			String content = IOUtils.toString(new FileInputStream(fileName), "GB2312");

			String tableRegex = "<table id=\"Table1\" (.+?)<table (.+?) id=\"DataGrid1\" style=\"(.+?)\">(.+?)</table>";
			String result = CommonUtil.getRegexValueOnce(content, tableRegex, 4, Pattern.MULTILINE
				| Pattern.DOTALL | Pattern.CASE_INSENSITIVE);
			log.trace(result);

			String tableRowRegex = "<tr align=\"Right\" (.+?)</tr>(.+?)<tr align=\"Right\" (.+?)</tr>";
			result = CommonUtil.getRegexValueOnce(result, tableRowRegex, 2, Pattern.MULTILINE
				| Pattern.DOTALL | Pattern.CASE_INSENSITIVE);
			log.trace(result);

			tableRowRegex = "<tr>(.+?)</tr>";
			List<String> valueList = new ArrayList<String>();
			int rc = CommonUtil.getRegexValue(result, tableRowRegex, valueList, true,
				Pattern.MULTILINE | Pattern.DOTALL | Pattern.CASE_INSENSITIVE);

			log.info("Found regsiter record count: " + (rc - 1));

			List<String> fieldList = new ArrayList<String>(8);
			String recordRegex = "<td>(.+?)</td><td>(.+?)</td><td>(.+?)</td><td>(.+?)</td><td>(.+?)</td><td>(.+?)</td><td>(.+?)</td>";
			String record = null;
			for(int i = 3; i < valueList.size(); i += 2)
			{
				record = valueList.get(i).trim();

				fieldList.clear();

				CommonUtil.getRegexValueOnce(record, recordRegex, fieldList, true,
					Pattern.MULTILINE | Pattern.DOTALL | Pattern.CASE_INSENSITIVE);

				RegInfo regInfo = new RegInfo(fieldList, 1);
				regList.add(regInfo);
			}
		}
		catch(Exception e)
		{
			log.error(e.toString(), e);
		}
	}

	private void parseQueryRegListPage(String content, Map<String, String> paramMap)
		throws ParseException
	{
		String result = null;

		// 1. __VIEWSTATE
		String viewStateRegex = "<input type=\"hidden\" name=\"__VIEWSTATE\" value=\"(.+)\" />";
		result = CommonUtil.getRegexValueOnce(content, viewStateRegex, 1);
		paramMap.put("__VIEWSTATE", result);

		// 2. btn_search
		String btnSearchRegex = "<input type=\"submit\" name=\"btn_search\" value=\"(.+)\" id=\"btn_search\" />";
		result = CommonUtil.getRegexValueOnce(content, btnSearchRegex, 1);
		paramMap.put("btn_search", result);

		// 3. StartDate
		String startDateRegex = "name=\"txt_StartDate\" type=\"text\" value=\"(.+)\" id=\"txt_StartDate\"";
		result = CommonUtil.getRegexValueOnce(content, startDateRegex, 1);
		paramMap.put("txt_StartDate", result);

		// 4. EndDate
		String endDateRegex = "name=\"txt_EndDate\" type=\"text\" value=\"(.+)\" id=\"txt_EndDate\"";
		result = CommonUtil.getRegexValueOnce(content, endDateRegex, 1);
		paramMap.put("txt_EndDate", result);
	}

	private void parseLoginParameters(String content, Map<String, String> paramMap)
		throws ParseException
	{
		String result = null;

		// 1. validationCodeRegex
		String validationCodeRegex = "<img src=\"\\.\\./ValidateToken.aspx\\?yymm=(.+)\" id=\"IMG1\" />";
		result = CommonUtil.getRegexValueOnce(content, validationCodeRegex, 1);
		paramMap.put("ValidateToken", result);

		// 2. __VIEWSTATE
		String viewStateRegex = "<input type=\"hidden\" name=\"__VIEWSTATE\" value=\"(.+)\" />";
		result = CommonUtil.getRegexValueOnce(content, viewStateRegex, 1);
		paramMap.put("__VIEWSTATE", result);

		// 3. _stv
		String stvRegex = "<input name=\"_stv\" id=\"_stv\" type=\"hidden\" style=\"(.+)\" value=\"(.+)\" />";
		result = CommonUtil.getRegexValueOnce(content, stvRegex, 2);
		paramMap.put("_stv", result);

		paramMap.put("BtnLogin.x", "38");
		paramMap.put("BtnLogin.y", "5");
	}

	private static class RegInfo
	{
		public String hospitalName;
		public String patientName;
		public String regTime;
		public String department;
		public String doctorName;
		public String flagName;

		public RegInfo(List<String> fieldList, int fromIndex)
		{
			if(fromIndex < 0 || fromIndex > fieldList.size() - 6)
			{
				throw new IllegalArgumentException("invalid field list. fromIndex=" + fromIndex
					+ ", size=" + fieldList.size());
			}

			hospitalName = fieldList.get(fromIndex++);
			patientName = fieldList.get(fromIndex++);
			regTime = fieldList.get(fromIndex++);
			department = fieldList.get(fromIndex++);
			doctorName = fieldList.get(fromIndex++);
			flagName = fieldList.get(fromIndex++);
		}

		public String toString()
		{
			StringBuilder sb = new StringBuilder();
			sb.append("hospitalName=").append(hospitalName).append("; ");
			sb.append("patientName=").append(patientName).append("; ");
			sb.append("regTime=").append(regTime).append("; ");
			sb.append("department=").append(department).append("; ");
			sb.append("doctorName=").append(doctorName).append("; ");
			sb.append("flagName=").append(flagName);

			return sb.toString();
		}
	}

	public static void main(String[] args) throws Exception
	{
		String user = AppConfig.getInstance().getPropInput("rjh.user");
		String password = AppConfig.getInstance().getPropInput("rjh.password");
		
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
		
		RjLogin rjlogin = new RjLogin(user, password);
		rjlogin.get();
		
		System.exit(0);
	}
}
