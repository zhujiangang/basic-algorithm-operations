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
	
	public static final String PARAM_PASSENGER_COUNT = "td.param.user.count";
	public static final String PARAM_PASSENGER_LIST = "td.param.passenger.list";
	
	public static final String PARAM_WELCOME_URL = "td.param.welcome_url";
	public static final String PARAM_WELCOME_REFERER = "td.param.welcome_referer";
	
	public static final String PARAM_LOGIN_URL = "td.param.login_url";
	public static final String PARAM_LOGIN_REFERER = "td.param.login_referer";
	
	public static final String PARAM_TICKET_BOOK_INIT_URL = "td.param.ticket_booking_init_url";
	public static final String PARAM_TICKET_BOOK_INIT_REFERER = "td.param.ticket_booking_init_referer";
	
	public static final String PARAM_QUERY_LEFT_TICKET_BASE_URL = "td.param.query_left_ticket_base_url";
	public static final String PARAM_QUERY_LEFT_TICKET_REFERER = "td.param.query_left_ticket_referer";
	
	public static final String PARAM_SUBMIT_ORDER_URL = "td.param.submit_order_url";
	public static final String PARAM_SUBMIT_ORDER_REFERER = "td.param.submit_order_referer";
	
	public static final String PARAM_CONFIRM_PASSENGER_INIT_URL = "td.param.confirm_passenger_init_url";
	public static final String PARAM_CONFIRM_PASSENGER_INIT_REFERER = "td.param.confirm_passenger_init_referer";

	public static final String PARAM_CONFIRM_PASSENGER_DO_URL = "td.param.confirm_passenger_do_url";
	public static final String PARAM_CONFIRM_PASSENGER_DO_REFERER = "td.param.confirm_passenger_do_referer";
	
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
	
	public static final String _CONFIRM_PASSENGER_CONTENT = "_td.confirm_passenger.content";
	public static final String _CONFIRM_PASSENGER_ENCODING = "_td.confirm_passenger.encoding";
	public static final String _CONFIRM_PASSENGER_VOCDE = "_td.confirm_passenger.verification.code";
	
}
