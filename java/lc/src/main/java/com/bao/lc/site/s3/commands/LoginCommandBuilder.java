package com.bao.lc.site.s3.commands;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ChainBase;
import org.apache.commons.collections.MapUtils;

import com.bao.lc.httpcommand.CommandBuilder;
import com.bao.lc.site.s3.params.TdPNames;

public class LoginCommandBuilder implements CommandBuilder
{

	@Override
	public Command build(Context context, int execCount)
	{
		Chain chain = new ChainBase();

		String content = MapUtils.getString(context, TdPNames._LOGIN_PAGE_CONTENT);
		if(execCount <= 1 || content == null)
		{
			chain.addCommand(new GetLoginPage());
		}
		else
		{
			chain.addCommand(new ParseLoginPage());
		}
		chain.addCommand(new DoLogin());

		return chain;
	}

}
