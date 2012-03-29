package com.bao.lc.httpcommand;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.protocol.HttpContext;

import com.bao.lc.bean.HttpResult;
import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.params.MiscParams;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.util.AppUtils;

public class BasicHttpCommand extends AbstractCommand
{
	private Log log = LogFactory.getLog(getClass());

	public BasicHttpCommand()
	{
	}

	// ----------------------------------------------------------- Methods

	@SuppressWarnings("unchecked")
	@Override
	public boolean execute(Context context) throws Exception
	{
		boolean result = super.execute(context);

		// Log useful informations
		log.info(getResultLog(context, result));

		return result;
	}

	protected IDValuePair preExecute(Context context) throws Exception
	{
		// Remove the response if it exists
		HttpCommandParams.purgeResponseParams(context);

		return ResultCode.RC_OK;
	}

	protected IDValuePair doExecute(Context context) throws Exception
	{
		HttpClient httpClient = HttpCommandParams.getHttpClient(context);
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
				MiscParams.setReferer(request.getParams(), referer);
			}

			if(log.isDebugEnabled())
			{
				log.debug("[Execute]: URI=" + request.getURI() + ", referer=" + referer);
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

		HttpCommandParams.purgeRequestParams(context);

		return ResultCode.RC_OK;
	}

	private String getResultLog(Context context, boolean result)
	{
		StringBuilder sb = new StringBuilder();
		
		//1. Command Result
		sb.append("[Result]: ").append(result);

		//2. Application Result Code
		IDValuePair rc = HttpCommandParams.getResultCode(context);
		sb.append(", rc=").append(rc);
		
		//explain the request
		if(rc == ResultCode.RC_OK)
		{
			sb.append(", NextHop=");
		}
		else
		{
			sb.append(", CurrHop=");
		}

		//Request
		HttpUriRequest request = HttpCommandParams.getTargetRequest(context);
		if(request != null)
		{
			sb.append(request.getMethod()).append(" ").append(request.getURI());
		}
		else
		{
			sb.append("null");
		}

		//Append the referrer if it is OK 
		if(rc == ResultCode.RC_OK)
		{
			sb.append(", Referer=");
			sb.append(MapUtils.getString(context, HttpCommandPNames.TARGET_REFERER));
		}

		return sb.toString();
	}
}
