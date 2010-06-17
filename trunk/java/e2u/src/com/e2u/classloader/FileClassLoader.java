package com.e2u.classloader;

import java.io.*;

public class FileClassLoader extends ClassLoader {
	public Class findClass(String name) {
		byte[] data = loadClassData(name);
		return defineClass(name, data, 0, data.length);
	}

	public byte[] loadClassData(String name) {
		FileInputStream fis = null;
		byte[] data = null;
		try {
			fis = new FileInputStream(new File("C:\\Temp\\bin\\" + name + ".class"));
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			int ch = 0;
			while ((ch = fis.read()) != -1) {
				baos.write(ch);

			}
			data = baos.toByteArray();
		} catch (IOException e) {
			e.printStackTrace();
			
			try {
				fis = new FileInputStream(new File("C:\\Temp\\bin2\\" + name + ".class"));
				ByteArrayOutputStream baos = new ByteArrayOutputStream();
				int ch = 0;
				while ((ch = fis.read()) != -1) {
					baos.write(ch);

				}
				data = baos.toByteArray();
			} catch (Exception e2) {
				e.printStackTrace();
			}
			
		}

		return data;
	}
	
	
}
