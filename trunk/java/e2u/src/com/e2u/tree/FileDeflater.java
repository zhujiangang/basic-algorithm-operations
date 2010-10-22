package com.e2u.tree;

import java.io.*;

import com.e2u.bit.BitUtil;

public class FileDeflater
{
	private CGFileHeader header = null;
	private HuffmanTree hfTree = null;
	
	private String inputFileName = null;
	private String outputFileName = null;
	
	private Deflater deflater = null;
	
	public FileDeflater(String inFile, String outFile)
	{
		this.inputFileName = inFile;
		this.outputFileName = outFile;
		
		//Construct File Header construct
		header = new CGFileHeader();
		
		//Init file name filed
		byte[] fileNameInBytes = inputFileName.getBytes();
		if(fileNameInBytes.length > CGFileHeader.FILE_NAME_SIZE_BY_BYTE)
		{
			throw new IllegalArgumentException("The File name " + inputFileName + " is longer than " + CGFileHeader.FILE_NAME_SIZE_BY_BYTE);
		}
		for(int i = 0; i < fileNameInBytes.length; i++)
		{
			header.originalFileName[i] = fileNameInBytes[i];
		}
		
		//Init file length;
		File file = new File(inputFileName);
		header.originalLength = (int)file.length();
		
		//The whole file weights
//		int[] weights = new int[CGFileHeader.WEIGHT_TABLE_SIZE_BY_INT];
//		for(int i = 0; i < CGFileHeader.WEIGHT_TABLE_SIZE_BY_INT; i++)
//		{
//			weights[i] = 0;
//		}
		
		try
		{
			BufferedInputStream bis = new BufferedInputStream(new FileInputStream(inputFileName));

			int bufferSize = 4096;
			byte[] buffer = new byte[bufferSize];
			
			int bytesNumRead = 0;
			
			while((bytesNumRead = bis.read(buffer)) != -1)
			{
				for(int i = 0; i < bytesNumRead; i++)
				{
//					weights[BitUtil.unsigned(buffer[i])]++;
					header.weightTable[BitUtil.unsigned(buffer[i])]++;
				}
			}			
			bis.close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
		hfTree = new HuffmanTree(header.weightTable);
		deflater = new Deflater(hfTree.getEncodeTable());
	}
	
	public void compress()
	{
		try
		{
			BufferedInputStream bis = new BufferedInputStream(new FileInputStream(inputFileName));
			int bufferSize = 4096;
			byte[] buffer = new byte[bufferSize];
			
			int bytesNumRead = 0;
			
			while((bytesNumRead = bis.read(buffer)) != -1)
			{
				deflater.addCompressInputData(buffer, bytesNumRead);
			}			
			bis.close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
		byte[] encodeData = deflater.finishCompress();
		
		try
		{
			BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(outputFileName));
			
			bos.write(header.getBytes());
			bos.write(encodeData);
		
			bos.flush();
			
			bos.close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
	}
}
