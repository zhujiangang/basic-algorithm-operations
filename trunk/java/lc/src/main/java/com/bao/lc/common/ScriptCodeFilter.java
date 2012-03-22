package com.bao.lc.common;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.htmlparser.Node;
import org.htmlparser.NodeFilter;
import org.htmlparser.tags.ScriptTag;

@SuppressWarnings("serial")
public class ScriptCodeFilter implements NodeFilter
{
	/**
	 * The regular expression to use on the link.
	 */
	protected Pattern mRegex;

	public ScriptCodeFilter(String regexPattern)
	{
		this(regexPattern, 0);
	}

	public ScriptCodeFilter(String regexPattern, int flags)
	{
		mRegex = Pattern.compile(regexPattern, flags);
	}

	public boolean accept(Node node)
	{
		boolean ret = false;

		if(ScriptTag.class.isAssignableFrom(node.getClass()))
		{
			String scriptCode = ((ScriptTag) node).getScriptCode();
			if(scriptCode != null && !scriptCode.isEmpty())
			{
				scriptCode = scriptCode.trim();

				Matcher matcher = mRegex.matcher(scriptCode);
				ret = matcher.find();
			}
		}

		return (ret);
	}
}
