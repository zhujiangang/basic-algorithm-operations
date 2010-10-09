package com.e2u.classloader;

import java.io.File;
import java.io.FileInputStream;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;

/**
 * 可以重新载入同名类的类加载器实现
 * 
 * 
 * 放弃了双亲委派的加载链模式. 需要外部维护重载后的类的成员变量状态.
 * 
 * @author ken.wu
 * @mail ken.wug@gmail.com 2007-9-28 下午01:37:43
 */
public class HotSwapClassLoader extends URLClassLoader
{

	public HotSwapClassLoader(URL[] urls)
	{
		super(urls);
	}

	public HotSwapClassLoader(URL[] urls, ClassLoader parent)
	{
		super(urls, parent);
	}

	public Class load(String name) throws ClassNotFoundException
	{
		return load(name, false);
	}

	public Class load(String name, boolean resolve)
		throws ClassNotFoundException
	{
		if(null != findLoadedClass(name))
			return reload(name, resolve);

		Class clazz = findClass(name);

		if(resolve)
			resolveClass(clazz);

		return clazz;
	}

	public Class reload(String name, boolean resolve)
		throws ClassNotFoundException
	{
		return new HotSwapClassLoader(getURLs(), getParent()).load(
			name, resolve);
	}
}
