package com.e2u.gc;

public class CircularReference
{
    private static boolean isTestThreadAlive = true;
    private static int counter = 0;
    
    private static Object dummyRef = new Object();
    public static void test1()
    {
        ClassB cb = null;
        cb = new ClassB(new ClassA());

        monitorGC();

        cb = null;
        monitorGC();

        new ClassB(new ClassA());
        monitorGC();

        System.out.println("\nBefore exit test1 method");
    }

    public static void test2()
    {
        {
            Node a = new Node("a", null), b = new Node("b", a), c = new Node("c", b);
            a.next = c;
            
            c.extension = dummyRef;
            b.extension = dummyRef;
            a.extension = dummyRef;
        }
        monitorGC();
        
        System.out.println("\nBefore exit test2 method");
    }
    public static void test3()
    {
        {
            Node a = new Node("A", null), b = new Node("B", a), c = new Node("C", b);
            a.next = c;

            a = null;
            b = null;
            c = null;
            
        }
        monitorGC();
        System.out.println("\nBefore exit test3 method");
    }

    private static void monitorGC()
    {
        System.out.println("\nGC: Before " + (++counter));
        System.gc();
        System.out.println("GC: After " + counter);
    }

    private static void runTestCaseInThread()
    {
        Thread t = new Thread(new Runnable()
        {
            public void run()
            {
                test1();
                test2();
                test3();
                isTestThreadAlive = false;
            }
        }, "TheTest");
        t.start();
    }
    /*
     * @param args
     */
    public static void main(String[] args)
    {
        runTestCaseInThread();

        while (isTestThreadAlive)
        {
            try
            {
                Thread.sleep(1000);
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }
        }
        System.out.println("\n\nMain Thread exit");
    }
}

class Finalizable
{
    public void finalize()
    {
        String className = getClass().getSimpleName();
        String currentThreadName = Thread.currentThread().getName();
        System.out.println(className + " " + this + " finalized, Current Thread = " + currentThreadName);
        try
        {
            super.finalize();
        }
        catch (Throwable e)
        {
            e.printStackTrace();
        }
    }
}

class ClassA extends Finalizable
{
    private ClassB refB;
    public ClassA()
    {
        refB = null;
    }
    public void setRefB(ClassB refB)
    {
        this.refB = refB;
    }
}

class ClassB extends Finalizable
{
    private ClassA refA;
    public ClassB(ClassA ca)
    {
        refA = ca;
        refA.setRefB(this);
    }
}

class Node extends Finalizable
{
    public Object value;
    public Node next;
    public Object extension = null;
    public Node(Object o, Node n)
    {
        value = o;
        next = n;
    }
    public String toString()
    {
    	return "Node " + value;
    }
}
