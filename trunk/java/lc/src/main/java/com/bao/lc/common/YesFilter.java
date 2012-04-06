package com.bao.lc.common;

import org.htmlparser.Node;
import org.htmlparser.NodeFilter;

@SuppressWarnings("serial")
public class YesFilter implements NodeFilter
{

	@Override
	public boolean accept(Node node)
	{
		return true;
	}

}
