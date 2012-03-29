package com.bao.lc.httpcommand;

import org.apache.commons.chain.Context;
import org.apache.commons.chain.Filter;
import org.apache.commons.chain.Command;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.httpcommand.impl.DefaultCommandBuilder;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;

public class RetryHttpCommand implements Filter
{
	private static Log log = LogFactory.getLog(RetryHttpCommand.class);
	
	/*Input parameters*/
	private CommandBuilder commandBuilder = null;
	private CommandRetryStrategy retryStrategy = null;
	
	/*Cache the last generated command for method "postprocess" usage*/
	private Command command = null;

	public RetryHttpCommand(CommandBuilder commandBuilder, CommandRetryStrategy retryStrategy)
	{
		if(commandBuilder == null)
		{
			throw new IllegalArgumentException("commandBuilder can't be null");
		}
		this.commandBuilder = commandBuilder;
		this.retryStrategy = retryStrategy;
	}
	public RetryHttpCommand(Command command, CommandRetryStrategy retryStrategy)
	{
		this(new DefaultCommandBuilder(command), retryStrategy);
	}

	@Override
	public boolean execute(Context context) throws Exception
	{
		boolean result = Command.PROCESSING_COMPLETE;

		int execCount = 0;

		while(true)
		{
			execCount++;
			try
			{
				log.debug("Attempt to execute: " + execCount);

				//Get the command
				command = commandBuilder.build(context, execCount);
				result = command.execute(context);

				IDValuePair rc = HttpCommandParams.getResultCode(context);
				//Get out of the loop in 2 cases:
				//1. Success
				//2. Fail but RetryStrategy tell us to retry
				if(rc == ResultCode.RC_OK || !retryStrategy.shouldRetry(context, execCount, rc))
				{
					break;
				}
			}
			catch(Exception e)
			{
				if(!retryStrategy.shouldRetry(context, execCount, e))
				{
					throw e;
				}
			}
		}
		return result;
	}

	@Override
	public boolean postprocess(Context context, Exception exception)
	{
		if(command instanceof Filter)
		{
			return ((Filter) command).postprocess(context, exception);
		}

		// save the exception command
		if(!context.containsKey(HttpCommandPNames.EXCEPTION_COMMAND))
		{
			context.put(HttpCommandPNames.EXCEPTION_COMMAND, this);
		}

		return false;
	}

}
