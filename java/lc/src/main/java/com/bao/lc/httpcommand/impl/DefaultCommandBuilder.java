package com.bao.lc.httpcommand.impl;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;

import com.bao.lc.httpcommand.CommandBuilder;

public class DefaultCommandBuilder implements CommandBuilder
{
	private Command immutableCommand = null;
	
	public DefaultCommandBuilder(Command command)
	{
		this.immutableCommand = command;
	}
	@Override
	public Command build(Context context, int execCount)
	{
		return this.immutableCommand;
	}

}
