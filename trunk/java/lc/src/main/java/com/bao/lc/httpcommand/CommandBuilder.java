package com.bao.lc.httpcommand;

import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;

public interface CommandBuilder
{
	public Command build(Context context, int execCount);
}
