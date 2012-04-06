package com.bao.lc.site.s3.commands;

import java.net.URI;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.client.methods.HttpUriRequest;
import org.htmlparser.Node;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.filters.OrFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.ImageTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.RequestBuilder;
import com.bao.lc.httpcommand.AbstractCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.GetRandCode;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.util.MiscUtils;

public class ParseLoginPage extends AbstractCommand
{
	private static Log log = LogFactory.getLog(ParseLoginPage.class);

	private FormTag loginForm = null;
	private ImageTag vCodeImage = null;

	public ParseLoginPage()
	{
		reset();
	}

	private void reset()
	{
		loginForm = null;
		vCodeImage = null;
	}

	@Override
	protected IDValuePair preExecute(Context context) throws Exception
	{
		return ResultCode.RC_OK;
	}
	
	@Override
	protected IDValuePair doExecute(Context context) throws Exception
	{
		return ResultCode.RC_OK;
	}
	
	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		String content = MapUtils.getString(context, TdPNames._LOGIN_PAGE_CONTENT);
		if(content == null)
		{
			log.error("content is null.");
			return ResultCode.RC_NULL_PARAMETER;
		}

		String encoding = MapUtils.getString(context, TdPNames._LOGIN_PAGE_ENCODING);
		if(encoding == null)
		{
			log.error("encoding is null.");
			return ResultCode.RC_NULL_PARAMETER;
		}

		// 1. parse content
		parseContent(context, content, encoding);
		
		// 2. Verification Code
		String vCode = MapUtils.getString(context, TdPNames._LOGIN_VOCDE);
		if(vCode == null)
		{
			vCode = getVerificationCode(context, this.vCodeImage.extractImageLocn());
		}

		// 3. Get all input parameters
		Map<String, String> params = new HashMap<String, String>();
		getInputParameters(context, vCode, this.loginForm, params);

		// 4. create next hop
		String location = this.loginForm.getFormLocation();
		URI baseURI = HttpCommandParams.getTargetRequestURI(context);
		if(baseURI == null)
		{
			throw new ParserException("Assert failure. baseURI is null.");
		}

		// Create request of the next hop
		RequestBuilder rb = new RequestBuilder();
		rb.method(this.loginForm.getFormMethod()).baseURI(baseURI).reference(location);
		rb.paramMap(params).encoding(encoding);
		HttpUriRequest nextRequest = rb.create();

		// done
		context.put(HttpCommandPNames.TARGET_REQUEST, nextRequest);
		context.put(HttpCommandPNames.TARGET_REFERER, baseURI.toString());

		return ResultCode.RC_OK;
	}

	protected void parseContent(Context context, String content, String charset)
		throws ParserException
	{
		// 2. Create parser to parse the content
		Parser parser = MiscUtils.createParser(content, charset, log);

		// Set filters
		List<NodeFilter> predicates = new ArrayList<NodeFilter>(3);
		NodeFilter[] a = new NodeFilter[0];

		// 1. login form filters
		predicates.add(new HasAttributeFilter("id", "loginForm"));
		predicates.add(new HasAttributeFilter("name", "loginForm"));
		predicates.add(new NodeClassFilter(FormTag.class));
		NodeFilter loginFormFilter = new AndFilter(predicates.toArray(a));

		// 2. verification code source filters
		predicates.clear();
		predicates.add(new HasAttributeFilter("id", "img_rrand_code"));
		predicates.add(new NodeClassFilter(ImageTag.class));
		NodeFilter vCodeSrcFilter = new AndFilter(predicates.toArray(a));

		predicates.clear();
		NodeFilter finalFilter = new OrFilter(loginFormFilter, vCodeSrcFilter);

		// parse
		NodeList nodeList = parser.parse(finalFilter);

		// Validation
		for(int i = 0, size = nodeList.size(); i < size; i++)
		{
			Node node = nodeList.elementAt(i);
			if(node instanceof FormTag)
			{
				this.loginForm = (FormTag) node;
			}
			else if(node instanceof ImageTag)
			{
				this.vCodeImage = (ImageTag) node;
			}
		}
		if(this.loginForm == null || this.vCodeImage == null)
		{
			throw new ParserException("loginForm or(and) vCodeImage doesn't exist. loginForm="
				+ this.loginForm + ", vCodeImage=" + this.vCodeImage);
		}
	}

	private void getInputParameters(Context context, String vCode, FormTag loginForm,
		Map<String, String> params)
	{
		NodeList inputList = loginForm.getFormInputs();
		for(int i = 0, size = inputList.size(); i < size; i++)
		{
			InputTag input = (InputTag) inputList.elementAt(i);
			String name = input.getAttribute("name");
			String value = input.getAttribute("value");
			if(name.equals("loginUser.user_name"))
			{
				value = MapUtils.getString(context, TdPNames.PARAM_USER);
			}
			else if(name.equals("user.password"))
			{
				value = MapUtils.getString(context, TdPNames.PARAM_PASSWORD);
			}
			else if(name.equals("randCode"))
			{
				value = vCode;
			}
			params.put(name, value);
		}
	}
	
	private String getVerificationCode(Context context, String vCodeLocation) throws Exception
	{
		URI baseURI = HttpCommandParams.getTargetRequestURI(context);
		boolean isFirstLogin = MapUtils.getBooleanValue(context, TdPNames._IS_FIRST_LOGIN, true);
		if(!isFirstLogin)
		{
			vCodeLocation += "&" + MiscUtils.randJS();
		}
		// Create request of the next hop
		RequestBuilder rb = new RequestBuilder();
		rb.baseURI(baseURI).reference(vCodeLocation);
		HttpUriRequest nextRequest = rb.create();

		// Do execute GetVerificationCode
		GetRandCode randCode = new GetRandCode(context, nextRequest, baseURI.toString());
		// Get the result
		String vCode = randCode.get();
		
		context.put(TdPNames._LOGIN_VOCDE, vCode);

		return vCode;
	}
}
