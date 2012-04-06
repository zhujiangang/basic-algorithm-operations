package com.bao.lc.site.s3.commands;

import com.bao.lc.httpcommand.RetryHttpCommand;

public class BookTicket extends RetryHttpCommand
{
	public BookTicket()
	{
		super(new BookTicketCommandBuilder(), new BookTicketRetryStrategy());
	}
}
