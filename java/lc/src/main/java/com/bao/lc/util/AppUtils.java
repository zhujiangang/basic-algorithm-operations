package com.bao.lc.util;

import java.io.File;

import com.bao.lc.AppConfig;

public class AppUtils
{
	
	public static String getOutputFilePath(String fileName)
	{
		return AppConfig.getInstance().getOutputDir() + File.separator + fileName;
	}
	
	public static String getTempFilePath(String fileName)
	{
		return AppConfig.getInstance().getTempDir() + File.separator + fileName;
	}
}
