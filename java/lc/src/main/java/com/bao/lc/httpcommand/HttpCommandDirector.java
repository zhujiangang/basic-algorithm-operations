package com.bao.lc.httpcommand;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;

public interface HttpCommandDirector
{
	public void execute(Command command, Context context, CommandCompleteListener listener);

	public void execute(Command mainCommand, Context mainContext,
		CommandCompleteListener mainListener, Command finalCommand, Context finalContext,
		CommandCompleteListener finalListener);
}
