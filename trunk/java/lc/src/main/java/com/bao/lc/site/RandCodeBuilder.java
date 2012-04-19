package com.bao.lc.site;

import java.awt.Component;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ContextBase;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpUriRequest;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.common.Builder;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.util.AppUtils;
import com.bao.lc.util.MiscUtils;

public class RandCodeBuilder implements Builder<String>
{
	private static Log log = LogFactory.getLog(RandCodeBuilder.class);

	private static final String RAND_CODE_RESULT = "lc.rand.code.result";

	// Required
	private Context origContext = null;
	private HttpUriRequest req = null;

	// Optional
	private String referer = null;
	private String prompt = null;
	private Component parentComponent;

	public RandCodeBuilder()
	{
		reset();
	}

	private void reset()
	{
		origContext = null;
		req = null;
		referer = null;
		prompt = null;
		parentComponent = null;
	}

	public RandCodeBuilder context(Context context)
	{
		this.origContext = context;
		return this;
	}

	public RandCodeBuilder request(HttpUriRequest request)
	{
		this.req = request;
		return this;
	}

	public RandCodeBuilder referer(String referer)
	{
		this.referer = referer;
		return this;
	}

	public RandCodeBuilder prompt(String str)
	{
		this.prompt = str;
		return this;
	}

	public RandCodeBuilder parent(Component parentComponent)
	{
		this.parentComponent = parentComponent;
		return this;
	}

	@Override
	public String build()
	{
		String vCode = null;
		try
		{
			//Check parameters
			if(this.req == null)
			{
				throw new IllegalArgumentException("'request' can't be null");
			}

			// Prepare context
			Context context = new ContextBase();
			context.putAll(this.origContext);
			context.put(HttpCommandPNames.TARGET_REQUEST, this.req);
			if(referer != null)
			{
				context.put(HttpCommandPNames.TARGET_REFERER, this.referer);
			}

			// Execute
			vCode = executeCommand(context);

			// Remove the copied items to let GC happy
			context.clear();
		}
		finally
		{
			reset();
		}
		return vCode;
	}

	private String executeCommand(Context context)
	{
		// Do execute GetVerificationCode
		Command childCommand = new GetVerificationCode();
		try
		{
			childCommand.execute(context);
		}
		catch(Exception e)
		{
			log.error("Failed to execute command@uri=" + req.getURI(), e);
			return null;
		}

		// Get the result
		String vCode = MapUtils.getString(context, RAND_CODE_RESULT);
		return vCode;
	}

	private class GetVerificationCode extends BasicHttpCommand
	{
		@Override
		protected IDValuePair postExecute(Context context) throws Exception
		{
			// 1. Get response content
			HttpResponse rsp = HttpCommandParams.getResponse(context);

			String fileName = AppUtils.getTempFilePath("vode.jpg");
			HttpClientUtils.saveToFile(rsp.getEntity(), fileName);

			// show validation code
			String validationCode = MiscUtils.getValidationCode(fileName, prompt, parentComponent);
			context.put(RAND_CODE_RESULT, validationCode);

			// Remove next hop
			HttpCommandParams.purgeRequestParams(context);

			return ResultCode.RC_OK;
		}
	}
}
