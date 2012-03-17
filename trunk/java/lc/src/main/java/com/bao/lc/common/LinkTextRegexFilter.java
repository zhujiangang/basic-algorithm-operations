package com.bao.lc.common;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.htmlparser.Node;
import org.htmlparser.NodeFilter;
import org.htmlparser.tags.LinkTag;

/**
 * This class accepts tags of class LinkTag that contain a text matching a given
 * regex pattern. Use this filter to extract LinkTag nodes with text that match
 * the desired regex pattern.
 */

@SuppressWarnings("serial")
public class LinkTextRegexFilter implements NodeFilter
{
	/**
     * The regular expression to use on the link.
     */
    protected Pattern mRegex;

    /**
     * Creates a LinkRegexFilter that accepts LinkTag nodes containing
     * a text that matches the supplied regex pattern.
     * The match is case insensitive.
     * @param regexPattern The pattern to match.
     */
    public LinkTextRegexFilter (String regexPattern)
    {
        this (regexPattern, true);
    }

    /**
     * Creates a LinkRegexFilter that accepts LinkTag nodes containing
     * a text that matches the supplied regex pattern.
     * @param regexPattern The regex pattern to match.
     * @param caseSensitive Specifies case sensitivity for the matching process.
     */
    public LinkTextRegexFilter (String regexPattern, boolean caseSensitive)
    {
        if (caseSensitive)
            mRegex = Pattern.compile (regexPattern);
        else
            mRegex = Pattern.compile (regexPattern,
                Pattern.CASE_INSENSITIVE | Pattern.UNICODE_CASE);
    }

    /**
     * Accept nodes that are a LinkTag and have a text
     * that matches the regex pattern supplied in the constructor.
     * @param node The node to check.
     * @return <code>true</code> if the node is a link with the pattern.
     */
    @Override
    public boolean accept (Node node)
    {
        boolean ret = false;
        if(LinkTag.class.isAssignableFrom (node.getClass ()))
        {
            String linkText = ((LinkTag)node).getLinkText();
            Matcher matcher = mRegex.matcher(linkText);
            ret = matcher.find ();
        }
        return (ret);
    }
}
