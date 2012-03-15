package com.bao.lc.client;

import java.net.URI;
import java.net.URISyntaxException;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ChainBase;
import org.apache.commons.chain.impl.ContextBase;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.Header;
import org.apache.http.HttpResponse;
import org.apache.http.ProtocolException;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.utils.URIUtils;
import org.htmlparser.Node;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.Tag;
import org.htmlparser.Text;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.HasChildFilter;
import org.htmlparser.filters.HasSiblingFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.filters.OrFilter;
import org.htmlparser.lexer.Lexer;
import org.htmlparser.lexer.Page;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.tags.LinkTag;
import org.htmlparser.tags.OptionTag;
import org.htmlparser.tags.SelectTag;
import org.htmlparser.tags.TableColumn;
import org.htmlparser.tags.TableRow;
import org.htmlparser.tags.TableTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;
import org.htmlparser.util.ParserFeedback;
import org.htmlparser.visitors.NodeVisitor;

import com.bao.lc.AppConfig;
import com.bao.lc.common.HttpResult;
import com.bao.lc.common.IDValuePair;
import com.bao.lc.common.LinkTextRegexFilter;
import com.bao.lc.common.LoggerFeedback;
import com.bao.lc.common.ResultCode;
import com.bao.lc.util.AppUtils;
import com.bao.lc.util.CommonUtil;
import com.bao.lc.util.HttpClientUtil;

@SuppressWarnings("unchecked")
public class ZyClient
{
	private static Log log = LogFactory.getLog(ZyClient.class);

	private static final String LOGIN_STATE_KEY = "login.state";
	private static final String LOGIN_URL_KEY = "login.url";
	private static final String FOLLOW_UP_REFERER_KEY = "followup.referer";
	private static final String FOLLOW_UP_URL_KEY = "followup.url";
	private static final String USER_PAGE_URI_KEY = "user.page.url";
	private static final String USER_PAGE_CONTENT_KEY = "user.page.content";
	private static final String LOGOUT_URI_KEY = "logout.link";
	private static final String RESULT_KEY = "result";

	/**
	 * Input parameters
	 */
	private String user = null;
	private String password = null;
	private String hospital = null;
	private String doctor = null;

	private String dayOfWeek = "6a";

	private String encoding = "UTF-8";

	/**
	 * Imp variables
	 */
	private BrowserClient session = null;
	private PostRedirectStrategy redirectStrategy = null;

	/**
	 * 
	 * @param user
	 * @param password
	 * @param hospital
	 * @param doctor
	 * @param dayOfWeek
	 */

	public ZyClient(String user, String password, String hospital, String doctor, String dayOfWeek)
	{
		this.user = user;
		this.password = password;
		this.hospital = hospital;
		this.doctor = doctor;
		this.dayOfWeek = dayOfWeek;

		session = new BrowserClient();
		redirectStrategy = new PostRedirectStrategy();
		session.getParams().setParameter(RequestResetRedirectChain.REDIRECT_STRATEGY, redirectStrategy);
		session.setRedirectStrategy(redirectStrategy);
		
		session.addRequestInterceptor(new RequestResetRedirectChain());
	}

	public boolean isLogin(Context context)
	{
		Object loginState = context.get(LOGIN_STATE_KEY);
		if(loginState instanceof Boolean)
		{
			return ((Boolean)loginState).booleanValue();
		}
		return false;
	}
	
	public boolean logout(Context context) throws Exception
	{
		if(!isLogin(context))
		{
			log.info("User doesn't login yet.");
			return true;
		}
		
		URI logoutURI = (URI)context.get(LOGOUT_URI_KEY);
		if(logoutURI == null)
		{
			throw new IllegalStateException("Logout URI doesn't exist in the context!");
		}
		
		URI userPageURI = (URI)context.get(USER_PAGE_URI_KEY);
		if(userPageURI == null)
		{
			throw new IllegalStateException("userPageURI URI doesn't exist in the context!");
		}
		
		session.addReferer(userPageURI.toString());
		HttpResponse rsp = session.get(logoutURI, null, null, null);
		HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("logout.html"));
		log.info("User [" + user + "] logout result: " + rsp.getStatusLine().getStatusCode());
		
		context.put(LOGIN_STATE_KEY, Boolean.FALSE);
		
		return true;
	}

	private static Parser createParser(String text, String charset, Log logger)
	{
		Lexer lexer = new Lexer(new Page(text, charset));
		ParserFeedback feedback = new LoggerFeedback(logger);

		Parser parser = new Parser(lexer, feedback);
		return parser;
	}
	
	private URI getRedirectURI(Context context, HttpResponse response)
		throws ProtocolException, URISyntaxException
	{
		// get the location header to find out where to redirect to
		Header locationHeader = response.getFirstHeader("location");
		if(locationHeader == null)
		{
			// got a redirect response, but no location header
			throw new ProtocolException("Received redirect response " + response.getStatusLine()
				+ " but no location header");
		}
		String location = locationHeader.getValue();

		URI uri = new URI(location);

		// rfc2616 demands the location value be a complete URI
		// Location = "Location" ":" absoluteURI
		if(!uri.isAbsolute())
		{
			HttpUriRequest nextRequest = (HttpUriRequest) context.get(FOLLOW_UP_URL_KEY);
			uri = URIUtils.resolve(nextRequest.getURI(), uri);
		}
		
		if(log.isDebugEnabled())
		{
			log.debug("Redirect requested to location '" + uri.toString() + "'");
		}

		return uri;
	}

	public void test1()
	{
		Chain chain = new ChainBase();
		chain.addCommand(new CommandGetLoginPage());
		chain.addCommand(new CommandDoLogin());
		chain.addCommand(new CommandQueryTicketNumResult());
		chain.addCommand(new CommandGetTicketDetail());
		chain.addCommand(new CommandSelectTicketDate());
		chain.addCommand(new CommandDoBookTicket());

		String loginURL = AppConfig.getInstance().getPropInternal("zy.login.url");
		Context context = new ContextBase();
		context.put(LOGIN_URL_KEY, loginURL);

		boolean result = Command.PROCESSING_COMPLETE;
		try
		{
			result = chain.execute(context);
		}
		catch(Exception e)
		{
			log.error("Exception caught.", e);
		}
		finally
		{
			log.debug("Start to logout!");
			try
			{
				logout(context);
			}
			catch(Exception e2)
			{
				log.error("logout exception", e2);
			}

			// free context space
			context.clear();
			context = null;
		}

		log.info(result);
	}

	private class CommandGetLoginPage implements Command
	{
		public boolean execute(Context context) throws Exception
		{
			boolean result = PROCESSING_COMPLETE;
			IDValuePair rc = ResultCode.RC_UNKOWN;

			do
			{
				// 1. login url
				String url = (String) context.get(LOGIN_URL_KEY);
				if(url == null || url.isEmpty())
				{
					rc = ResultCode.RC_EMPTY_URL;
					break;
				}

				// 2. get login page
				HttpResponse rsp = session.get(url);
				if(rsp.getStatusLine().getStatusCode() != 200)
				{
					rc = new HttpResult(rsp.getStatusLine().getStatusCode(), url);
					break;
				}

				// 3. parse
				String text = HttpClientUtil.saveToString(rsp.getEntity(), encoding);
				Parser parser = createParser(text, encoding, log);
				parse(parser, context);

				rc = ResultCode.RC_OK;
				result = CONTINUE_PROCESSING;
			}
			while(false);

			// Post actions
			if(result == PROCESSING_COMPLETE)
			{
				context.put(RESULT_KEY, rc);
				log.error("[CommandGetLoginPage] failed: " + rc);
			}

			return result;
		}

		private void parse(Parser parser, Context context) throws Exception
		{
			// Set filters
			NodeFilter[] predicates = { new HasAttributeFilter("id", "Form1"),
				new HasAttributeFilter("name", "Form1"), new NodeClassFilter(FormTag.class) };
			NodeFilter filter = new AndFilter(predicates);

			// parse
			NodeList formNodeList = parser.parse(filter);
			if(formNodeList.size() != 1)
			{
				throw new ParserException("Found [" + formNodeList.size()
					+ "] form(s), Expected [1].");
			}

			// Found the form
			FormTag form = (FormTag) formNodeList.elementAt(0);
			NodeList inputNodeList = form.getFormInputs();
			if(inputNodeList.size() <= 0)
			{
				throw new ParserException("Found [0] form input.");
			}

			// Parameters extraction
			Map<String, String> params = new HashMap<String, String>();
			for(int i = 0, size = inputNodeList.size(); i < size; i++)
			{
				InputTag input = (InputTag) inputNodeList.elementAt(i);

				String name = input.getAttribute("name");
				String value = input.getAttribute("value");

				if("memtype".equals(name))
				{
					if("card".equalsIgnoreCase(input.getAttribute("id")))
					{
						params.put(name, value);
					}
					continue;
				}

				if("userName".equals(name))
				{
					params.put(name, user);
					continue;
				}

				if("password".equals(name))
				{
					params.put(name, password);
					continue;
				}

				// collect all the other fields.
				params.put(name, value);
			}

			// Method
			String method = form.getFormMethod();

			// Location
			String location = buildLocation("card", params.get("redirctTo"));

			URI baseURI = URI.create((String) context.get(LOGIN_URL_KEY));
			URI nextURI = URIUtils.resolve(baseURI, location);

			HttpUriRequest nextRequest = session.createRequest(nextURI, method, params, encoding,
				null);

			context.put(FOLLOW_UP_URL_KEY, nextRequest);
			log.info("Next hop: " + nextURI.toString());
		}

		private String buildLocation(String type, String redirct)
		{
			StringBuilder sb = new StringBuilder("Login.aspx?flag=login&type=");
			sb.append(CommonUtil.escapeJS(type));
			sb.append("&url-redirect-to=");
			sb.append(CommonUtil.escapeJS(redirct));
			return sb.toString();
		}
	}

	private class CommandDoLogin implements Command
	{
		public boolean execute(Context context) throws Exception
		{
			boolean result = PROCESSING_COMPLETE;
			IDValuePair rc = null;

			do
			{
				// check post url exist?
				HttpUriRequest nextRequest = (HttpUriRequest) context.get(FOLLOW_UP_URL_KEY);
				if(nextRequest == null)
				{
					log.error(FOLLOW_UP_URL_KEY + " is not available.");

					rc = ResultCode.RC_EMPTY_URL;
					break;
				}

				// Object rawReferer =
				// session.getParams().getParameter(RequestReferer.REFERER_INTERNAL);
				// post
				HttpResponse rsp = session.execute(nextRequest);
//				if(rsp.getStatusLine().getStatusCode() != 302)
//				{
//					rc = new HttpResult(rsp.getStatusLine().getStatusCode(), nextRequest.getURI()
//						.toString());
//					break;
//				}
//
//				// get the location url
//				URI nextURI = getRedirectURI(context, rsp);
//				nextRequest = session.createHttpGet(nextURI, null, null);
//				rsp = session.execute(nextRequest);
				if(rsp.getStatusLine().getStatusCode() != 200)
				{
					rc = new HttpResult(rsp.getStatusLine().getStatusCode(), nextRequest.getURI()
						.toString());
					break;
				}
				
				if(!redirectStrategy.isRedirected())
				{
					throw new IllegalStateException("Login doesn't find any redirect.");
				}

				HttpUriRequest finalRequest = redirectStrategy.getFinalRequest();
				// save the member center page
				String userPage = HttpClientUtil.saveToString(rsp.getEntity(), encoding);
				context.put(USER_PAGE_CONTENT_KEY, userPage);
				context.put(USER_PAGE_URI_KEY, finalRequest.getURI());
				
				//do parse
				URI logoutURI = getLogoutURI(context);
				context.put(LOGOUT_URI_KEY, logoutURI);
				
				//set login state
				context.put(LOGIN_STATE_KEY, Boolean.TRUE);

				rc = ResultCode.RC_OK;
				result = CONTINUE_PROCESSING;
				
				log.info("User [" + user + "] login successfully.");
			}
			while(false);

			// Post actions
			if(result == PROCESSING_COMPLETE)
			{
				context.put(RESULT_KEY, rc);
				log.error("[CommandDoLogin] failed: " + rc);
			}

			return result;
		}

		
		
		private URI getLogoutURI(Context context) throws ParserException
		{
			String userPage = (String)context.get(USER_PAGE_CONTENT_KEY);
			
			Parser parser = createParser(userPage, encoding, log);
			
			NodeList logoutList = parser.parse(new LinkTextRegexFilter(AppConfig.getInstance().getPropInternal("zy.logout.link.mark")));
			if(logoutList.size() <= 0)
			{
				throw new ParserException("No logout link found!");
			}
			
			if(logoutList.size() > 1)
			{
				log.warn("Found more than 1 logout link. size = " + logoutList.size());
			}
			LinkTag logoutLink = (LinkTag)logoutList.elementAt(0);
			String link = logoutLink.getLink();
			
			URI baseURI = (URI)context.get(USER_PAGE_URI_KEY);
			if(baseURI == null)
			{
				throw new IllegalStateException("Can't find the user page url. key=" + USER_PAGE_URI_KEY);
			}
			
			return URIUtils.resolve(baseURI, link);
		}
	}
	
	private class CommandQueryTicketNumResult implements Command
	{
		public boolean execute(Context context) throws Exception
		{
			boolean result = PROCESSING_COMPLETE;
			IDValuePair rc = ResultCode.RC_UNKOWN;

			do
			{
				URI baseURI = (URI)context.get(USER_PAGE_URI_KEY);				
				URI queryURI = getQueryExpertURL(baseURI);
				if(log.isDebugEnabled())
				{
					log.debug("Query URL: " + queryURI.toString());
				}
				
				session.addReferer(((URI)context.get(USER_PAGE_URI_KEY)).toString());
				
				//1. Get reg page list
				HttpResponse rsp = session.get(queryURI.toString());
				if(rsp.getStatusLine().getStatusCode() != 200)
				{
					rc = new HttpResult(rsp.getStatusLine().getStatusCode(), queryURI.toString());
					break;
				}
				
				//2. parse
				String queryResult = HttpClientUtil.saveToString(rsp.getEntity(), encoding);
				TicketNumResult regInfo = parse(context, queryResult);
				if(regInfo == null)
				{
					rc = ResultCode.RC_REG_LIST_EMPTY;
					result = PROCESSING_COMPLETE;
					break;
				}
				
				//3. Set next hop request
				context.put(FOLLOW_UP_REFERER_KEY, queryURI.toString());
				URI nextURI = URIUtils.resolve(queryURI, regInfo.getLink());
				context.put(FOLLOW_UP_URL_KEY, new HttpGet(nextURI));
				
				log.info("Next hop: " + nextURI.toString());
				
				//Done
				rc = ResultCode.RC_OK;
				result = CONTINUE_PROCESSING;
			}
			while(false);

			// Post actions
			if(result == PROCESSING_COMPLETE)
			{
				context.put(RESULT_KEY, rc);
				log.error("[CommandGetRegList] failed: " + rc);
			}
			return result;
		}
		
		private URI getQueryExpertURL(URI baseURI)
		{
			//Error when with Javascript escape
//			String escapedName = CommonUtil.escapeJS(doctor);
			String escapedName = CommonUtil.encode(doctor, encoding);
			String url = AppConfig.getInstance().getPropInternal("zy.query.expert.url");
			url = MessageFormat.format(url, escapedName);
			
			URI queryURI = URIUtils.resolve(baseURI, url);
			
			return queryURI;
		}
		
		private TicketNumResult parse(Context context, String regList) throws ParserException
		{
			Parser parser = createParser(regList, encoding, log);

			// Set filters
			NodeFilter[] predicates = { new HasAttributeFilter("class", "datatable"),
				new NodeClassFilter(TableTag.class) };
			NodeFilter filter = new AndFilter(predicates);
			
			NodeList dataTableList = parser.parse(filter);
			if(dataTableList.size() == 0)
			{
				log.info("No available reg list.");
				return null;
			}
			
			TableTag dataTable = (TableTag)dataTableList.elementAt(0);
			TableRow[] rows = dataTable.getRows();
			if(rows.length <= 0)
			{
				log.info("No available reg list.");
				return null;
			}
			
			List<TicketNumResult> regInfoList = new ArrayList<TicketNumResult>(3);
			
			TicketNumResult regInfo = new TicketNumResult();
			RegInfoVisitor visitor = new RegInfoVisitor();
			for(int i = 0; i < rows.length; i++)
			{
				TableColumn[] cols = rows[i].getColumns();
				for(int j = 0; j < cols.length; j++)
				{
					NodeList children = cols[j].getChildren();
					
					visitor.setData(regInfo, i, false);
					children.visitAllNodesWith(visitor);
					
					if(visitor.isMeetHR())
					{
						regInfoList.add(regInfo);
						regInfo = new TicketNumResult();
					}
				}
				regInfo.prevText = null;
			}
			log.debug("done: " + regInfoList.size() + ", " + regInfoList.toString());
			
			for(int i = 0, size = regInfoList.size(); i < size; i++)
			{
				regInfo = regInfoList.get(i);

				if(StringUtils.equals(regInfo.doctor, doctor)
					&& StringUtils.equals(regInfo.hospital, hospital) && regInfo.isAvailable())
				{
					log.info("Found target: " + regInfo);
					return regInfo;
				}
			}
			
			log.info("Can't find the target registration.");
			return null;
		}
	}
	
	private class CommandGetTicketDetail implements Command
	{
		public boolean execute(Context context) throws Exception
		{
			boolean result = PROCESSING_COMPLETE;
			IDValuePair rc = ResultCode.RC_UNKOWN;

			do
			{
				// check follow up url exist?
				HttpUriRequest nextRequest = (HttpUriRequest) context.get(FOLLOW_UP_URL_KEY);
				if(nextRequest == null)
				{
					log.error(FOLLOW_UP_URL_KEY + " is not available.");
					rc = ResultCode.RC_EMPTY_URL;
					break;
				}
				
				String referer = (String)context.get(FOLLOW_UP_REFERER_KEY);
				if(referer != null)
				{
					session.addReferer(referer);
				}

				HttpResponse rsp = session.execute(nextRequest);
				if(rsp.getStatusLine().getStatusCode() != 200)
				{
					rc = new HttpResult(rsp.getStatusLine().getStatusCode(), nextRequest.getURI()
						.toString());
					break;
				}
				
				//2. parse
				String queryResult = HttpClientUtil.saveToString(rsp.getEntity(), encoding);
				parse(context, queryResult);
				
				//Done
				rc = ResultCode.RC_OK;
				result = CONTINUE_PROCESSING;
			}
			while(false);

			// Post actions
			if(result == PROCESSING_COMPLETE)
			{
				context.put(RESULT_KEY, rc);
				log.error("[CommandGetTicketDetail] failed: " + rc);
			}
			return result;
		}
		
		private void parse(Context context, String queryResult) throws ParserException
		{
			Parser parser = createParser(queryResult, encoding, log);

			// Set filters
			List<NodeFilter> predicates = new ArrayList<NodeFilter>(4);
			NodeFilter[] a = new NodeFilter[0];
			
			//1. Submit filters
			predicates.add(new HasAttributeFilter("id", "submitForm"));
			predicates.add(new HasAttributeFilter("name", "submitForm"));
			predicates.add(new NodeClassFilter(FormTag.class));
			NodeFilter submitFormFilter = new AndFilter(predicates.toArray(a));
			
			//2. Diag time filters
			predicates.clear();
			predicates.add(new HasAttributeFilter("id", "rdDiagtime"));
			predicates.add(new HasAttributeFilter("name", "rdDiagtime"));
			predicates.add(new HasAttributeFilter("type", "radio"));
			predicates.add(new NodeClassFilter(InputTag.class));
			NodeFilter inputsFilter = new AndFilter(predicates.toArray(a));
			
			NodeFilter uncleFilter = new AndFilter(new HasSiblingFilter(new HasChildFilter(
				inputsFilter)), new NodeClassFilter(TableColumn.class));
			NodeFilter diagTimeFilter = new OrFilter(inputsFilter, uncleFilter);
			
			//3. Consumer List filters
			predicates.clear();
			predicates.add(new HasAttributeFilter("id", "ddlConsumerList"));
			predicates.add(new HasAttributeFilter("name", "ddlConsumerList"));
			predicates.add(new NodeClassFilter(SelectTag.class));
			NodeFilter consumerListFilter = new AndFilter(predicates.toArray(a));
			
			//Total filter
			predicates.clear();
			predicates.add(submitFormFilter);
			predicates.add(diagTimeFilter);
			predicates.add(consumerListFilter);
			NodeFilter finalFilter = new OrFilter(predicates.toArray(a));
			
			//Fire!
			NodeList nodeList = parser.parse(finalFilter);
			
			FormTag submitForm = null;
			SelectTag consumerList = null;
			Map<String, String> diagTimeMap = new HashMap<String, String>();
			
			InputTag diagTimeInput = null;
			TableColumn diagTimeColumn = null;
			for(int i = 0, size = nodeList.size(); i < size; i++)
			{
				Node node = nodeList.elementAt(i);
				
				if(node instanceof FormTag)
				{
					submitForm = (FormTag)node;
					continue;
				}
				else if(node instanceof SelectTag)
				{
					consumerList = (SelectTag)node;
					continue;
				}
				else if(node instanceof InputTag)
				{
					diagTimeInput = (InputTag)node;
				}
				else if(node instanceof TableColumn)
				{
					if(i == 0)
					{
						throw new ParserException("Unexpected TableColumn at index(0).");
					}
					Node prevNode = nodeList.elementAt(i - 1);
					if(prevNode == diagTimeInput)
					{
						diagTimeColumn = (TableColumn)node;
						
						String timeText = StringUtils.strip(diagTimeColumn.toPlainTextString(), " \t\r\n");
						String timeValue = diagTimeInput.getAttribute("value");
						
						timeText = CommonUtil.getTableColumnText(diagTimeColumn);
						timeValue = StringUtils.trim(timeValue);
						if(StringUtils.isEmpty(timeText))
						{
							throw new ParserException("Unexpected empty diag time.");
						}
						
						diagTimeMap.put(timeText, timeValue);
					}
				}
			}
			
			if(submitForm == null || consumerList == null || diagTimeMap.isEmpty())
			{
				throw new ParserException("The required filed does't exist.");
			}
			
			//1. Diag Time
			String expectedDiagTime = AppUtils.getDayOfWeekTextCN1(dayOfWeek);
			String diagTimeValue = diagTimeMap.get(expectedDiagTime);
			log.debug("expectedDiagTime=" + expectedDiagTime + ", diagTimeValue=" + diagTimeValue);
			
			Map<String, ConsumerInfo> consumerMap = new HashMap<String, ConsumerInfo>();
			OptionTag[] options = consumerList.getOptionTags();
			for(int i = 0; i < options.length; i++)
			{
				String name = options[i].getAttribute("name");
				if(name == null || name.isEmpty())
				{
					continue;
				}
				
				ConsumerInfo consumer = new ConsumerInfo();
				
				consumer.name = name;
				consumer.id = options[i].getAttribute("id");
				consumer.value = options[i].getAttribute("value");
				consumer.phone = options[i].getAttribute("phone");
				consumer.medical = options[i].getAttribute("medical");
				
				consumerMap.put(name, consumer);
			}
			String consumerName = AppConfig.getInstance().getPropInput("zy.consumer.name");
			ConsumerInfo consumer = consumerMap.get(consumerName);
			if(consumer == null)
			{
				throw new IllegalArgumentException("The user doesn't exist. " + consumerName);
			}
			
			Map<String, String> paramMap = new HashMap<String, String>();
			NodeList submitInputs = submitForm.getFormInputs();
			for(int i = 0, size = submitInputs.size(); i < size; i++)
			{
				InputTag input = (InputTag)submitInputs.elementAt(i);
				
				String name = input.getAttribute("name");
				if(name == null || name.isEmpty())
				{
					log.debug("Empty name input: " + input);
					continue;
				}
				String value = null;
				//Time
				if(name.equals("diagtimeid"))
				{
					value = diagTimeValue;
				}
				//Consumer
				else if(name.equals("txtConsumerName"))
				{
					value = consumer.name;
				}
				else if(name.equals("txtConsumerIdNo"))
				{
					value = consumer.value;
				}
				else if(name.equals("txtConsumerId"))
				{
					value = consumer.id;
				}
				else if(name.equals("txtConsumerTel"))
				{
					value = consumer.phone;
				}
				//other fields
				else
				{
					value = input.getAttribute("value");
				}
				
				paramMap.put(name, value);
			}
			
			if(log.isDebugEnabled())
			{
				log.debug("ParamMap: " + CommonUtil.toString(paramMap));
			}
			
			String method = submitForm.getFormMethod();
			String location = submitForm.getFormLocation();
			
			//Build the next URI
			HttpUriRequest theRequest = (HttpUriRequest) context.get(FOLLOW_UP_URL_KEY);
			URI baseURI = theRequest.getURI();
			URI nextURI = URIUtils.resolve(baseURI, location);
			
			//Next Request
			HttpUriRequest nextRequest = session.createRequest(nextURI, method, paramMap, encoding,
				null);

			context.put(FOLLOW_UP_REFERER_KEY, baseURI.toString());
			context.put(FOLLOW_UP_URL_KEY, nextRequest);
			log.info("Next hop: " + nextURI.toString());
		}
	}
	
	
	private class CommandSelectTicketDate implements Command
	{
		public boolean execute(Context context) throws Exception
		{
			boolean result = PROCESSING_COMPLETE;
			IDValuePair rc = ResultCode.RC_UNKOWN;

			do
			{
				// check follow up url exist?
				HttpUriRequest nextRequest = (HttpUriRequest) context.get(FOLLOW_UP_URL_KEY);
				if(nextRequest == null)
				{
					log.error(FOLLOW_UP_URL_KEY + " is not available.");
					rc = ResultCode.RC_EMPTY_URL;
					break;
				}
				
				String referer = (String)context.get(FOLLOW_UP_REFERER_KEY);
				if(referer != null)
				{
					session.addReferer(referer);
				}

				HttpResponse rsp = session.execute(nextRequest);
				if(rsp.getStatusLine().getStatusCode() != 200)
				{
					rc = new HttpResult(rsp.getStatusLine().getStatusCode(), nextRequest.getURI()
						.toString());
					break;
				}
				
				//2. parse
				String content = HttpClientUtil.saveToString(rsp.getEntity(), encoding);
				parse(context, content);
				
				//Done
				rc = ResultCode.RC_OK;
				result = CONTINUE_PROCESSING;
			}
			while(false);

			// Post actions
			if(result == PROCESSING_COMPLETE)
			{
				context.put(RESULT_KEY, rc);
				log.error("[CommandSelectTicketDate] failed: " + rc);
			}
			return result;
		}
		
		private void parse(Context context, String content) throws ParserException
		{
			Parser parser = createParser(content, encoding, log);

			// Set filters
			List<NodeFilter> predicates = new ArrayList<NodeFilter>(2);
			NodeFilter[] a = new NodeFilter[0];

			//1. Submit filters
			predicates.add(new HasAttributeFilter("id", "submitForm"));
			predicates.add(new NodeClassFilter(FormTag.class));
			NodeFilter submitFormFilter = new AndFilter(predicates.toArray(a));
			predicates.clear();
			
			//2. time select
			predicates.add(new HasAttributeFilter("name", "ddlselect"));
			predicates.add(new NodeClassFilter(InputTag.class));
			NodeFilter selectFilter = new AndFilter(predicates.toArray(a));
			predicates.clear();
			
			//3. 
			predicates.add(new HasAttributeFilter("name", "ddlRegisterTime"));
			predicates.add(new HasAttributeFilter("date"));
			NodeFilter regTimeFilter = new AndFilter(predicates.toArray(a));
			predicates.clear();
			
			
			//4. final filter
			predicates.add(submitFormFilter);
			predicates.add(selectFilter);
			predicates.add(regTimeFilter);
			NodeFilter finalFilter = new OrFilter(predicates.toArray(a));
			predicates.clear();
			
			//Fire!
			NodeList nodeList = parser.parse(finalFilter);
			
			FormTag submitForm = null;
			List<InputTag> timeList = new ArrayList<InputTag>(2);
			List<Tag> regTimeList = new ArrayList<Tag>(0);
			
			for(int i = 0, size = nodeList.size(); i < size; i++)
			{
				Node node = nodeList.elementAt(i);
				if(!(node instanceof Tag))
				{
					log.debug("Non-Tag node. " + node);
					continue;
				}
				
				Tag tagNode = (Tag)node;
				
				String name = tagNode.getAttribute("name");
				if(StringUtils.equals(name, "ddlRegisterTime"))
				{
					regTimeList.add(tagNode);
					continue;
				}
				else if(StringUtils.equals(name, "ddlselect"))
				{
					timeList.add((InputTag)tagNode);
					continue;
				}
				
				String id = tagNode.getAttribute("id");
				if(StringUtils.equals(id, "submitForm"))
				{
					submitForm = (FormTag)tagNode;
				}
			}
			
			if(log.isInfoEnabled())
			{
				StringBuilder sb = new StringBuilder("[SelectTicket]: count=");
				sb.append(timeList.size());
				for(int i = 0, size = timeList.size(); i < size; i++)
				{
					sb.append(" ").append(timeList.get(i).getAttribute("value"));
				}
				log.info(sb.toString());
			}
			
			if(timeList.isEmpty())
			{
//				context.put(RESULT_KEY, ResultCode.RC_DOCTOR_REG_LIST_FULL);
//				
//				log.info("[IMPORTANT]: No available tickets!");
//				return;
				throw new ParserException("[IMPORTANT]: No available tickets!");
			}
			
			String diagDate = null;
			for(int i = 0, size = timeList.size(); i < size; i++)
			{
				String value = timeList.get(i).getAttribute("value");
				if(diagDate == null)
				{
					diagDate = value;
				}
				else 
				{
					//pick up the big one
					if(diagDate.compareTo(value) < 0)
					{
						diagDate = value;
					}
				}
			}
			log.info("Target reg date: " + diagDate);
			
			String registertime = "";
			for(int i = 0, size = regTimeList.size(); i < size; i++)
			{
				Tag tag = regTimeList.get(i);
				String date = tag.getAttribute("date");
				if(StringUtils.equals(date, diagDate))
				{
					registertime = tag.getAttribute("value");
					break;
				}
			}
			
			Map<String, String> paramMap = new HashMap<String, String>();
			NodeList submitInputs = submitForm.getFormInputs();
			for(int i = 0, size = submitInputs.size(); i < size; i++)
			{
				InputTag input = (InputTag)submitInputs.elementAt(i);
				
				String name = input.getAttribute("name");
				String value = input.getAttribute("value");
				if(name.equals("diagdate"))
				{
					value = diagDate;
				}
				else if(name.equals("registertime"))
				{
					value = registertime;
				}
				paramMap.put(name, value);
			}
			
			if(log.isDebugEnabled())
			{
				log.debug("ParamMap: " + CommonUtil.toString(paramMap));
			}
			
			String method = submitForm.getFormMethod();
			String location = submitForm.getFormLocation();
			
			//Build the next URI
			HttpUriRequest theRequest = (HttpUriRequest) context.get(FOLLOW_UP_URL_KEY);
			URI baseURI = theRequest.getURI();
			URI nextURI = URIUtils.resolve(baseURI, location);
			
			//Next Request
			HttpUriRequest nextRequest = session.createRequest(nextURI, method, paramMap, encoding,
				null);

			context.put(FOLLOW_UP_REFERER_KEY, baseURI.toString());
			context.put(FOLLOW_UP_URL_KEY, nextRequest);
			log.info("Next hop: " + nextURI.toString());
		}
	}
	
	private class CommandDoBookTicket implements Command
	{
		public boolean execute(Context context) throws Exception
		{
			boolean result = PROCESSING_COMPLETE;
			IDValuePair rc = ResultCode.RC_UNKOWN;

			do
			{
				// check follow up url exist?
				HttpUriRequest nextRequest = (HttpUriRequest) context.get(FOLLOW_UP_URL_KEY);
				if(nextRequest == null)
				{
					log.error(FOLLOW_UP_URL_KEY + " is not available.");
					rc = ResultCode.RC_EMPTY_URL;
					break;
				}
				
				String referer = (String)context.get(FOLLOW_UP_REFERER_KEY);
				if(referer != null)
				{
					session.addReferer(referer);
				}

				HttpResponse rsp = session.execute(nextRequest);
//				if(rsp.getStatusLine().getStatusCode() != 302)
//				{
//					rc = new HttpResult(rsp.getStatusLine().getStatusCode(), nextRequest.getURI()
//						.toString());
//					break;
//				}
//				
//				// get the location url
//				URI nextURI = getRedirectURI(context, rsp);
//				nextRequest = session.createHttpGet(nextURI, null, null);
//				rsp = session.execute(nextRequest);
				if(rsp.getStatusLine().getStatusCode() != 200)
				{
					rc = new HttpResult(rsp.getStatusLine().getStatusCode(), nextRequest.getURI()
						.toString());
					break;
				}

				if(!redirectStrategy.isRedirected())
				{
					throw new IllegalStateException("Book doesn't find any redirect.");
				}

				HttpUriRequest finalRequest = redirectStrategy.getFinalRequest();
				
				// save the result page
				HttpClientUtil.saveToFile(rsp.getEntity(), AppUtils.getTempFilePath("BookTicketResult.html"));

				rc = ResultCode.RC_OK;
				result = CONTINUE_PROCESSING;
				
				log.info("Book ticket successfully.");
			}
			while(false);

			// Post actions
			if(result == PROCESSING_COMPLETE)
			{
				context.put(RESULT_KEY, rc);
				log.error("[CommandSelectTicketDate] failed: " + rc);
			}
			return result;
		}
	}
	
	private static class RegInfoVisitor extends NodeVisitor
	{
		private boolean isMeetHR = false;
		private int rowIndex = -1;
		private TicketNumResult regInfo = null;
		
		public RegInfoVisitor()
		{
		}
		
		public void setData(TicketNumResult regInfo, int rowIndex, boolean separator)
		{
			this.regInfo = regInfo;
			this.rowIndex = rowIndex;
			this.isMeetHR = separator;
		}
		
		public void visitTag (Tag tag)
	    {
			if(tag.getTagName().equals("HR"))
			{
				isMeetHR = true;
			}
			else if(tag instanceof LinkTag)
			{
				if(StringUtils.equals(regInfo.prevText,
					AppConfig.getInstance().getPropInternal("zy.available.doctor.name")))
				{
					regInfo.doctorAnchor = (LinkTag)tag;
				}
				else
				{
					regInfo.regAnchor = (LinkTag)tag;
				}
			}
	    }
		
		public void visitStringNode (Text string)
	    {
			String text = StringUtils.strip(string.getText(), " \t\r\n");
			if(StringUtils.isEmpty(text))
			{
				return;
			}
			
			if((rowIndex % 6) == 3)
			{
				regInfo.available = text;
				return;
			}
			
			if(StringUtils.equals(regInfo.prevText, AppConfig.getInstance().getPropInternal("zy.available.doctor.name")))
			{
				regInfo.doctor = text;
			}
			else if(StringUtils.equals(regInfo.prevText, AppConfig.getInstance().getPropInternal("zy.available.hospital.name")))
			{
				regInfo.hospital = text;
			}
			regInfo.prevText = text;
	    }
		
		public boolean isMeetHR()
		{
			return isMeetHR;
		}
	}
	
	private static class TicketNumResult
	{
		public String doctor;
		public LinkTag doctorAnchor;
		public String hospital;
		public String available;
		public LinkTag regAnchor;
		
		public String prevText = null;
		
		public boolean isAvailable()
		{
			if(available == null || regAnchor == null)
			{
				return false;
			}
			if(!available.contains(AppConfig.getInstance().getPropInternal("zy.available.reg.mark")))
			{
				return false;
			}
			
			String aText = StringUtils.strip(regAnchor.getStringText(), " \t\r\n");
			String href = AppConfig.getInstance().getPropInternal("zy.available.link.mark");
			if(!StringUtils.equals(aText, href))
			{
				return false;
			}
			return true;
		}
		
		public String getLink()
		{
			if(isAvailable())
			{
				return regAnchor.getLink();
			}
			return null;
		}
		
		public String toString()
		{
			StringBuilder sb = new StringBuilder();
			sb.append("doctor=").append(doctor);
			sb.append(",hospital=").append(hospital);
			sb.append(",available=").append(available);
			sb.append("(").append(isAvailable()).append(")");
			sb.append(",regAnchor=").append(regAnchor.getLink());
			sb.append(",doctorAnchor=").append(doctorAnchor.getLink());
			return sb.toString();
		}
	}
	
	private static class ConsumerInfo
	{
		public String name;
		
		public String id;
		public String value;
		public String phone;
		public String medical;
		
		public String toString()
		{
			StringBuilder sb = new StringBuilder();
			sb.append("name=").append(name);
			sb.append(",id=").append(id);
			sb.append(",value=").append(value);
			sb.append(",phone=").append(phone);
			sb.append(",medical=").append(medical);
			return sb.toString();
		}
	}

	public static void main(String[] args)
	{
		String user = AppConfig.getInstance().getPropInput("zy.user");
		String pwd = AppConfig.getInstance().getPropInput("zy.password");
		String hos = AppConfig.getInstance().getPropInput("zy.hospital");
		String doctor = AppConfig.getInstance().getPropInput("zy.doctor");
		String dayOfWeek = AppConfig.getInstance().getPropInput("zy.dayOfWeek");

		String msg = String.format("User=[%s], pwd=[%s], hos=[%s], doctor=[%s], dayOfWeek=[%s]",
			user, pwd, hos, doctor, dayOfWeek);
		log.info(msg);

		ZyClient zy = new ZyClient(user, pwd, hos, doctor, dayOfWeek);
		zy.test1();
	}
}
