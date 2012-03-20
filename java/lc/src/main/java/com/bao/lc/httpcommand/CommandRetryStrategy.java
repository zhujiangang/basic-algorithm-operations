package com.bao.lc.httpcommand;

import org.apache.commons.chain.Context;

import com.bao.lc.bean.IDValuePair;

public interface CommandRetryStrategy
{
	public boolean shouldRetry(Context context, int executionCount, IDValuePair rc);

	public boolean shouldRetry(Context context, int executionCount, Exception e);
}
