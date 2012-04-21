package com.bao.lc.site.s3;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.httpcommand.CommandCompleteListener;
import com.bao.lc.httpcommand.impl.LogCompleteListener;
import com.bao.lc.httpcommand.params.HttpCommandParams;

public class TdWorkCompleteListener implements CommandCompleteListener
{
	private CommandCompleteListener internalListener = null;
	private Log uiLog = null;
	public TdWorkCompleteListener(Log uiLog, Log internalLog)
	{
		this.uiLog = uiLog;
		internalListener = new LogCompleteListener(internalLog);
	}
	@Override
	public void commandComplete(boolean commandResult, IDValuePair rc, Command command,
		Context context)
	{
		internalListener.commandComplete(commandResult, rc, command, context);
		if(rc != ResultCode.RC_OK && rc.getID() == ResultCode.RC_HTTP_ERROR.getID())
		{
			StringBuilder sb = new StringBuilder();
			String commandName = getCommandName(command, context);
			if(commandName != null)
			{
				sb.append("Command[").append(commandName).append("] ");
			}
			sb.append("Failed. Code: ").append(rc);
			uiLog.error(sb.toString());
		}
	}

	@Override
	public void commandException(Exception e, Command command, Context context) throws Exception
	{
		internalListener.commandException(e, command, context);

		StringBuilder sb = new StringBuilder();
		
		String commandName = getCommandName(command, context);
		if(commandName != null)
		{
			sb.append("Command[").append(commandName).append("] ");
		}
		sb.append("Exception");
		Command exceptionCommand = HttpCommandParams.getExceptionCommand(context);
		if(exceptionCommand != null)
		{
			sb.append(" at: ").append(exceptionCommand.getClass().getName());
		}
		uiLog.error(sb.toString(), e);
	}

	private String getCommandName(Command command, Context context)
	{
		while(command instanceof Chain)
		{
			command = HttpCommandParams.getExceptionCommand(context);
		}
		if(command == null)
		{
			return null;
		}
		String fqcn = command.getClass().getName();
		int index = fqcn.lastIndexOf('.');
		if(index >= 0)
		{
			return fqcn.substring(index + 1);
		}
		return fqcn;
	}
}
