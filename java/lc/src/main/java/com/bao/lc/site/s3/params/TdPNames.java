package com.bao.lc.site.s3.params;

public interface TdPNames
{
	/* Input parameters*/
	public static final String PARAM_USER = "td.param.user";
	public static final String PARAM_PASSWORD = "td.param.password";
	
	public static final String PARAM_FROM_STATION = "td.param.from_station";
	public static final String PARAM_TO_STATION = "td.param.to_station";
	public static final String PARAM_TICKET_DATE = "td.param.ticket_date";
	public static final String PARAM_TICKET_TIME_RANGE = "td.param.ticket_time_range";
	
	public static final String PARAM_USER_COUNT = "td.param.user.count";
	
	public static final String PARAM_TICKET_BOOK_INIT_URL = "td.param.ticket_booking_init_url";
	public static final String PARAM_TICKET_BOOK_INIT_REFERER = "td.param.ticket_booking_init_referer";
	
	public static final String PARAM_QUERY_LEFT_TICKET_BASE_URL = "td.param.query_left_ticket_base_url";
	public static final String PARAM_QUERY_LEFT_TICKET_REFERER = "td.param.query_left_ticket_referer";
	
	public static final String PARAM_SUBMIT_ORDER_URL = "td.param.submit_order_url";
	public static final String PARAM_SUBMIT_ORDER_REFERER = "td.param.submit_order_referer";
	
	/*Computed parameters*/
	public static final String _USER_INTERFACE = "_td.user_interface";	
	public static final String _TICKET_INFO_LIST = "_td.train_ticket_info.list";
	public static final String _ORDER_FORM_PARAMETER_MAP = "_td.order_form.parameter.map";
	public static final String _ORDER_TICKET_INFO = "_td.order_ticket_info";
	
	public static final String _LOGIN_PAGE_CONTENT = "_td.login_page.content";
	public static final String _LOGIN_PAGE_ENCODING = "_td.input.encoding";
	public static final String _IS_FIRST_LOGIN = "_td.is.first_login";
	public static final String _LOGIN_VOCDE = "_td.login.verification.code";
	public static final String _LOGIN_STATE = "_td.login.state";
}
