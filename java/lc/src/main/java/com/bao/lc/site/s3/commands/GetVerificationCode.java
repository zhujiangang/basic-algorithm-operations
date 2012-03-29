package com.bao.lc.site.s3.commands;

import org.apache.commons.chain.Context;
import org.apache.http.HttpResponse;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.util.AppUtils;
import com.bao.lc.util.MiscUtils;

public class GetVerificationCode extends BasicHttpCommand
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
		context.put(TdPNames.PARAM_LOGIN_VCODE, validationCode);
		
		//Remove next hop
		HttpCommandParams.purgeRequestParams(context);

		return ResultCode.RC_OK;
	}
}
