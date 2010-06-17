package com.e2u.classloader;

public class LoaderSample1 {
	public static void main(String[] args) {
		Class c;
		ClassLoader cl;
		cl = ClassLoader.getSystemClassLoader();
		System.out.println(cl);
		while (cl != null) {
			cl = cl.getParent();
			System.out.println(cl);
		}
		try {
			c = Class.forName("java.lang.Object");
			cl = c.getClassLoader();
			System.out.println("java.lang.Object's loader is " + cl);
			c = Class.forName("com.e2u.classloader.LoaderSample1");
			cl = c.getClassLoader();
			System.out.println("LoaderSample1's loader is " + cl);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
