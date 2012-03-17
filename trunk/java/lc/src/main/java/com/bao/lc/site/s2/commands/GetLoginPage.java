package com.bao.lc.site.s2.commands;

import java.net.URI;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.utils.URIUtils;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.lc.common.IDValuePair;
import com.bao.lc.common.ResultCode;
import com.bao.lc.httpcommand.DefaultHttpCommand;
import com.bao.lc.httpcommand.HttpCommandPNames;
import com.bao.lc.httpcommand.HttpCommandParams;
import com.bao.lc.site.s2.ZyContants;
import com.bao.lc.util.CommonUtil;
import com.bao.lc.util.HttpClientUtil;

public class GetLoginPage extends DefaultHttpCommand
{
	private static Log log = LogFactory.getLog(GetLoginPage.class);

	public GetLoginPage()
	{
		super();
	}

	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		
		parse(context, rsp);
		return ResultCode.RC_OK;
	}

	@SuppressWarnings("unchecked")
	private void parse(Context context, HttpResponse rsp) throws Exception
	{
		String encoding = "UTF-8";
		String text = HttpClientUtil.saveToString(rsp.getEntity(), encoding);

		// Parse
		Parser parser = CommonUtil.createParser(text, encoding, log);
		// Set filters
		NodeFilter[] predicates = { new HasAttributeFilter("id", "Form1"),
			new HasAttributeFilter("name", "Form1"), new NodeClassFilter(FormTag.class) };
		NodeFilter filter = new AndFilter(predicates);

		// parse
		NodeList formNodeList = parser.parse(filter);
		if(formNodeList.size() != 1)
		{
			throw new ParserException("Found [" + formNodeList.size() + "] form(s), Expected [1].");
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
				params.put(name, MapUtils.getString(context, ZyContants.PARAM_USER));
				continue;
			}

			if("password".equals(name))
			{
				params.put(name, MapUtils.getString(context, ZyContants.PARAM_PASSWORD));
				continue;
			}

			// collect all the other fields.
			params.put(name, value);
		}

		// Method
		String method = form.getFormMethod();

		// Location
		String location = buildLocation("card", params.get("redirctTo"));
		URI baseURI = HttpCommandParams.getTargetRequestURI(context);
		URI nextURI = URIUtils.resolve(baseURI, location);

		HttpUriRequest nextRequest = HttpClientUtil.createRequest(nextURI, method, params, encoding,
			null);

		context.put(HttpCommandPNames.TARGET_REQUEST, nextRequest);
		context.put(HttpCommandPNames.TARGET_REFERER, baseURI.toString());
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
