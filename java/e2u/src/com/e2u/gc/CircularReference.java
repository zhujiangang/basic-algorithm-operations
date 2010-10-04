package com.e2u.gc;

public class CircularReference
{
    private static boolean isTestThreadAlive = true;
    private static int counter = 0;
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
        }
        monitorGC();
        
        System.out.println("\nBefore exit test2 method");
    }
    public static void test3()
    {
        {
            Node a = new Node("a", null), b = new Node("b", a), c = new Node("c", b);
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
    private static int counter = 0;
    private static int availableCount = 0;
    private int index = 0;
    public Finalizable()
    {
        index = ++counter;
        availableCount++;
    }
    public void finalize()
    {
        availableCount--;

        String className = getClass().getSimpleName();
        String currentThreadName = Thread.currentThread().getName();
        System.out.println(className + " finalized, index = " + index + ", AC = " + availableCount +
            ", Current Thread = " + currentThreadName);
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
    public Node(Object o, Node n)
    {
        value = o;
        next = n;
    }
}
