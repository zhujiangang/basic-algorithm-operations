package com.e2u.tree;

public class TestHuff
{
	public static void main(String[] args)
	{
//		FileDeflater df = new FileDeflater("serial", "serial.cg");
//		df.compress();
		
		FileInflater inf = new FileInflater("serial.cg");
		inf.decompress();
	}
}
