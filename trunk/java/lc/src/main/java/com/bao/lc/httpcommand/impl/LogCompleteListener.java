package com.bao.lc.httpcommand.impl;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.httpcommand.CommandCompleteListener;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.httpcommand.utils.HttpCommandUtils;

public class LogCompleteListener implements CommandCompleteListener
{
	private static Log classLog = LogFactory.getLog(LogCompleteListener.class);
	
	private Log log = null;
	
	public LogCompleteListener(Log log)
	{
		this.log = log;
		if(this.log == null)
		{
			this.log = classLog;
		}
	}
	public LogCompleteListener()
	{
		this(null);
	}

	@Override
	public void commandComplete(boolean commandResult, IDValuePair rc, Command command,
		Context context)
	{
		StringBuilder sb = new StringBuilder();
		sb.append("Command[").append(command.getClass().getName()).append("] Result: ");
		sb.append(HttpCommandUtils.getCommandResultText(commandResult));
		sb.append(", Code: ").append(rc);
		log.info(sb.toString());
	}

	@Override
	public void commandException(Exception e, Command command, Context context) throws Exception
	{
		StringBuilder sb = new StringBuilder();
		sb.append("Command[").append(command.getClass().getName()).append("] Exception");
		Command exceptionCommand = HttpCommandParams.getExceptionCommand(context);
		if(exceptionCommand != null)
		{
			sb.append(" at: ").append(exceptionCommand.getClass().getName());
		}
		log.error(sb.toString(), e);
	}

}
