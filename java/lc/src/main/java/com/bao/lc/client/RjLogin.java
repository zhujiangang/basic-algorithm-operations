package com.bao.lc.client;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Pattern;

import org.apache.commons.io.IOUtils;
import org.apache.commons.lang.StringUtils;
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
	private String department = null;
	private String doc = null;
	private int week = 5;
	private int dayOfWeek = 5;

	public RjLogin(String user, String password, String department, String doc, int week, int dayOfWeek)
	{
		session = new BrowserClient();
		isLogin = false;
		
		this.user = user;
		this.password = password;
		this.department = department;
		this.doc = doc;
		this.week = week;
		this.dayOfWeek = dayOfWeek;
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
				queryRegList(false);
				
				//3. 
				boolean bRegResult = register(department, doc, week, dayOfWeek);
				if(bRegResult)
				{
					log.info("Register Successfully!");
				}
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

	public boolean queryRegList(boolean check) throws ClientProtocolException, IOException, ParseException
	{
		String url = null, content = null;
		HttpResponse rsp = null;

		// 1. open query page
		url = AppConfig.getInstance().getPropInternal("rjh.url.query_reg_page");
		session.addReferer(AppConfig.getInstance().getPropInternal("rjh.url.user_panel"));
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
		
		if(check)
		{
			String mark = AppConfig.getInstance().getPropInternal("rjh.mark.reg_valid");
			for(int i = 0, size = regList.size(); i < size; i++)
			{
				RegInfo regInfo = regList.get(i);
				
				if(!regInfo.flagName.equals(mark))
				{
					continue;
				}
				if(!regInfo.department.contains(department))
				{
					continue;
				}
				if(!regInfo.doctorName.equals(doc))
				{
					continue;
				}
				
				return true;
			}
			return false;
		}
		
		return true;
	}
	
	public boolean register(String department, String name, int week, int dayOfWeek) throws ClientProtocolException, IOException, ParseException
	{
		boolean bResult = false;
		
		String url = null, content = null;
		HttpResponse rsp = null;
		Map<String, String> paramMap = new HashMap<String, String>();
		
		do
		{
			//1. Get register page
			url = AppConfig.getInstance().getPropInternal("rjh.url.reg_page");
			session.addReferer(AppConfig.getInstance().getPropInternal("rjh.url.user_panel"));
			rsp = session.get(url);
			if(rsp.getStatusLine().getStatusCode() != 200)
			{
				break;
			}			
			content = HttpClientUtil.saveToString(rsp.getEntity(), "GB2312");

			//2. Query available register list
			parseRegPageParameter(content, department, week, paramMap);
			rsp = session.post(url, paramMap, "GB2312");
			if(rsp.getStatusLine().getStatusCode() != 200)
			{
				break;
			}
			content = HttpClientUtil.saveToString(rsp.getEntity(), "GB2312");
			
			//3. Parse available register list
			Map<Integer, List<RegDoctorInfo>> availMap = new HashMap<Integer, List<RegDoctorInfo>>();
			parseAvailableRegList(content, availMap);
			
			List<RegDoctorInfo> regDoctorInfoList = availMap.get(dayOfWeek);
			if(regDoctorInfoList == null || regDoctorInfoList.isEmpty())
			{
				log.info("No available register doctors.");
				break;
			}
			RegDoctorInfo regDoctorInfo = null;
			for(int i = 0, size = regDoctorInfoList.size(); i < size; i++)
			{
				if(name.equals(regDoctorInfoList.get(i).name))
				{
					regDoctorInfo = regDoctorInfoList.get(i);
					break;
				}
			}
			if(regDoctorInfo == null)
			{
				log.info("No specified doctor, doctor=" + name);
				break;
			}
			
			url = regDoctorInfo.href.replaceAll(" ", "%20");
			url = AppConfig.getInstance().getPropInternal("rjh.url.reg_doctor_root") + url;
			
			
			rsp = session.get(url);
			if(rsp.getStatusLine().getStatusCode() != 200)
			{
				break;
			}
			content = HttpClientUtil.saveToString(rsp.getEntity(), "GB2312");
			
			RegDoctorInfoDetailResult parseResult = new RegDoctorInfoDetailResult();
			parseDoctorRegList(content, parseResult);
			
			if(parseResult.viewState == null || parseResult.regDoctorInfoDetailList.isEmpty())
			{
				break;
			}
			
			RegDoctorInfoDetail regDetail = parseResult.regDoctorInfoDetailList.get(0);
			boolean bHasAvailable = regDetail.isAvailable();
			
			if(!bHasAvailable)
			{
				log.info("There's no available doctor to register!");
				break;
			}
			paramMap.clear();
			paramMap.put("__VIEWSTATE", parseResult.viewState);
			paramMap.put("__EVENTTARGET", regDetail.eventTarget);
			paramMap.put("__EVENTARGUMENT", regDetail.eventArgument);
			rsp = session.post(url, paramMap, "GB2312");
			if(rsp.getStatusLine().getStatusCode() != 200)
			{
				break;
			}
			content = HttpClientUtil.saveToString(rsp.getEntity(), "GB2312");
			
			paramMap.clear();
			parseDoRegisterParameters(content, paramMap);
			rsp = session.post(url, paramMap, "GB2312");
			if(rsp.getStatusLine().getStatusCode() != 200)
			{
				break;
			}
			content = HttpClientUtil.saveToString(rsp.getEntity(), "GB2312");
			String result = parseDoRegisterResult(content);
			log.info("[RESULT]: " + result);
			
			if(result.contains(AppConfig.getInstance().getPropInternal("rjh.mark.reg_result_ok")))
			{
				bResult = queryRegList(true);
			}
			else
			{
				bResult = false;
			}
		}
		while(false);
		
		if(!bResult)
		{
			String resultFile = AppUtils.getOutputFilePath("RegResultError.html");
			HttpClientUtil.saveToFile(rsp.getEntity(), resultFile);
			
			String logInfo = String.format("[Fail] register: StatusCode=%d,URL=%s,ResultFile=%s",
				rsp.getStatusLine().getStatusCode(), url, resultFile);
			log.info(logInfo);
		}
		
		return bResult;
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
	
	private void parseRegPageParameter(String content, String departmentName, int week,
		Map<String, String> paramMap) throws ParseException
	{
		String result = null;
		int flags = Pattern.DOTALL | Pattern.MULTILINE | Pattern.CASE_INSENSITIVE;

		// 1. Get department list
		String departmentRegex = "<select name=\"ddl_ks\" id=\"ddl_ks\">(.+?)</select>";
		result = CommonUtil.getRegexValueOnce(content, departmentRegex, 1, flags);

		// Then Get department id
		String departmentIDListRegex = "<option value=\"(.+?)\">(.+?)</option>";
		List<String> valueList = new ArrayList<String>();
		int matchCount = CommonUtil.getRegexValue(result, departmentIDListRegex, valueList, true, flags);
		if(valueList.size() != matchCount * 3)
		{
			throw new ParseException("Unexpected result. matchCount=" + matchCount + ", listSize="
				+ valueList.size());
		}
		
		boolean bFound = false;
		for(int i = 0; i < matchCount; i++)
		{
			String departmentFullName = valueList.get(i * 3 + 2);
			if(departmentFullName.contains(departmentName))
			{
				bFound = true;
				
				String departmentID = valueList.get(i * 3 + 1);
				paramMap.put("ddl_ks", departmentID);
				
				String logInfo = String.format(
					"Found department [%s] with matched result: name=%s, id=%s.", departmentName,
					departmentFullName, departmentID);
				log.info(logInfo);
				
				break;
			}
		}
		
		if(!bFound)
		{
			throw new ParseException("Can't find matched department: " + departmentName);
		}

		// 2. Get ViewState
		String viewStateRegex = "<input type=\"hidden\" name=\"__VIEWSTATE\" value=\"(.+?)\" />";
		result = CommonUtil.getRegexValueOnce(content, viewStateRegex, 1, flags);
		paramMap.put("__VIEWSTATE", result);

		// 3. Get Button value
		String okBtnRegex = "<input type=\"submit\" name=\"btn_OK\" value=\"(.+?)\" id=\"btn_OK\" />";
		result = CommonUtil.getRegexValueOnce(content, okBtnRegex, 1, flags);
		paramMap.put("btn_OK", result);

		// ...
		paramMap.put("ddl_lx", "ALL");
		paramMap.put("ddl_yydm", "00");
		paramMap.put("ddl_week", String.valueOf(week));
		
		if(log.isDebugEnabled())
		{
			log.debug("parseRegPageParameter result: " + paramMap);
		}
	}
	
	private void parseAvailableRegList(String content, Map<Integer, List<RegDoctorInfo>> availMap) throws ParseException
	{
		int flags = Pattern.DOTALL | Pattern.MULTILINE | Pattern.CASE_INSENSITIVE;
		
		StringBuilder sb = new StringBuilder();
		sb.append("<TR align=\"center\">");
		String columnRegex = "(\\s*?)<TD valign=\"top\">(.*?)<table (.+?) id=\"dg_%d\" (.+?)\">(.+?)</table>(.*?)</TD>";
		for(int i = 1; i <= 6; i++)
		{
			sb.append(String.format(columnRegex, i));
		}
		sb.append("(\\s*?)</TR>");
		String regex = sb.toString();
		
		//1.
		List<String> valueList = new ArrayList<String>();
		int matchCount = CommonUtil.getRegexValue(content, regex, valueList, true, flags);
		log.info("Found matchCount=" + matchCount);
		if(matchCount <= 0)
		{
			return;
		}

		//2.
		sb.setLength(0);
		sb.append("<tr style=\"(.+?)\">");
		sb.append("(\\s*?)<td style=\"(.+?)\">(.+?)</td>");
		sb.append("(\\s*?)<td>(.+?)</td>");
		sb.append("(\\s*?)<td>(\\s*?)<a href='(.+?)' target=\"_blank\">(\\s*?)(.+?)</a>(\\s*?)</td>");
		sb.append("(\\s*?)</tr>");
		regex = sb.toString();
		
		int[] indexes = {14, 4, 6, 9};

		List<String> rawRegInfoList = new ArrayList<String>();
		List<String> validRegInfoList = new ArrayList<String>();
		for(int i = 5, week = 1, size = valueList.size(); i < size; i += 6, week++)
		{
			rawRegInfoList.clear();
			validRegInfoList.clear();
			
			String availRawRegInfo = valueList.get(i).trim();
			matchCount = CommonUtil.getRegexValue(availRawRegInfo, regex, rawRegInfoList, true, flags);
			
			List<RegDoctorInfo> regDoctorInfoList = new ArrayList<RegDoctorInfo>();
			for(int j = 0; j < matchCount; j++)
			{
				for(int k = 1; k < indexes.length; k++)
				{
					int index = j * indexes[0] + indexes[k];
					validRegInfoList.add(rawRegInfoList.get(index));
				}
				
				RegDoctorInfo regDoctorInfo = new RegDoctorInfo(validRegInfoList, j * 3);
				regDoctorInfoList.add(regDoctorInfo);
			}
			
			availMap.put(week, regDoctorInfoList);
		}
		
		if(log.isDebugEnabled())
		{
			log.debug("parseAvailableRegList result: " + toString(availMap));
		}
	}
	
	private void parseDoctorRegList(String content, RegDoctorInfoDetailResult parseResult) throws ParseException
	{
		int flags = Pattern.DOTALL | Pattern.MULTILINE | Pattern.CASE_INSENSITIVE;
		
		//1.
		String regex = "<table (.+?) id=\"dg_List\" (.+?)>(.+?)</table>";
		String result = CommonUtil.getRegexValueOnce(content, regex, 3, flags);
		
		//2.
		StringBuilder sb = new StringBuilder();
		sb.append("<tr style=\"(.+?)\">").append("(\\s*?)");
		for(int i = 0; i < 6; i++)
		{
			sb.append("<td>(.+?)</td>");
		}
		sb.append("<td align=\"(.+?)\">(\\s*?)<a id=\"(.+?)\" NAME=\"(.+?)\" href=\"javascript:__doPostBack\\('(.+?)','(.*?)'\\)\">(.+?)</a>(\\s*?)</td>");
		sb.append("(\\s*?)</tr>");
		regex = sb.toString();
		
		List<String> valueList = new ArrayList<String>();
		int matchCount = CommonUtil.getRegexValue(result, regex, valueList, true, flags);
		
		if(matchCount <= 0)
		{
			return;
		}
		
		int[] indexes = {18, 3, 4, 5, 6, 7, 8, 13, 14};
		List<String> validRegInfoList = new ArrayList<String>();
		for(int i = 0; i < matchCount; i++)
		{
			validRegInfoList.clear();
			for(int j = 1; j < indexes.length; j++)
			{
				validRegInfoList.add(valueList.get(i * indexes[0] + indexes[j]));
			}
			RegDoctorInfoDetail regDoctorInfoDetail = new RegDoctorInfoDetail(validRegInfoList, 0);
			parseResult.regDoctorInfoDetailList.add(regDoctorInfoDetail);
		}
		Collections.sort(parseResult.regDoctorInfoDetailList);
		
		//get viewstate
		regex = "<input type=\"hidden\" name=\"__VIEWSTATE\" value=\"(.+?)\"(\\s*?)/>";
		result = CommonUtil.getRegexValueOnce(content, regex, 1);
		parseResult.viewState = result;
	}
	
	private void parseDoRegisterParameters(String content, Map<String, String> paramMap) throws ParseException
	{
		String regex = null, result = null;
		
		//1. btn_Submit
		regex = "<input type=\"submit\" name=\"btn_Submit\" value=\"(.+?)\" id=\"btn_Submit\"(\\s*?)/>";
		result = CommonUtil.getRegexValueOnce(content, regex, 1);
		paramMap.put("btn_Submit", result);
		
		//2. 
		regex = "<input type=\"hidden\" name=\"__EVENTTARGET\" value=\"(.*?)\"(\\s*?)/>";
		result = CommonUtil.getRegexValueOnce(content, regex, 1);
		paramMap.put("__EVENTTARGET", result);
		
		//2. 
		regex = "<input type=\"hidden\" name=\"__EVENTARGUMENT\" value=\"(.*?)\"(\\s*?)/>";
		result = CommonUtil.getRegexValueOnce(content, regex, 1);
		paramMap.put("__EVENTARGUMENT", result);
		
		//2. 
		regex = "<input type=\"hidden\" name=\"__VIEWSTATE\" value=\"(.*?)\"(\\s*?)/>";
		result = CommonUtil.getRegexValueOnce(content, regex, 1);
		paramMap.put("__VIEWSTATE", result);
	}
	
	private String parseDoRegisterResult(String content) throws ParseException
	{
		String regex = "<span id=\"lbl_Message\">(.*?)</span>";
		return CommonUtil.getRegexValueOnce(content, regex, 1);
	}
	
	private static class RegDoctorInfoDetailResult
	{
		public String viewState = null;
		public List<RegDoctorInfoDetail> regDoctorInfoDetailList = null;
		
		public RegDoctorInfoDetailResult()
		{
			this.viewState = null;
			this.regDoctorInfoDetailList = new ArrayList<RegDoctorInfoDetail>();
		}
	}
	
	private String toString(Map<Integer, List<RegDoctorInfo>> availMap)
	{
		String LF = "\r\n";
		StringBuilder sb = new StringBuilder();
		sb.append("Size=").append(availMap.size());
		
		for(int week = 1; week <= 6; week++)
		{
			sb.append(LF);
			sb.append("week[").append(week).append("]: ");
			
			List<RegDoctorInfo> regDoctorInfoList = availMap.get(week);
			if(regDoctorInfoList != null && !regDoctorInfoList.isEmpty())
			{
				sb.append(regDoctorInfoList);
			}
		}
		
		return sb.toString();
	}

	private static class RegDoctorInfoDetail implements Comparable<RegDoctorInfoDetail>
	{
		public String name;
		public String timeStart;
		public String timeEnd;
		public int total;
		public int used;
		
		public String eventTarget;
		public String eventArgument;
		
		public RegDoctorInfoDetail(List<String> fieldList, int fromIndex)
		{
			if(fromIndex < 0 || fromIndex > fieldList.size() - 8)
			{
				throw new IllegalArgumentException("invalid field list. fromIndex=" + fromIndex
					+ ", size=" + fieldList.size());
			}
			
			
			timeStart = fieldList.get(fromIndex++);
			timeEnd = fieldList.get(fromIndex++);
			name = fieldList.get(fromIndex++);
			
			String temp = fieldList.get(fromIndex++);
			total = CommonUtil.toInt(temp);
			
			temp = fieldList.get(fromIndex++);
			used = CommonUtil.toInt(temp);
			
			fromIndex++;
			
			eventTarget = fieldList.get(fromIndex++);
			eventArgument = fieldList.get(fromIndex++);
			
			execJS();
		}
		
		public boolean isAvailable()
		{
			return (total > 0) && ((total - used) > 0);
		}
		
		public int compareTo(RegDoctorInfoDetail o)
		{
			if(!isAvailable() && !o.isAvailable())
			{
				return 0;
			}
			
			if(!isAvailable())
			{
				return 1;
			}
			if(!o.isAvailable())
			{
				return -1;
			}
			
			int result = this.timeStart.compareTo(o.timeStart);
			if(result != 0)
			{
				return result;
			}
			
			return 0;
		}
		
		private void execJS()
		{
			if(eventTarget != null)
			{
				String[] a = eventTarget.split("\\$");
				eventTarget = StringUtils.join(a, ':');
			}
		}
		
		public String toString()
		{
			StringBuilder sb = new StringBuilder();
			sb.append("name=").append(name);
			sb.append("; ").append("timeStart=").append(timeStart);
			sb.append("; ").append("timeEnd=").append(timeEnd);
			sb.append("; ").append("total=").append(total);
			sb.append("; ").append("used=").append(used);
			sb.append("; ").append("eventTarget=").append(eventTarget);
			sb.append("; ").append("eventArgument=").append(eventArgument);

			return sb.toString();
		}
	}
	private static class RegDoctorInfo
	{
		public String name;
		public String level;
		public String href;
		
		public RegDoctorInfo(List<String> fieldList, int fromIndex)
		{
			if(fromIndex < 0 || fromIndex > fieldList.size() - 3)
			{
				throw new IllegalArgumentException("invalid field list. fromIndex=" + fromIndex
					+ ", size=" + fieldList.size());
			}
			
			name = fieldList.get(fromIndex++);
			level = fieldList.get(fromIndex++);
			href = fieldList.get(fromIndex++);
		}
		
		public String toString()
		{
			StringBuilder sb = new StringBuilder();
			sb.append("name=").append(name);
			sb.append("; ").append("level=").append(level);
			sb.append("; ").append("href=").append(href);

			return sb.toString();
		}
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
		String department = AppConfig.getInstance().getPropInput("rjh.department");
		String docName = AppConfig.getInstance().getPropInput("rjh.doctor");
		
		String temp = AppConfig.getInstance().getPropInput("rjh.week");
		int week = CommonUtil.toInt(temp);
		if(week <= 0 || week > 5)
		{
			week = 5;
		}
		
		temp = AppConfig.getInstance().getPropInput("rjh.dayOfWeek");
		int dayOfWeek = CommonUtil.toInt(temp);
		if(dayOfWeek < 1 || dayOfWeek > 6)
		{
			dayOfWeek = 5;
		}
		
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
		log.info(String.format(
			"user=%s, password=%s, department=%s, docName=%s, week=%d, dayOfWeek=%d", user,
			password, department, docName, week, dayOfWeek));

		RjLogin rjlogin = new RjLogin(user, password, department, docName, week, dayOfWeek);
		rjlogin.get();
		
		System.exit(0);
	}
}
