package com.e2u.thread.timeout.test;

import java.util.concurrent.TimeoutException;

import com.e2u.thread.timeout.ExecThreadTimeout;

public class ThreadUsingJoin extends Thread
{
    private static int count = 0;
    public ThreadUsingJoin()
    {
        super();
        count++;
        this.setName("Work Thread" + count);
    }
    public void run()
    {
        for(int i = 0; i < 1000; i++)
        {
            try
            {
                Thread.sleep(10000);
            }
            catch (InterruptedException e)
            {
                interrupt(); // very important
                break;
            }
        }
    }
    
    public static void main(String[] args)
    {
        ThreadUsingJoin t1 = new ThreadUsingJoin();
        try
        {
            ExecThreadTimeout.execute(t1, 13000);
        }
        catch (TimeoutException e)
        {
            System.out.println("TimeoutException");
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
        while(true)
        {
            try
            {
                Thread.sleep(30000);
            }
            catch (InterruptedException e)
            {
                Thread.currentThread().interrupt(); // very important
                break;
            }           
        }
    }
}
