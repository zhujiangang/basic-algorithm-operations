package com.bao.lc.site.s3.commands;

import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.regex.Pattern;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.io.IOUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.Text;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.filters.OrFilter;
import org.htmlparser.filters.StringFilter;
import org.htmlparser.tags.Div;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.ScriptTag;
import org.htmlparser.tags.Span;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.lc.AppConfig;
import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.common.ScriptCodeFilter;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.util.AppUtils;
import com.bao.lc.util.MiscUtils;

public class DoLogin extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(DoLogin.class);

	private static final String JS_CODE_REGEX = "var message = \"(.*?)\";";

	private static final String JS_LOGIN_REGEX = "var isLogin(.*?)=(.+?)var u_name = '(.*?)';";

	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		String charset = HttpClientUtils.getCharset(rsp);

		// Login page content
		String userPage = HttpClientUtils.saveToString(rsp.getEntity(), charset);
		context.put(TdPNames.LOGIN_PAGE_CONTENT, userPage);

		// For manual check
		IOUtils.write(userPage, new FileOutputStream(AppUtils.getTempFilePath("login.html")),
			charset);

		String user = MapUtils.getString(context, TdPNames.PARAM_USER);

		// Login OK
		if(isLogin(context, userPage, charset))
		{
			context.put(TdPNames.LOGIN_STATE, Boolean.TRUE);
			log.info("User [" + user + "] login successfully.");

			return ResultCode.RC_OK;
		}
		// Login Failed
		else
		{
			context.put(TdPNames.LOGIN_STATE, Boolean.FALSE);
			log.info("User [" + user + "] login failed.");

			// TODO: check failed reason.

			return ResultCode.RC_USER_NOT_LOGIN;
		}
	}

	private void parse(Context context, String pageContent, String charset) throws ParserException
	{
		Parser parser = MiscUtils.createParser(pageContent, charset, log);

		// Set filters
		List<NodeFilter> predicates = new ArrayList<NodeFilter>(3);
		NodeFilter[] a = new NodeFilter[0];

		// 1. login form filters
		predicates.add(new HasAttributeFilter("id", "loginForm"));
		predicates.add(new HasAttributeFilter("name", "loginForm"));
		predicates.add(new NodeClassFilter(FormTag.class));
		NodeFilter loginFormFilter = new AndFilter(predicates.toArray(a));

		// 2. verification code error filters
		predicates.clear();
		predicates.add(new HasAttributeFilter("id", "randErr"));
		predicates.add(new NodeClassFilter(Span.class));
		NodeFilter vCodeErrorFilter = new AndFilter(predicates.toArray(a));

		// 3. error message from java script code
		NodeFilter scriptFilter = new ScriptCodeFilter(JS_CODE_REGEX);

		// 4. successfully filter
		predicates.clear();
		// @TODO
		NodeFilter okFilter = null;

		// Final filter
		predicates.clear();
		predicates.add(loginFormFilter);
		predicates.add(vCodeErrorFilter);
		predicates.add(scriptFilter);
		predicates.add(okFilter);
		NodeFilter finalFilter = new OrFilter(predicates.toArray(a));

		// parse
		NodeList nodeList = parser.parse(finalFilter);
	}

	private boolean isLogin(Context context, String pageContent, String charset)
		throws ParserException
	{
		boolean ret = false;

		do
		{
			if(!isLoginByJS(context, pageContent, charset))
			{
				log.info("isLoginByJS failed");
				break;
			}
			if(!isLoginByMenu(context, pageContent, charset))
			{
				log.info("isLoginByMenu failed");
				break;
			}
			if(!isLoginByWelcomeMsg(context, pageContent, charset))
			{
				log.info("isLoginByWelcomeMsg failed");
				break;
			}
			ret = true;
		}
		while(false);

		return ret;
	}

	private boolean isLoginByJS(Context context, String pageContent, String charset)
		throws ParserException
	{
		Parser parser = MiscUtils.createParser(pageContent, charset, log);
		int flags = Pattern.MULTILINE | Pattern.DOTALL;

		// 1. login form filters
		NodeFilter scriptFilter = new ScriptCodeFilter(JS_LOGIN_REGEX, flags);

		// parse
		NodeList nodeList = parser.parse(scriptFilter);
		if(nodeList.size() < 1)
		{
			log.info("Can't find login Javascript code. Assume login failed.");
			return false;
		}
		ScriptTag js = (ScriptTag) nodeList.elementAt(0);
		List<String> valueList = new ArrayList<String>();
		int matchCount = MiscUtils.getRegexValue(js.getScriptCode().trim(), JS_LOGIN_REGEX,
			valueList, true, flags);
		if(matchCount != 1)
		{
			log.fatal("Unexpected: matchCount=" + matchCount);
			return false;
		}

		String loginStatus = valueList.get(2).trim();
		String loginName = valueList.get(3).trim();
		log.info("Javascript code: loginStatus=" + loginStatus + ", loginName=" + loginName);
		if(StringUtils.equalsIgnoreCase(loginStatus, "true") && !loginName.isEmpty())
		{
			return true;
		}

		return false;
	}

	private boolean isLoginByMenu(Context context, String pageContent, String charset)
		throws ParserException
	{
		Parser parser = MiscUtils.createParser(pageContent, charset, log);

		// Set filters
		List<NodeFilter> predicates = new ArrayList<NodeFilter>(3);
		NodeFilter[] a = new NodeFilter[0];

		predicates.add(new HasAttributeFilter("id", "left_down_z"));
		predicates.add(new HasAttributeFilter("class", "left_down_z"));
		predicates.add(new NodeClassFilter(Div.class));

		NodeFilter filter = new AndFilter(predicates.toArray(a));
		predicates.clear();

		NodeList nodeList = parser.parse(filter);
		if(nodeList.size() <= 0)
		{
			log.info("Can't find the DIV with id=left_down_z. Assume login failed.");
			return false;
		}

		Div div = (Div) nodeList.elementAt(0);
		if(div.getChildren() == null)
		{
			log.info("Can't find any children by the DIV.");
			return false;
		}
		nodeList = div.getChildren();

		predicates.add(new StringFilter(AppConfig.getInstance()
			.getPropInternal("td.login.ok.menu1"), false, Locale.SIMPLIFIED_CHINESE));
		predicates.add(new StringFilter(AppConfig.getInstance()
			.getPropInternal("td.login.ok.menu2"), false, Locale.SIMPLIFIED_CHINESE));
		predicates.add(new StringFilter(AppConfig.getInstance()
			.getPropInternal("td.login.ok.menu3"), false, Locale.SIMPLIFIED_CHINESE));

		int expectedMenuCount = predicates.size();

		filter = new OrFilter(predicates.toArray(a));
		predicates.clear();

		NodeList menuList = nodeList.extractAllNodesThatMatch(filter, true);
		log.info("Found menu count: " + menuList.size() + ", expected: " + expectedMenuCount);
		if(menuList.size() < expectedMenuCount)
		{
			return false;
		}

		return true;
	}

	private boolean isLoginByWelcomeMsg(Context context, String pageContent, String charset)
		throws ParserException
	{
		Parser parser = MiscUtils.createParser(pageContent, charset, log);

		NodeFilter filter = new StringFilter(AppConfig.getInstance().getPropInternal(
			"td.login.ok.msg1"), false, Locale.SIMPLIFIED_CHINESE);

		NodeList msgList = parser.parse(filter);
		log.info("Found message count: " + msgList.size());
		if(msgList.size() <= 0)
		{
			return false;
		}

		Text msgText = (Text) msgList.elementAt(0);
		log.info("The welcome message: " + msgText.getText());

		return true;
	}
}
