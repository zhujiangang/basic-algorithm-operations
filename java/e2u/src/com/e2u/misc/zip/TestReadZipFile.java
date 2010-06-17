package com.e2u.misc.zip;

import java.io.File;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class TestReadZipFile {

	public static void readZip(String zipFileName)
	{
		try {
			ZipFile zipFile = new ZipFile(zipFileName);
			for(Enumeration e = zipFile.entries(); e.hasMoreElements(); )
			{
				ZipEntry entry = (ZipEntry)e.nextElement();
//				System.out.println(entry.getName() + " | " + entry.getMethod() + " | " + entry.getSize() + " | " + entry.getCompressedSize());
				System.out.println(entry.getName());
			}
		} catch (Exception e) {
			e.printStackTrace();
		}	
	}
	public static void scanZipDir(String dirName, String fileNameInZip)
	{
		try {
			File dir = new File(dirName);
			if(!dir.isDirectory())
			{
				System.err.println(dirName + " is not Directory.");
				return;
			}
			
			File[] files = dir.listFiles();
			if(files == null || files.length == 0)
			{
				System.err.println("There's no files in the directory " + dirName);
				return;
			}			
			
			String fileName;
			for(int i = 0; i < files.length; i++)
			{
				if(files[i].isFile())
				{
					fileName = files[i].getName();
					if(!fileName.endsWith(".jar") && !fileName.endsWith(".zip"))
					{
						continue;
					}
					findFileInZip(files[i], fileNameInZip);
				}
			}
			
			for(int i = 0; i < files.length; i++)
			{
				if(files[i].isDirectory())
				{
					scanZipDir(files[i].getCanonicalPath(), fileNameInZip);
				}
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}
	private static boolean findFileInZip(File file, String fileNameInZip)
	{
		try {
			ZipFile zipFile = new ZipFile(file);
			for(Enumeration e = zipFile.entries(); e.hasMoreElements(); )
			{
				ZipEntry entry = (ZipEntry)e.nextElement();
				if(entry.getName().indexOf(fileNameInZip) != -1)
				{
					System.out.println("Found \"" + fileNameInZip + "\" in " + file.getCanonicalPath() + ", entry = " + entry.getName());
					return true;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}	
		return false;
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
//		readZip("C:/Temp/test/lib/bbms.jar");
		scanZipDir("C:/Temp/test", "com/tellabs/inm/oam/Logging");
	}

}
