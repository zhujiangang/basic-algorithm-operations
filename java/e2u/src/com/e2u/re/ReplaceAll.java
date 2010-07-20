package com.e2u.re;

public class ReplaceAll
{
	public static void main(String[] args)
	{
		String filePath = "C:\\temp\\dat\\";
		System.out.println(filePath); 		
		System.out.println(filePath.replaceAll("\\\\", "\\\\\\\\"));
		System.out.println(filePath.replaceAll("\\", "\\\\"));

	}
}
