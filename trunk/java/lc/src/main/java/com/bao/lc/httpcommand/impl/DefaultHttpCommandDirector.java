package com.bao.lc.httpcommand.impl;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;

import com.bao.lc.httpcommand.CommandCompleteListener;
import com.bao.lc.httpcommand.HttpCommandDirector;
import com.bao.lc.httpcommand.params.HttpCommandParams;

public class DefaultHttpCommandDirector implements HttpCommandDirector
{
	@Override
	public void execute(Command command, Context context, CommandCompleteListener listener) throws Exception
	{
		try
		{
			// Command result
			boolean commandResult = command.execute(context);

			if(listener != null)
			{
				listener.commandComplete(commandResult, HttpCommandParams.getResultCode(context),
					command, context);
			}
		}
		catch(Exception e)
		{
			if(listener != null)
			{
				listener.commandException(e, command, context);
			}
		}
	}

	@Override
	public void execute(Command mainCommand, Context mainContext,
		CommandCompleteListener mainListener, Command finalCommand, Context finalContext,
		CommandCompleteListener finalListener) throws Exception
	{
		try
		{
			execute(mainCommand, mainContext, mainListener);
		}
		finally
		{
			execute(finalCommand, finalContext, finalListener);
		}
	}
}
