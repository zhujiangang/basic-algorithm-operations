package com.e2u.classloader;

import java.lang.reflect.Method;
import java.util.Timer;
import java.util.TimerTask;



public class TestCustomClassLoader
{
    private Timer timer = null;

    public TestCustomClassLoader()
    {
        timer = new Timer();
        timer.schedule(new RefreshClassTimerTask(), 0, // initial delay
            3 * 1000); // subsequent rate
    }
    private static void loadClass(String[] classes)
    {
        try
        {
            CustomCL cl = new CustomCL("./swap", classes);
            for(int i = 0; i < classes.length; i++)
            {
                Class cls = cl.loadClass(classes[i]);
                Object foo = cls.newInstance();
                System.out.println("ClassLoader = " + cls.getClassLoader() + ", Class = " + cls);
                
                ObjectInf oi = (ObjectInf)foo;
                oi.sayHello();
            }            
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

    }
    public static void testLoadClass()
    {
        String[] classes = 
        { 
            //javax.* classes is ok
            "javax.net.SocketFactory",
            //Can't custom java.* classes
//            "java.lang.Object", 
//            "java.lang.String"
        };
        loadClass(classes);
    }
    
    public static void main(String[] args)
    {
//       TestCustomClassLoader autoUpdater = new TestCustomClassLoader();
       testLoadClass();
    }
}

class RefreshClassTimerTask extends TimerTask
{
    public void run()
    {
        try
        {
            // 每次都创建出一个新的类加载器
            CustomCL cl = new CustomCL("./swap", new String[] { "Foo" });
            Class cls = cl.loadClass("Foo");
            Object foo = cls.newInstance();

            Method m = foo.getClass().getMethod("sayHello", new Class[] {});
            m.invoke(foo, new Object[] {});

        }
        catch (Exception ex)
        {
            ex.printStackTrace();
        }
    }
}
