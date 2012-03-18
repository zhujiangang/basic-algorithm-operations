package com.bao.lc.httpcommand;

import org.apache.commons.chain.Context;
import org.apache.commons.chain.Filter;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.bean.HttpResult;
import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.BrowserClient;
import com.bao.lc.client.params.MiscParams;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.util.AppUtils;

public class BasicHttpCommand implements Filter
{
	private Log log = LogFactory.getLog(getClass());

	public BasicHttpCommand()
	{
	}

	// ----------------------------------------------------------- Methods
	@Override
	public boolean postprocess(Context context, Exception exception)
	{
		//save the exception command
		if(!context.containsKey(HttpCommandPNames.EXCEPTION_COMMAND))
		{
			context.put(HttpCommandPNames.EXCEPTION_COMMAND, this);
		}
		
		return false;
	}
	
	@SuppressWarnings("unchecked")
	@Override
	public boolean execute(Context context) throws Exception
	{
		// Default: complete the current command
		boolean result = PROCESSING_COMPLETE;

		IDValuePair rc = ResultCode.RC_OK;

		do
		{
			// 0. Pre Execute
			rc = preExecute(context);
			if(rc != ResultCode.RC_OK)
			{
				break;
			}

			// 1. Do Execute
			rc = doExecute(context);
			if(rc != ResultCode.RC_OK)
			{
				break;
			}

			// -1. Post Execute
			rc = postExecute(context);
			if(rc != ResultCode.RC_OK)
			{
				break;
			}

			// Done, everything is OK
			rc = ResultCode.RC_OK;
			result = CONTINUE_PROCESSING;
		}
		while(false);

		// Save and log(if needed) the command result
		context.put(HttpCommandPNames.HTTP_COMMAND_RESULT_CODE, rc);

		if(rc != ResultCode.RC_OK)
		{
			log.info(getResultLog(rc, HttpCommandParams.getTargetRequest(context)));
		}
		if(log.isDebugEnabled())
		{
			StringBuilder sb = new StringBuilder();
			sb.append("Next hop: URI=");
			sb.append(HttpCommandParams.getTargetRequestURI(context));
			sb.append(", Referer=");
			sb.append(MapUtils.getString(context, HttpCommandPNames.TARGET_REFERER));
			log.debug(sb.toString());
		}

		return result;
	}

	protected IDValuePair preExecute(Context context) throws Exception
	{
		// Remove the response if it exists
		context.remove(HttpCommandPNames.TARGET_RESPONSE);
		context.remove(HttpCommandPNames.HTTP_COMMAND_RESULT_CODE);

		return ResultCode.RC_OK;
	}

	protected IDValuePair doExecute(Context context) throws Exception
	{
		BrowserClient httpClient = HttpCommandParams.getBrowserClient(context);
		HttpContext httpContext = HttpCommandParams.getHttpContext(context);

		IDValuePair rc = ResultCode.RC_OK;

		do
		{
			// 1. Check the target UriRequest exists?
			HttpUriRequest request = HttpCommandParams.getTargetRequest(context);
			if(request == null)
			{
				rc = ResultCode.RC_NULL_TARGET_URI_REQUEST;
				break;
			}

			// 2. Check the referrer exists?
			String referer = (String) context.get(HttpCommandPNames.TARGET_REFERER);
			if(referer != null)
			{
				MiscParams.setReferer(httpClient.getParams(), referer);
			}

			// 3. Execute the URI request
			HttpResponse rsp = httpClient.execute(request, httpContext);
			int statusCode = rsp.getStatusLine().getStatusCode();
			if(statusCode != 200)
			{
				rc = new HttpResult(statusCode, request.getURI().toString());

				// Save the result file so make the content to be consumed
				String file = AppUtils.getTempFilePath("HttpErrorResult_" + statusCode + ".html");
				HttpClientUtils.saveToFile(rsp.getEntity(), file);
				break;
			}

			// Done
			context.put(HttpCommandPNames.TARGET_RESPONSE, rsp);
		}
		while(false);

		return rc;
	}

	protected IDValuePair postExecute(Context context) throws Exception
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);

		// Save the result file so make the content to be consumed
		String file = AppUtils.getTempFilePath("HttpResult_200.html");
		HttpClientUtils.saveToFile(rsp.getEntity(), file);

		context.remove(HttpCommandPNames.TARGET_REQUEST);
		context.remove(HttpCommandPNames.TARGET_REFERER);

		return ResultCode.RC_OK;
	}

	private String getResultLog(IDValuePair rc, HttpUriRequest request)
	{
		StringBuilder sb = new StringBuilder();
		sb.append("rc=").append(rc);
		sb.append(", ").append(request.getMethod());
		sb.append(" ").append(request.getURI().toString());
		return sb.toString();
	}
}
