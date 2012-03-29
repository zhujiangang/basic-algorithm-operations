package com.bao.lc.httpcommand;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;

public interface HttpCommandDirector
{
	public void execute(Command command, Context context, CommandCompleteListener listener) throws Exception;

	public void execute(Command mainCommand, Context mainContext,
		CommandCompleteListener mainListener, Command finalCommand, Context finalContext,
		CommandCompleteListener finalListener) throws Exception;
}
