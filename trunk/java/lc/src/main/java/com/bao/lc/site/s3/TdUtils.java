package com.bao.lc.site.s3;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.http.HttpResponse;

import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.site.s3.params.TdPNames;

public class TdUtils
{
	public static String getCharset(HttpResponse rsp, Context context)
	{
		String charsetName = HttpClientUtils.getCharset(rsp);
		if(charsetName == null)
		{
			charsetName = MapUtils.getString(context, TdPNames.PARAM_RSP_ENCODING, "UTF-8");
		}
		return charsetName;
	}
}
