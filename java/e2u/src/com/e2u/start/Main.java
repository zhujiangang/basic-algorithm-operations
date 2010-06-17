package com.e2u.start;

import java.lang.reflect.Method;

import com.e2u.classloader.FileClassLoader;

public class Main {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		ClassLoader cl = new FileClassLoader();
		
		try {
			Class clazz = cl.loadClass("HelloWorld");
			
			Class[] signs = new Class[1];
			signs[0] = String[].class;
//			signs[0] = Class.forName("[Ljava.lang.String");
			Method method = clazz.getDeclaredMethod("main", signs);
			
			Object[] params = new Object[1];
			
			String[] inputArgs = {"abc", "def"};
			params[0] = inputArgs;
			method.invoke(null, params);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}

}
