package com.e2u.classloader;

import java.io.*;

public class FileSystemClassLoader extends ClassLoader
{
	private String rootDir = null;

	public FileSystemClassLoader(String rootDir)
	{
		this.rootDir = rootDir;
	}

	public Class<?> loadClass(String name) throws ClassNotFoundException
	{
		// First, check if the class has already been loaded
		Class c = findLoadedClass(name);
		
		//Not loaded yet
		if(c == null)
		{
			//First load this class by own
			c = findClass(name);
			
			//Can't find, delegate to parent
			if(c == null)
			{
				System.out.println("Load class from default ClassLoader: [" + name + "]");
				c = super.loadClass(name);
			}
		}
		else
		{
			System.out.println(c.getClassLoader() + " load class " + name);
		}

		return c;
	}

	protected Class<?> findClass(String name)
	{
		byte[] classData = getClassData(name);
		if(classData == null)
		{
			return null;
		}
		return defineClass(name, classData, 0, classData.length);
	}

	private byte[] getClassData(String className)
	{
		byte[] byteArray = null;
		String path = classNameToPath(className);
		
		File file = new File(path);
		if(!file.exists())
		{
			return null;
		}
		try
		{
			InputStream ins = new FileInputStream(file);
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			int bufferSize = 4096;
			byte[] buffer = new byte[bufferSize];
			int bytesNumRead = 0;
			while((bytesNumRead = ins.read(buffer)) != -1)
			{
				baos.write(buffer, 0, bytesNumRead);
			}
			byteArray = baos.toByteArray();

			ins.close();
			baos.close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		return byteArray;
	}

	private String classNameToPath(String className)
	{
		return rootDir + File.separatorChar
			+ className.replace('.', File.separatorChar) + ".class";
	}
}
