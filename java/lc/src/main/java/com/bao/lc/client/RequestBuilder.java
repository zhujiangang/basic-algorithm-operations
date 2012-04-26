package com.bao.lc.client;

import java.io.UnsupportedEncodingException;
import java.net.URI;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.http.Header;
import org.apache.http.NameValuePair;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.utils.URLEncodedUtils;
import org.apache.http.message.BasicHeader;
import org.apache.http.message.BasicNameValuePair;

public class RequestBuilder
{
	//Optional. Default: GET
	private String method;

	//Must specify one of the below 3
	
	//1. Raw URI
	private URI uri;
	
	//2. baseURI and reference
	private URI baseURI;
	private String reference;
	
	//3. Raw URI string
	private String uriString = null;

	//Optional
	private Map<String, String> paramMap = null;
	private List<NameValuePair> paramList = null;
	
	//Optional
	private Map<String, String> headers = null;

	//Optional. Default: UTF-8
	private String encoding = null;


	//Optional.
	private boolean allowRelativeURI = false;

	public RequestBuilder()
	{
		reset();
	}

	private void reset()
	{
		method = HttpGet.METHOD_NAME;

		uri = null;
		baseURI = null;
		reference = null;
		uriString = null;

		paramMap = null;
		paramList = null;
		headers = null;
		encoding = "UTF-8";

		allowRelativeURI = false;
	}

	private URI getRequestURI()
	{
		URI requestURI = null;

		do
		{
			// URI first
			if(uri != null)
			{
				requestURI = uri;
				break;
			}

			// Then baseURI
			if(baseURI != null)
			{
				if(reference == null)
				{
					throw new IllegalArgumentException(
						"must specify 'referrence' when 'baseURI' is available");
				}
				else
				{
					requestURI = baseURI.resolve(reference);
				}
				break;
			}

			// Last uriString
			if(uriString != null)
			{
				requestURI = URI.create(uriString);
				break;
			}

			// Error here
			throw new IllegalArgumentException("must specify URI");
		}
		while(false);

		if(!allowRelativeURI && !requestURI.isAbsolute())
		{
			throw new IllegalArgumentException("URI must be absolute, URI=" + requestURI.toString());
		}

		return requestURI;
	}
	
	private List<NameValuePair> getParameters()
	{
		if(paramList != null && paramMap != null)
		{
			throw new IllegalArgumentException("paramList and paramMap can't exist at the same time.");
		}
		
		if(paramList != null)
		{
			return paramList;
		}
		
		if(paramMap != null)
		{
			return toNameValuePairList(paramMap);
		}
		
		return null;
	}
	
	public RequestBuilder method(String method)
	{
		this.method = method;
		return this;
	}
	public RequestBuilder uri(URI uri)
	{
		this.uri = uri;
		return this;
	}
	public RequestBuilder baseURI(URI baseURI)
	{
		this.baseURI = baseURI;
		return this;
	}
	public RequestBuilder reference(String referrence)
	{
		this.reference = referrence;
		return this;
	}
	public RequestBuilder uriStr(String uriString)
	{
		this.uriString = uriString;
		return this;
	}
	public RequestBuilder paramMap(Map<String, String> paramMap)
	{
		this.paramMap = paramMap;
		return this;
	}
	public RequestBuilder paramList(List<NameValuePair> paramList)
	{
		this.paramList = paramList;
		return this;
	}
	public RequestBuilder headers(Map<String, String> headers)
	{
		this.headers = headers;
		return this;
	}
	public RequestBuilder encoding(String encoding)
	{
		this.encoding = encoding;
		return this;
	}
	public RequestBuilder allowRelativeURI(boolean allowRelativeURI)
	{
		this.allowRelativeURI = allowRelativeURI;
		return this;
	}

	public HttpUriRequest create()
	{
		HttpUriRequest request = null;
		if(HttpPost.METHOD_NAME.equalsIgnoreCase(method))
		{
			request = createPost();
		}
		else
		{
			request = createGet();
		}
		return request;
	}

	public HttpUriRequest create(String uriString)
	{
		this.uriString = uriString;
		return create();
	}

	public HttpGet createGet()
	{
		HttpGet get = null;
		try
		{
			URI requestURI = getRequestURI();

			// parameters
			List<NameValuePair> parameters = this.getParameters();
			if(parameters != null && !parameters.isEmpty())
			{
				String addedQuery = URLEncodedUtils.format(parameters, encoding);
				
				StringBuilder sb = new StringBuilder();
				sb.append(requestURI.toString());
				
				String rawQuery = requestURI.getRawQuery();
				//e.g. http://www.yahoo.com/
				if(rawQuery == null)
				{
					sb.append("?");
				}
				else
				{
					//e.g. http://www.yahoo.com/a.html?a=1
					if(!rawQuery.isEmpty() && !rawQuery.endsWith("&"))
					{
						sb.append("&");
					}
					//else case:
					//e.g. empty rawQuery: http://www.yahoo.com/?
					//e.g. http://www.yahoo.com/a.html?a=1&
				}
				sb.append(addedQuery);
				
				get = new HttpGet(sb.toString());
			}
			else
			{
				get = new HttpGet(requestURI);
			}

			// Headers
			if(null != headers)
			{
				get.setHeaders(toHeaders(headers));
			}

		}
		finally
		{
			// reset the properties
			reset();
		}

		return get;
	}

	public HttpGet createGet(String uriString)
	{
		this.uriString = uriString;
		return createGet();
	}

	public HttpPost createPost()
	{
		HttpPost post = null;
		try
		{
			URI requestURI = getRequestURI();
			
			// parameters
			List<NameValuePair> parameters = this.getParameters();
			if(parameters == null)
			{
				throw new IllegalArgumentException("must specify 'parameters' for POST");
			}
			if(encoding == null)
			{
				throw new IllegalArgumentException("must specify 'encoding' for POST");
			}

			post = new HttpPost(requestURI);

			// Post data
			try
			{
				post.setEntity(new UrlEncodedFormEntity(parameters, encoding));
			}
			catch(UnsupportedEncodingException e)
			{
				IllegalArgumentException e2 = new IllegalArgumentException();
				e2.initCause(e);
				throw e2;
			}

			// Headers
			if(null != headers)
			{
				post.setHeaders(toHeaders(headers));
			}
		}
		finally
		{
			// reset the properties
			reset();
		}

		return post;
	}

	public HttpGet createPost(String uriString)
	{
		this.uriString = uriString;
		return createGet();
	}
	
	/**
	 * Assembly Headers
	 * 
	 * @param headers
	 * @return
	 */
	private static Header[] toHeaders(Map<String, String> headers)
	{
		Header[] allHeader = new BasicHeader[headers.size()];
		int i = 0;
		for(String str : headers.keySet())
		{
			allHeader[i] = new BasicHeader(str, headers.get(str));
			i++;
		}
		return allHeader;
	}
	
	private static List<NameValuePair> toNameValuePairList(Map<String, String> paramMap)
	{
		List<NameValuePair> list = new ArrayList<NameValuePair>(paramMap.size());
		for(String paramName : paramMap.keySet())
		{
			list.add(new BasicNameValuePair(paramName, paramMap.get(paramName)));
		}
		return list;
	}
}
