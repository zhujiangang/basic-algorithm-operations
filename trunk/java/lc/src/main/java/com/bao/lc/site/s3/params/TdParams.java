package com.bao.lc.site.s3.params;

import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;

public class TdParams
{
	public static Log getUI(Context context)
	{
		Log uiLog = (Log)context.get(TdPNames.PARAM_UI);
		if(uiLog == null)
		{
			throw new IllegalStateException("User interface log can't be null.");
		}
		return uiLog;
	}
}
