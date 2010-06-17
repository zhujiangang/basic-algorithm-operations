package com.e2u.file;

import java.io.*;

public class FileTransformer
{
	private String inFileName = null;
	private FileContentListener listener = null;
	
	public FileTransformer(String inFileName, FileContentListener listener)
	{
		this.inFileName = inFileName;
		this.listener = listener;
	}
	
	public void transform() throws Exception
	{
		BufferedReader bufReader = null;
		try
		{
			bufReader = new BufferedReader(new FileReader(inFileName));
			
			String line = null;
			int lineNum = 0;
			while( (line = bufReader.readLine()) != null)
			{
				lineNum++;
				listener.actionPeformed(line, lineNum);
			}
		}
		finally
		{
			if(bufReader != null)
			{
				try
				{
					bufReader.close();
				}
				catch(IOException e)
				{
					e.printStackTrace();
				}
			}
		}		
	}
}
