package com.bao.examples.js;

import javax.script.*;

import org.apache.commons.io.HexDump;

import junit.framework.TestCase;

public class JavaScriptTest extends TestCase
{
	public void testJavaScript1() throws Exception
	{
		ScriptEngineManager sem = new ScriptEngineManager();
		// 添加上下文绑定数据
		SimpleBindings bindings = new SimpleBindings();
		bindings.put("aaa_av", new Double(10000.1));
		bindings.put("bbb_av", new Double(20000.2));
		ScriptEngine engine = sem.getEngineByExtension("js");
		engine.setBindings(bindings, ScriptContext.ENGINE_SCOPE);
		try
		{
			// 直接解析
			Object res = engine.eval(" res = Math.max(aaa_av , bbb_av) + aaa_av");
			System.out.println(res);
			// 创建脚本
			String script = "function getMax() " + "{ return Math.max(aaa_av , bbb_av)}";
			// 执行脚本
			engine.eval(script);
			Invocable inv = (Invocable) engine;
			// 执行方法并传递参数
			Object obj = inv.invokeFunction("getMax", (Object[])null);
			// 打印结果
			System.out.println(obj);
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
		}
	}
	
	public void testJavaScript2() throws Exception
	{
		String[] data = {"打印结果", "徐工建设"};
		
		for(int i = 0; i < data.length; i++)
		{
			HexDump.dump(data[i].getBytes("ISO-8859-1"), 0, System.out, 0);
			System.out.println(showBin(data[i]));
			System.out.println(escapeJS(data[i]));
		}
	}
	
	private String showBin(String s)
	{
		StringBuilder sb = new StringBuilder();
		for(int i = 0; i < s.length(); i++)
		{
			char c = s.charAt(i);
			
			int hi = (int)((c >> 8) & 0x000000FF);
			int lo = (int)(c & 0x000000FF);
			
			sb.append(String.format("%02X", (int)hi));
			sb.append(String.format("%02X", (int)lo));
			sb.append(" ");
		}
		return sb.toString();
	}
	
	private String escapeJS(String s) throws ScriptException
	{
		ScriptEngineManager sem = new ScriptEngineManager();
		ScriptEngine engine = sem.getEngineByExtension("js");
		
		SimpleBindings bindings = new SimpleBindings();
		bindings.put("str", s);
		
		engine.setBindings(bindings, ScriptContext.ENGINE_SCOPE);
		
		Object res = engine.eval("escape(str)");
		if(res instanceof String)
		{
			return (String)res;
		}
		return res.toString();
	}
}
