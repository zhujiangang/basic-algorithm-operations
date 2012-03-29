package com.bao.lc.httpcommand.params;

public interface HttpCommandPNames
{
	/**
	 * Attribute name of a {@link org.apache.http.client.methods.HttpUriRequest}
	 * object that represents the actual URI to connect.
	 */
	public static final String TARGET_REQUEST = "lc.target.request";

	/**
	 * Attribute name of a {@link java.lang.String} object that represents the
	 * referrer field when connect to TARGET_URI_REQUEST
	 */
	public static final String TARGET_REFERER = "lc.target.referer";

	/**
	 * Attribute name of a {@link org.apache.http.HttpResponse} object that
	 * represents the execution response of HTTP client.
	 */
	public static final String TARGET_RESPONSE = "lc.target.response";

	/**
	 * Attribute name of a {@link java.lang.String} object that represents the
	 * default charset of the TARGET_RESPONSE
	 */
	public static final String RESPONSE_DEFAULT_CHARSET = "lc.response_default_charset";

	/**
	 * Attribute name of a {@link com.bao.lc.bean.IDValuePair} object that
	 * represents the actual result of the current command.
	 */
	public static final String HTTP_COMMAND_RESULT_CODE = "lc.http.command.result.code";

	/**
	 * Attribute name of a {@link org.apache.commons.chain.Command} object that
	 * represents the actual result of the current command.
	 */
	public static final String EXCEPTION_COMMAND = "lc.exception.command";

	/**
	 * Attribute name of a {@link com.bao.lc.client.BrowserClient} object that
	 * represents the HTTP client.
	 */
	public static final String HTTP_CLIENT = "lc.http_client";

	/**
	 * Attribute name of a {@link org.apache.http.protocol.HttpContext} object
	 * that represents the execution context of HTTP client.
	 */
	public static final String HTTP_CONTEXT = "lc.http_context";
}
