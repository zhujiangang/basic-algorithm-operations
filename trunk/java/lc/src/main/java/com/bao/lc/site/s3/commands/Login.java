package com.bao.lc.site.s3.commands;

import com.bao.lc.httpcommand.RetryHttpCommand;

public class Login extends RetryHttpCommand
{
	public Login()
	{
		super(new LoginCommandBuilder(), new LoginRetryStrategy());
	}
}
