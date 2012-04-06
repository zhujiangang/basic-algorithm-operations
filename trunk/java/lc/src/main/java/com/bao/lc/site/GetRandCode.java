package com.bao.lc.site;

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
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.util.AppUtils;
import com.bao.lc.util.MiscUtils;

public class GetRandCode
{
	private static Log log = LogFactory.getLog(GetRandCode.class);
	
	private static final String RAND_CODE_RESULT = "lc.rand.code.result___";

	private Context origContext = null;
	private HttpUriRequest req = null;
	private String referer = null;

	public GetRandCode(Context context, HttpUriRequest req)
	{
		this(context, req, null);
	}
	public GetRandCode(Context context, HttpUriRequest req, String referer)
	{
		this.origContext = context;
		this.req = req;
		this.referer = referer;
	}
	
	public void setRequest(HttpUriRequest req)
	{
		this.req = req;
	}
	public void setReferer(String referer)
	{
		this.referer = referer;
	}

	public String get()
	{
		if(this.req == null)
		{
			throw new IllegalArgumentException("request can't be null");
		}
		
		Context context = new ContextBase();
		context.putAll(this.origContext);

		context.put(HttpCommandPNames.TARGET_REQUEST, this.req);
		if(referer != null)
		{
			context.put(HttpCommandPNames.TARGET_REFERER, this.referer);
		}

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
		
		//Remove next hop
		context.clear();

		return vCode;
	}

	private static class GetVerificationCode extends BasicHttpCommand
	{
		@Override
		protected IDValuePair postExecute(Context context) throws Exception
		{
			// 1. Get response content
			HttpResponse rsp = HttpCommandParams.getResponse(context);

			String fileName = AppUtils.getTempFilePath("vode.jpg");
			HttpClientUtils.saveToFile(rsp.getEntity(), fileName);

			// show validation code
			String validationCode = MiscUtils.getValidationCode(fileName);
			context.put(RAND_CODE_RESULT, validationCode);

			// Remove next hop
			HttpCommandParams.purgeRequestParams(context);

			return ResultCode.RC_OK;
		}
	}
}
