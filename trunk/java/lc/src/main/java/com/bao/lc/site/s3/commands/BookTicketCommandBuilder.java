package com.bao.lc.site.s3.commands;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.commons.chain.impl.ChainBase;
import org.apache.commons.collections.MapUtils;

import com.bao.lc.httpcommand.CommandBuilder;
import com.bao.lc.site.s3.params.TdPNames;

public class BookTicketCommandBuilder implements CommandBuilder
{

	@Override
	public Command build(Context context, int execCount)
	{
		Chain chain = new ChainBase();

		String content = MapUtils.getString(context, TdPNames._CONFIRM_PASSENGER_CONTENT);
		if(execCount <= 1 || content == null)
		{
			chain.addCommand(new QueryLeftTicket());
			chain.addCommand(new SubmitOrder());
		}
		else
		{
			chain.addCommand(new ParseSubmitOrderPage());
		}
		chain.addCommand(new ConfirmOrder());

		return chain;
	}

}
