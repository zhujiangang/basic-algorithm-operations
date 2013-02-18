package com.bao.examples.thread;

public interface TokenPool
{
	public static final int MAX_TOKEN_COUNT = 20;
	
	public int getToken();
	
	public void releaseToken(int token);
	
	public int getTotalCount();
	
	public int getReservedCount();
}
