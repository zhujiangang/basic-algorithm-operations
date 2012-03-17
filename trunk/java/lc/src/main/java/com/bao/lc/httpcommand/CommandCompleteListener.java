package com.bao.lc.httpcommand;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;

import com.bao.lc.common.IDValuePair;

public interface CommandCompleteListener
{
	public void commandComplete(boolean commandResult, IDValuePair rc, Command command,
		Context context);

	public void commandException(Exception e, Command command, Context context);
}
