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
	private Map<String, String> params = null;
	
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

		params = null;
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
			throw new IllegalArgumentException("URI must be absolute");
		}

		return requestURI;
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
	public RequestBuilder parameters(Map<String, String> params)
	{
		this.params = params;
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
			if(params != null && !params.isEmpty())
			{
				get = new HttpGet(requestURI.toString() + assemblyParameter(params));
			}
			else
			{
				get = new HttpGet(requestURI);
			}

			// Headers
			if(null != headers)
			{
				get.setHeaders(assemblyHeader(headers));
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
			if(params == null)
			{
				throw new IllegalArgumentException("must specify 'parameters' for POST");
			}
			if(encoding == null)
			{
				throw new IllegalArgumentException("must specify 'encoding' for POST");
			}

			post = new HttpPost(requestURI);

			// Post data
			List<NameValuePair> list = new ArrayList<NameValuePair>();
			for(String paramName : params.keySet())
			{
				list.add(new BasicNameValuePair(paramName, params.get(paramName)));
			}
			try
			{
				post.setEntity(new UrlEncodedFormEntity(list, encoding));
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
				post.setHeaders(assemblyHeader(headers));
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
	public static Header[] assemblyHeader(Map<String, String> headers)
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

	/**
	 * Assembly Parameters
	 * 
	 * @param paramMap
	 * @return
	 */
	public static String assemblyParameter(Map<String, String> paramMap)
	{
		StringBuilder sb = new StringBuilder("?");
		for(String str : paramMap.keySet())
		{
			sb.append(str).append("=").append(paramMap.get(str));
			sb.append("&");
		}
		return sb.substring(0, sb.length() - 1);
	}
}
