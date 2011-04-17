package com.e2u.thread;

class SerialNum
{
    // The next serial number to be assigned
    public static int nextSerialNum = 0;

    @SuppressWarnings("unchecked")
    public static ThreadLocal serialNum = new ThreadLocal()
    {
        protected synchronized Object initialValue()
        {
            return new Integer(nextSerialNum++);
        }
    };

    public static int get()
    {
        return ((Integer) (serialNum.get())).intValue();
    }
}

public class ThreadLocalTest extends Thread
{
    public ThreadLocalTest(String name)
    {
        setName(name);
    }
    private int num;
    public void run()
    {
        num = SerialNum.get();
        print(SerialNum.get());
        
        SerialNum.serialNum.set(new Integer(num + 100));
        print(SerialNum.get());
        print(SerialNum.nextSerialNum);
    }
    private void print(int x)
    {
        print(String.valueOf(x));
    }
    private void print(String str)
    {
        System.out.println("[" + this.getName() + "]" + str);
    }
    public static void main(String[] args)
    {
        int len = 7;
        ThreadLocalTest[] tlt = new ThreadLocalTest[len];
        for(int i = 0; i < len; i++)
        {
            tlt[i] = new ThreadLocalTest(String.valueOf(i+1));
        }
        for(int i = 0; i < len; i++)
        {
            tlt[i].start();
        }
        try
        {
            sleep(10000);
        }
        catch (InterruptedException e1)
        {
            e1.printStackTrace();
        }
    }
}