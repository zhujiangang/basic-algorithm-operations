package com.bao.lc.site.s3.commands;

import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.ParseException;
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
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.HttpCommandDirector;
import com.bao.lc.httpcommand.impl.DefaultHttpCommandDirector;
import com.bao.lc.httpcommand.impl.LogCompleteListener;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.TdUtils;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.util.MiscUtils;

public class GetLoginPage extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(GetLoginPage.class);

	private FormTag loginForm = null;
	private ImageTag vCodeImage = null;

	public GetLoginPage()
	{
		super();
		reset();
	}

	private void reset()
	{
		loginForm = null;
		vCodeImage = null;
	}

	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		parse(context);
		return ResultCode.RC_OK;
	}

	private void parse(Context context) throws IOException, ParseException, ParserException
	{
		// 1. parse response
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		String charsetName = TdUtils.getCharset(rsp, context);
		parseResponse(rsp, charsetName);

		// 2. Verification Code
		String vCode = getVerificationCode(context, this.vCodeImage.extractImageLocn());

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
//		location = "/otsweb/loginAction.do?method=login";

		// Create request of the next hop
		RequestBuilder rb = new RequestBuilder();
		rb.method(this.loginForm.getFormMethod()).baseURI(baseURI).reference(location);
		rb.parameters(params).encoding(charsetName);
		HttpUriRequest nextRequest = rb.create();

		// done
		context.put(HttpCommandPNames.TARGET_REQUEST, nextRequest);
		context.put(HttpCommandPNames.TARGET_REFERER, baseURI.toString());
	}

	private void parseResponse(HttpResponse rsp, String charset) throws IOException,
		ParseException, ParserException
	{
		// Get input
		String content = HttpClientUtils.saveToString(rsp.getEntity(), charset);

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

	private String getVerificationCode(Context context, String vCodeLocation)
	{
		// Save the current context
		HttpUriRequest currRequest = HttpCommandParams.getTargetRequest(context);
		String currReferer = MapUtils.getString(context, HttpCommandPNames.TARGET_REFERER);

		URI baseURI = currRequest.getURI();

//		vCodeLocation += "&0.9037692602898029";
		// Create request of the next hop
		RequestBuilder rb = new RequestBuilder();
		rb.baseURI(baseURI).reference(vCodeLocation);
		HttpUriRequest nextRequest = rb.create();

		context.put(HttpCommandPNames.TARGET_REQUEST, nextRequest);
		context.put(HttpCommandPNames.TARGET_REFERER, baseURI.toString());

		// Do execute GetVerificationCode
		HttpCommandDirector director = new DefaultHttpCommandDirector();
		director.execute(new GetVerificationCode(), context, new LogCompleteListener(log));

		// Get the result
		String vCode = MapUtils.getString(context, TdPNames.VERIFICATION_CODE);

		// Restore
		context.put(HttpCommandPNames.TARGET_REQUEST, currRequest);
		context.put(HttpCommandPNames.TARGET_REFERER, currReferer);

		return vCode;
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
}
