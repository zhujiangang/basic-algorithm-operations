package com.bao.examples.app;

import java.io.File;

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
