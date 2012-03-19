package com.bao.examples.temp;

import javax.script.*;
import java.util.*;
import static java.lang.System.*;

public class ListScriptEngines
{
	public static void main(String args[])
	{
		ScriptEngineManager manager = new ScriptEngineManager();
		// 得到所有的脚本引擎工厂

		List<ScriptEngineFactory> factories = manager.getEngineFactories();
		// 这是Java SE 5 和Java SE 6的新For语句语法

		for(ScriptEngineFactory factory : factories)
		{
			// 打印脚本信息

			out.printf("Name: %s%n" + "Version: %s%n" + "Language name: %s%n"
				+ "Language version: %s%n" + "Extensions: %s%n" + "Mime types: %s%n"
				+ "Names: %s%n", factory.getEngineName(), factory.getEngineVersion(),
				factory.getLanguageName(), factory.getLanguageVersion(), factory.getExtensions(),
				factory.getMimeTypes(), factory.getNames());
			// 得到当前的脚本引擎

			ScriptEngine engine = factory.getScriptEngine();

			System.out.println(engine);
		}
	}
}
