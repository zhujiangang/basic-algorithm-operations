package com.e2u.file;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

public class TransformFileFrameWork
{
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		String inFileName = "mapping.xml";
		MyFileContentListener listener = new MyFileContentListener();

		FileTransformer transformer = new FileTransformer(inFileName, listener);

		try
		{
			transformer.transform();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}

class MyFileContentListener implements FileContentListener
{
	private static String XML_HEADER = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";

	private PrintWriter pw = null;
	private String curFileName = null;

	public void actionPeformed(String line, int lineNum) throws Exception
	{
		if(!line.startsWith("  "))
		{
			if(line.startsWith("\t"))
			{
				System.out.println("[REPLACE TAB]:" + "(" + lineNum + ") file="
					+ curFileName + line);
				line.replaceFirst("\t", "  ");
			}
			else
			{
				System.out.println("[INVALID START]:" + "(" + lineNum
					+ ") file=" + curFileName + line);
				return;
			}
		}
		// process
		if(line.trim().startsWith("<programEntity "))
		{
			String programName = getProgramName(line);
			if(programName == null)
			{
				System.err.println("[ERROR]: Get programe name null from "
					+ line + "(" + lineNum + ")");
				return;
			}
			if(pw != null)
			{
				System.err.println("[ERROR]: pw is not null" + "(" + lineNum
					+ ")");
				throw new Exception("[ERROR]: pw is not null" + "(" + lineNum
					+ ")");
			}
			curFileName = "mapping-" + programName + ".xml";
			pw = new PrintWriter(new BufferedWriter(new OutputStreamWriter(
				new FileOutputStream(curFileName))), true);
			pw.println(XML_HEADER);
			pw.println(line.substring(2));
		}
		else if(line.trim().startsWith("</programEntity>"))
		{
			pw.println(line.substring(2));
			pw.close();
			pw = null;
			curFileName = null;
		}
		else
		{
			if(pw == null)
			{
				System.out.println("[INFO]: pw = null, line=" + line + "(" + lineNum + ")");
				return;
			}
			pw.println(line.substring(2));
		}
	}

	private String getProgramName(String str)
	{
		String[] results = str.trim().split(" ");
		for(int i = 0; i < results.length; i++)
		{
			if(results[i].startsWith("programName="))
			{
				String tmp = results[i].substring("programName=".length());
				return tmp.substring(1, tmp.length() - 1);
			}
		}
		return null;
	}
}
