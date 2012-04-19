package com.bao.lc.site.s3.commands;

import java.io.FileOutputStream;
import java.text.MessageFormat;
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
import org.htmlparser.Node;
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
import com.bao.lc.ResMgr;
import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.common.ScriptCodeFilter;
import com.bao.lc.common.URI2NameBuilder;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.site.s3.params.TdParams;
import com.bao.lc.util.AppUtils;
import com.bao.lc.util.MiscUtils;

public class DoLogin extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(DoLogin.class);

	private static final String JS_ERROR_MSG_REGEX = "var message = \"(.+?)\";";

	private static final String JS_LOGIN_REGEX = "var isLogin(.*?)=(.+?)var u_name = '(.*?)';";
	
	private String loginName = null;

	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		String charset = HttpCommandParams.getCharset(rsp, context);

		// Login page content
		URI2NameBuilder ub = new URI2NameBuilder();
		ub.uri(HttpCommandParams.getTargetRequestURI(context));
		ub.addParamName("method").encoding(charset);
		
		String userPage = HttpClientUtils.saveToString(rsp.getEntity(), charset, ub);
		context.put(TdPNames._LOGIN_PAGE_CONTENT, userPage);
		context.put(TdPNames._LOGIN_PAGE_ENCODING, charset);

		// For manual check
		IOUtils.write(userPage, new FileOutputStream(AppUtils.getTempFilePath("login.html")),
			charset);

		String user = MapUtils.getString(context, TdPNames.PARAM_USER);

		// Login OK
		if(isLogin(context, userPage, charset))
		{
			context.put(TdPNames._LOGIN_STATE, Boolean.TRUE);

			String message = MessageFormat.format(ResMgr.getString("td.msg.user.login.ok"), user, loginName);
			TdParams.getUI(context).info(message);
			log.info("User [" + user + "] login successfully.");
			
			return ResultCode.RC_OK;
		}
		// Login Failed
		else
		{
			context.put(TdPNames._LOGIN_STATE, Boolean.FALSE);
			log.info("User [" + user + "] login failed.");

			//check failed reason.
			IDValuePair rc = parseFailReason(context, userPage, charset);
			return rc;
		}
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
		loginName = valueList.get(3).trim();
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
		String lastLoginMsg = msgText.getText();
		
		TdParams.getUI(context).info(lastLoginMsg);
		log.info("The welcome message: " + lastLoginMsg);

		return true;
	}
	
	private IDValuePair parseFailReason(Context context, String pageContent, String charset) throws ParserException
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
		NodeFilter scriptFilter = new ScriptCodeFilter(JS_ERROR_MSG_REGEX);

		// Final filter
		predicates.clear();
		predicates.add(loginFormFilter);
		predicates.add(vCodeErrorFilter);
		predicates.add(scriptFilter);
		NodeFilter finalFilter = new OrFilter(predicates.toArray(a));

		// parse
		NodeList nodeList = parser.parse(finalFilter);
		
		FormTag loginForm = null;
		Span vCodeSpan = null;
		ScriptTag scriptErrorMsg = null;
		
		for(int i = 0, size = nodeList.size(); i < size; i++)
		{
			Node node = nodeList.elementAt(i);
			if(node instanceof FormTag)
			{
				loginForm = (FormTag)node;
			}
			else if(node instanceof Span)
			{
				vCodeSpan = (Span)node;
			}
			else if(node instanceof ScriptTag)
			{
				scriptErrorMsg = (ScriptTag)node;
			}
		}
		
		IDValuePair rc = ResultCode.RC_TD_LOGIN_UNKOWN_ERROR;
		String outErrorMsg = null;
		do
		{
			//RC_TD_LOGIN_VCODE_ERROR
			if(vCodeSpan != null)
			{
				rc = ResultCode.RC_TD_LOGIN_VCODE_ERROR;
				outErrorMsg = vCodeSpan.toPlainTextString().trim();
				break;
			}
			
			//Message error
			if(scriptErrorMsg != null)
			{				
				String srcCode = scriptErrorMsg.getScriptCode();
				List<String> valueList = new ArrayList<String>();
				int matchCount = MiscUtils.getRegexValue(srcCode, JS_ERROR_MSG_REGEX, valueList, true, 0);
				if(matchCount > 0)
				{
					outErrorMsg = valueList.get(1);
				}
				
				if(outErrorMsg != null)
				{
					if(outErrorMsg.contains(AppConfig.getInstance().getPropInternal("td.login.nok.account")))
					{
						rc = ResultCode.RC_TD_LOGIN_ACCOUNT_ERROR;
					}
					else if(outErrorMsg.contains(AppConfig.getInstance().getPropInternal("td.login.nok.password")))
					{
						rc = ResultCode.RC_TD_LOGIN_PASSWORD_ERROR;
					}
					else
					{
						rc = ResultCode.RC_TD_LOGIN_OTHER_MSG_ERROR;
					}
					
					break;
				}
			}
			
			if(loginForm == null)
			{
				log.error("Can't find the login form, error reason unkown.");
				rc = ResultCode.RC_TD_LOGIN_NO_SUBMIT_FORM_ERROR;
			}
			outErrorMsg = "Unkown error.";
		}
		while(false);
		
		String failMsg = "[" + outErrorMsg + "], rc=" + rc;
		String message = MessageFormat.format(ResMgr.getString("td.msg.user.login.failed"),
			MapUtils.getString(context, TdPNames.PARAM_USER), failMsg);
		TdParams.getUI(context).error(message);
		
		return rc;
	}
}
