package com.e2u.file;

import java.io.*;

public class FileReadWrite
{
	private static String XML_HEADER = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";

	private void readFile(String inFileName)
	{
		try
		{
			BufferedReader bufReader = new BufferedReader(new FileReader(
				inFileName));

			PrintWriter pw = null;
			String curFileName = null;
			String line = null;
			int lineNum = 0;
			while((line = bufReader.readLine()) != null)
			{
				lineNum++;
				if(!line.startsWith("  "))
				{					
					if(line.startsWith("\t"))
					{
						System.out.println("[REPLACE TAB]:" + "(" + lineNum + ") file=" + curFileName + line);
						line.replaceFirst("\t", "  ");
					}
					else
					{
						System.out.println("[INVALID START]:" + "(" + lineNum + ") file=" + curFileName + line);
						continue;						
					}
				}
				// process
				if(line.trim().startsWith("<programEntity "))
				{
					String programName = getProgramName(line);
					if(programName == null)
					{
						System.err.println("[ERROR]: Get programe name null from " + line + "(" + lineNum + ")");
						continue;
					}
					if(pw != null)
					{
						System.err.println("[ERROR]: pw is not null" + "(" + lineNum + ")");
						break;
					}
					curFileName = "mapping-" + programName + ".xml";
					pw = new PrintWriter(new BufferedWriter(
						new OutputStreamWriter(new FileOutputStream(curFileName))), true);
					pw.println(XML_HEADER);					
					pw.println(line.substring(2));
				}
				else if(line.trim().startsWith("</programEntity>"))
				{
					pw.println(line.substring(2));
					pw = null;
					curFileName = null;
				}
				else
				{
					if(pw == null)
					{
						System.out.println("[INFO]: pw = null, line=" + line + "(" + lineNum + ")");
						continue;
					}
					pw.println(line.substring(2));
				}
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
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

	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		FileReadWrite frw = new FileReadWrite();
		frw.readFile("mapping.xml");
	}

}
