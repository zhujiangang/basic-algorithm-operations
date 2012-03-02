package com.bao.lc.common;

public enum ResultCode implements IDValuePair
{
	RC_OK,
	RC_UNKOWN,
	RC_LOGIN_FAILED,
	RC_HTTP_ERROR,
	RC_REG_LIST_EMPTY,
	RC_DOCTOR_NOT_EXIST,
	RC_DOCTOR_DETAIL_LIST_EMPTY,
	RC_DOCTOR_REG_LIST_FULL,
	RC_REG_MESSAGE_SUCCESS,
	RC_REG_MESSAGE_FAIL,
	RC_ALREADY_REGISTERED,
	RC_NO_MATCHED_RECORD,
	RC_EXCEPTION_CAUGHT,
	RC_TIMEOUT,
	RC_EXCEED_MAX_TRY_TIMES,
	RC_LAST;
	
	private final int id;
	private final String value;
	
	ResultCode()
	{
		this.id = this.ordinal();
		this.value = this.name();
	}
	ResultCode(int id, String value)
	{
		this.id = id;
		this.value = value;
	}
	
	public int getID()
	{
		return id;
	}
	
	public String getValue()
	{
		return value;
	}
}
