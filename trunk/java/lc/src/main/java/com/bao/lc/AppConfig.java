package com.bao.lc;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Properties;

import org.apache.commons.io.IOUtils;
import org.apache.commons.lang.BooleanUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.common.UTF8BufferedInputStream;

public class AppConfig
{
	private static Log log = LogFactory.getLog(AppConfig.class);

	private static final String[] RES_LOCATIONS = { "conf/usr", "conf/default" };

	private Properties propInternal = new Properties();
	private Properties propConfig = new Properties();
	private Properties propInput = new Properties();

	private String baseDir = null;
	private String outputDir = null;
	private String tmpDir = null;
	
	private boolean isDebug = false;

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
		init();
	}

	private boolean init()
	{
		boolean result = false;

		String path = null;
		do
		{
			// 1. init base dir
			path = getFolderPath(".", true);
			if(path == null)
			{
				break;
			}
			baseDir = path;

			loadFile("internal.properties", "UTF-8", propInternal);
			loadFile("config.properties", "UTF-8", propConfig);
			loadFile("input.properties", "UTF-8", propInput);

			outputDir = getFolderPath(getPropConfig("dir.output"), true);
			tmpDir = getFolderPath(getPropConfig("dir.temp"), true);
			isDebug = BooleanUtils.toBoolean(getPropConfig("debug"));

			// everything is ok
			result = true;
		}
		while(false);

		return result;
	}

	private String getFolderPath(String folderName, boolean makeSureExist)
	{
		File folder = new File(folderName);
		if(makeSureExist && !folder.exists())
		{
			if(!folder.mkdirs())
			{
				log.error("Failed to create dir: " + folderName);
				return null;
			}
		}

		String path = null;
		try
		{
			path = folder.getCanonicalPath();
		}
		catch(IOException e)
		{
			log.warn("Failed to canonicalize path: " + folderName, e);
			path = folder.getAbsolutePath();
		}

		return path;
	}

	public String getBaseDir()
	{
		return this.baseDir;
	}

	public String getOutputDir()
	{
		return outputDir;
	}

	public String getTempDir()
	{
		return tmpDir;
	}
	
	public boolean isDebug()
	{
		return isDebug;
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

	private String getResourceFullName(String baseName, String name)
	{
		// FileSystem search
		StringBuilder sb = new StringBuilder(getBaseDir());
		sb.append(System.getProperty("file.separator"));
		sb.append(baseName);
		sb.append(System.getProperty("file.separator"));
		sb.append(name);

		return getFolderPath(sb.toString(), false);
	}

	private String getProperty(Properties prop, String propName)
	{
		String propValue = prop.getProperty(propName);

		if(propValue == null)
		{
			log.warn("key [" + propName + "] not found.");
			propValue = propName;
		}
		if(propValue.startsWith(" ") || propValue.endsWith(" "))
		{
			log.warn("The value of key [" + propName + "] contains WHITESPACE at the begin or end.");
			propValue = propValue.trim();
		}
		
		return propValue;
	}

	public URL getResource(String name)
	{
		URL url = null;
		String fullName = null;

		// 1. Search from file system
		for(int i = 0; i < RES_LOCATIONS.length; i++)
		{
			fullName = getResourceFullName(RES_LOCATIONS[i], name);
			File file = new File(fullName);

			if(file.exists())
			{
				try
				{
					url = file.toURI().toURL();

					if(log.isDebugEnabled())
					{
						log.debug("Found resource [" + name + "] from url: " + url.toString());
					}
					return url;
				}
				catch(MalformedURLException e)
				{
					log.error("Failed to convert file [" + fullName + "] to URL.", e);
				}
			}
		}

		// 2. Search from ClassLoader
		if(!name.startsWith("/"))
		{
			fullName = "/" + name;
		}
		else
		{
			fullName = name;
		}
		url = getClass().getResource(fullName);
		if(log.isDebugEnabled() && url != null)
		{
			log.debug("Found resource [" + name + "] from url: " + url.toString());
		}

		if(url == null)
		{
			log.warn("Failed to find resource: " + name);
		}
		return url;
	}

	private boolean loadFile(String file, String charsetName, Properties prop)
	{
		URL url = getResource(file);
		if(url == null)
		{
			return false;
		}

		boolean result = false;

		InputStream is = null;
		InputStreamReader reader = null;
		try
		{
			is = url.openStream();
			if("UTF-8".equalsIgnoreCase(charsetName))
			{
				is = new UTF8BufferedInputStream(is);
			}

			reader = new InputStreamReader(is, charsetName);
			prop.load(reader);

			result = true;
		}
		catch(Exception e)
		{
			log.error("Failed to load file [" + file + "]", e);
		}
		finally
		{
			IOUtils.closeQuietly(reader);
			IOUtils.closeQuietly(is);
		}

		return result;
	}
}

