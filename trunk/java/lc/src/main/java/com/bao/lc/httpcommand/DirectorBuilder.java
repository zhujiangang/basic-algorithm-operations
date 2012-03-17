package com.bao.lc.httpcommand;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;

import com.bao.lc.httpcommand.impl.DefaultHttpCommandDirector;

public class DirectorBuilder
{
	private Command mMainCommand;
	private Context mMainContext;
	private CommandCompleteListener mMainListener;

	private Command mFinalCommand;
	private Context mFinalContext;
	private CommandCompleteListener mFinalListener;

	public DirectorBuilder mainCommand(Command command)
	{
		this.mMainCommand = command;
		return this;
	}

	public DirectorBuilder mainContext(Context context)
	{
		this.mMainContext = context;
		return this;
	}

	public DirectorBuilder mainListener(CommandCompleteListener listener)
	{
		this.mMainListener = listener;
		return this;
	}

	public DirectorBuilder finalCommand(Command command)
	{
		this.mFinalCommand = command;
		return this;
	}

	public DirectorBuilder finalContext(Context context)
	{
		this.mFinalContext = context;
		return this;
	}

	public DirectorBuilder finalListener(CommandCompleteListener listener)
	{
		this.mFinalListener = listener;
		return this;
	}

	public void execute()
	{
		if((mFinalCommand != null && mFinalContext == null)
			|| (mFinalCommand == null && mFinalContext != null))
		{
			throw new IllegalStateException(
				"The state of final command and context is inconsistent.");
		}

		HttpCommandDirector director = new DefaultHttpCommandDirector();
		if(mFinalCommand == null)
		{
			director.execute(mMainCommand, mMainContext, mMainListener);
		}
		else
		{
			director.execute(mMainCommand, mMainContext, mMainListener, mFinalCommand,
				mFinalContext, mFinalListener);
		}
	}
}
