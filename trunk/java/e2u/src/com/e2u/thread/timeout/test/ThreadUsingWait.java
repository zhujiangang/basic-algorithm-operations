package com.e2u.thread.timeout.test;

import java.util.concurrent.TimeoutException;
import com.e2u.thread.timeout.TimeoutThread;

public class ThreadUsingWait extends Thread
{
    private static int count = 0;
    public ThreadUsingWait()
    {
        super();
        count++;
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
                System.out.println("Caught InterruptedException here");
                this.interrupt();
                break;
            }
            System.out.println("Finish sleep 10 seconds");
        }
    }

    
    public static void main(String[] args)
    {
        ThreadUsingWait t1 = new ThreadUsingWait();
        
        TimeoutThread to = new TimeoutThread();
        try
        {
            to.invoke(t1);
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
