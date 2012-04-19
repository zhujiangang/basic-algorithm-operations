package com.bao.lc.site.s3.gui;

import javax.swing.JTextArea;

import org.apache.log4j.AppenderSkeleton;
import org.apache.log4j.PatternLayout;
import org.apache.log4j.spi.LoggingEvent;

public class TextAreaAppender extends AppenderSkeleton
{
	protected JTextArea textArea = null;
	
	public TextAreaAppender(JTextArea textArea)
	{
		super();
		
		this.name = "TdAppender";
		this.closed = false;
		this.layout = new PatternLayout("[%d][%t][%p] - %m%n");
		
		this.textArea = textArea;
	}
	@Override
	public void close()
	{
		//Do nothing
		this.closed = true;
	}

	@Override
	public boolean requiresLayout()
	{
		return true;
	}

	@Override
	protected void append(LoggingEvent event)
	{
		String logRecord = this.layout.format(event);
	
		textArea.append(logRecord);
		textArea.setCaretPosition(textArea.getText().length());
	}
}
