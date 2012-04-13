package com.bao.lc;

public class ResMgr
{
	private ResMgr() {}
	
	public static String getString(String key)
	{
		return AppConfig.getInstance().getPropResource(key);
	}
}
