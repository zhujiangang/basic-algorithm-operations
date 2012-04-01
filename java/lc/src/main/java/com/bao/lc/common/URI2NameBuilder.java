package com.bao.lc.common;

import java.net.URI;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.http.NameValuePair;
import org.apache.http.client.utils.URLEncodedUtils;

public class URI2NameBuilder implements Builder<String>
{
	private static final String DEFAULT_DELIM = "_";

	private URI uri;
	private String uriString;

	private Set<String> paramNameSet = null;
	private String encoding = null;

	private String delim = null;
	
	private boolean autoClear = true;

	public URI2NameBuilder()
	{
		reset();
	}

	private void reset()
	{
		if(autoClear)
		{
			clear();
		}
		uri = null;
		uriString = null;

		paramNameSet = null;
		encoding = "UTF-8";

		delim = DEFAULT_DELIM;
		autoClear = true;
	}

	private URI getURI()
	{
		if(uri != null)
		{
			return uri;
		}
		if(uriString != null)
		{
			return URI.create(uriString);
		}
		return null;
	}

	public URI2NameBuilder uri(URI uri)
	{
		this.uri = uri;
		return this;
	}

	public URI2NameBuilder uriStr(String uriString)
	{
		this.uriString = uriString;
		return this;
	}

	public URI2NameBuilder encoding(String encoding)
	{
		this.encoding = encoding;
		return this;
	}

	public URI2NameBuilder delim(String delim)
	{
		this.delim = delim;
		return this;
	}

	public URI2NameBuilder addParamName(String paramName)
	{
		if(paramNameSet == null)
		{
			paramNameSet = new HashSet<String>();
		}
		paramNameSet.add(paramName);
		return this;
	}

	public void clear()
	{
		if(paramNameSet != null)
		{
			paramNameSet.clear();
		}
	}

	public String build()
	{
		String result = null;
		try
		{
			URI targetURI = getURI();
			if(targetURI == null)
			{
				throw new IllegalArgumentException("Target URI is null");
			}

			StringBuilder sb = new StringBuilder();

			// Path part
			String rawPath = targetURI.getRawPath();
			if(rawPath != null && !rawPath.isEmpty())
			{
				int lastSlashIndex = rawPath.lastIndexOf('/');
				if(lastSlashIndex >= 0 && (lastSlashIndex < rawPath.length() - 1))
				{
					sb.append(rawPath.substring(lastSlashIndex + 1));
				}
			}

			// Query part
			if(paramNameSet != null && !paramNameSet.isEmpty())
			{
				List<NameValuePair> queryList = URLEncodedUtils.parse(targetURI, encoding);
				for(NameValuePair entry : queryList)
				{
					String name = entry.getName();
					if(paramNameSet.contains(name))
					{
						if(sb.length() > 0)
						{
							sb.append(delim);
						}
						sb.append(entry.getValue());
					}
				}
			}

			if(sb.length() == 0)
			{
				sb.append("default");
			}
			sb.append(".html");

			result = sb.toString();
		}
		finally
		{
			reset();
		}

		return result;
	}
}
