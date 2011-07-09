package com.e2u.thread.timeout;

import java.util.concurrent.TimeoutException;

public class TimeoutThread extends Thread
{
    private int requestTimeout = 10000;
    private Thread worker = null;

    private boolean active = true;
    private Object synch = new Object();

    private Exception returnedException;
    
    private static int count = 0;

    public TimeoutThread()
    {
        count++;
        this.setName("TimeOut Thread[" + count + "]");
        start();
    }
    public void setRequestTimeout(int requestTimeout)
    {
        this.requestTimeout = requestTimeout;
    }

    public int getRequestTimeout()
    {
        return requestTimeout;
    }

    /*
     * Stop the thread by setting the active flag to false. IMPORTANT : this
     * method has to be called for a proper cleanup The thread only terminates
     * after the return of any outstanding request.
     */
    public void setInactive()
    {
        synchronized (synch)
        {
            active = false;
            synch.notifyAll();
        }
    }

    public boolean isActive()
    {
        synchronized (synch)
        {
            return active;
        }
    }
    public void run()
    {
        while(isActive())
        {
//            try
//            {
                execute();
//            }
//            catch (InterruptedException e)
//            {
//                this.interrupt();
//            }
            
        }
    }
    public void execute()
    {
        synchronized (synch)
        {
            while ((worker == null) && (active == true))
            {
                try
                {
                    synch.wait();
                }
                catch (Exception e)
                {
                    // nothing to do
                }
            }

            if (active == false)
                return;
        }

        try
        {
            returnedException = null;
            worker.run();
        }
        catch (Exception e)
        {
            returnedException = e;
        }

        synchronized (synch)
        {
            active = false;
            worker = null;
            synch.notifyAll();
        }
    }
    public void invoke(Thread work) throws TimeoutException, Exception
    {
        synchronized (synch)
        {
            this.worker = work;
            synch.notifyAll();
        }

        synchronized (synch)
        {
            while (worker != null)
            {
                try
                {
                    synch.wait(getRequestTimeout());
                }
                catch (Exception e)
                {
                    // nothing to do
                }

                if (worker != null)
                {
                    interrupt();
                    // timeout
                    returnedException = new TimeoutException();
                    break;
                }
            }

            if (returnedException != null)
                throw returnedException;
        }
    }
}
