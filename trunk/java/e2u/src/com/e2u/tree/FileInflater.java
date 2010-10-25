package com.e2u.tree;

import java.io.*;

public class FileInflater
{
	private String inputFileName = null;
	private String outputFileName = null;
	
	private Inflater inflater = null;
	private CGFileHeader header = null;
	
	public FileInflater(String inFile, String outDir, String outFile)
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
			if(outFile != null)
			{
				outputFileName = outFile;
			}
			if(outDir != null)
			{
				outputFileName = outDir + File.separator + outputFileName;
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
	
	public FileInflater(String inFile)
	{
		this(inFile, null, null);
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
	
	public static void main(String[] args)
	{
		String input = "input.cg";
		String outDir = null;
		String outFile = null;
		
		if(args != null && args.length >= 1)
		{
			input = args[0];		
		}
		if(args != null && args.length >= 2)
		{
			outDir = args[1];		
		}
		if(args != null && args.length >= 3)
		{
			outFile = args[2];		
		}
		FileInflater fif = new FileInflater(input, outDir, outFile);
		fif.decompress();
	}
}
