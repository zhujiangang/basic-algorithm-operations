package com.bao.lc;

import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.util.CommonUtil;

public class AppConfig
{
	private static Log log = LogFactory.getLog(AppConfig.class);
	
	private Properties propInternal = new Properties();
	private Properties propConfig = new Properties();
	private Properties propInput = new Properties();
	
	private String baseDir = null;
	private String outputDir = null;
	private String tmpDir = null;
	
	private static AppConfig instance = null;
	
	public static AppConfig getInstance()
	{
		if(instance == null)
		{
			instance = new AppConfig();
		}
		return instance;
	}
	
	private AppConfig()
	{
		initConfig();
	}
	
	private void initConfig()
	{
		loadFile("/internal.properties", "UTF-8", propInternal);
		loadFile("/config.properties", "UTF-8", propConfig);
		loadFile("/input.properties", "UTF-8", propInput);
		
		baseDir = getFolderPath(".");
		outputDir = getFolderPath(getPropConfig("dir.output"));
		tmpDir = getFolderPath(getPropConfig("dir.temp"));
	}
	
	private String getFolderPath(String folderName)
	{
		File folder = new File(folderName);
		if(!folder.exists())
		{
			folder.mkdir();
		}
		
		String path = null;
		try
		{
			path = folder.getCanonicalPath();
		}
		catch(IOException e)
		{
			path = folder.getAbsolutePath();
		}
		
		return path;
	}
	
	private String getProperty(Properties prop, String propName)
	{
		String propValue = prop.getProperty(propName);
		
		if(propValue == null)
		{
			log.warn("key [" + propName + "] not found.");
			propValue = propName;
		}
		return propValue;
	}
	
	public String getPropInternal(String propName)
	{
		return getProperty(propInternal, propName);
	}
	
	public String getPropConfig(String propName)
	{
		return getProperty(propConfig, propName);
	}
	
	public String getPropInput(String propName)
	{
		return getProperty(propInput, propName);
	}
	
	public String getBaseDir()
	{
		return baseDir;
	}
	
	public String getOutputDir()
	{
		return outputDir;
	}
	
	public String getTempDir()
	{
		return tmpDir;
	}
	
	private boolean loadFile(String file, String charsetName, Properties prop)
	{
		boolean result = false;
		try
		{
			InputStreamReader reader = new InputStreamReader(getClass().getResourceAsStream(file), charsetName);
			prop.load(reader);
			
			CommonUtil.trimUtf8Bom(prop);
			
			result = true;
		}
		catch(Exception e)
		{
			log.error("Failed to load file [" + file + "]", e);
		}
		
		return result;
	}
}
