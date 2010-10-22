package com.e2u.tree;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;

public class FileInflater
{
	private String inputFileName = null;
	private String outputFileName = null;
	
	private Inflater inflater = null;
	private CGFileHeader header = null;
	
	public FileInflater(String inFile)
	{
		this.inputFileName = inFile;
	
		try
		{
			BufferedInputStream bis = new BufferedInputStream(new FileInputStream(inputFileName));

			int bufferSize = 4096;
			byte[] buffer = new byte[bufferSize];
			
			int bytesNumRead = bis.read(buffer, 0, CGFileHeader.HEADER_SIZE);
			if(bytesNumRead == -1)
			{
				throw new IllegalArgumentException("File Content Error");
			}
			header = new CGFileHeader(buffer, 0);
			
			for(int i = 0; i < CGFileHeader.FILE_NAME_SIZE_BY_BYTE; i++)
			{				
				if(header.originalFileName[i] == 0)
				{
					outputFileName = new String(header.originalFileName, 0, i);
					break;
				}				
			}
			if(outputFileName == null)
			{
				outputFileName = new String(header.originalFileName, 0, CGFileHeader.FILE_NAME_SIZE_BY_BYTE);
			}
			
			HuffmanTree hfTree = new HuffmanTree(header.weightTable);
			inflater = new Inflater(hfTree.getRoot());
			
			bis.close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public void decompress()
	{
		try
		{
			BufferedInputStream bis = new BufferedInputStream(new FileInputStream(inputFileName));
			int bufferSize = 4096;
			byte[] buffer = new byte[bufferSize];
			
			int bytesNumRead = bis.read(buffer, 0, CGFileHeader.HEADER_SIZE);
			
			while((bytesNumRead = bis.read(buffer)) != -1)
			{
				inflater.addDecompressInputData(buffer, bytesNumRead);
			}			
			bis.close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
		byte[] decodeData = inflater.finishDecompress();
		
		try
		{
			BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(outputFileName));
			
			bos.write(decodeData);
		
			bos.flush();
			
			bos.close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}
