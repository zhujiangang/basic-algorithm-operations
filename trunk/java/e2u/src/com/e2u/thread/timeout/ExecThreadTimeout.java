package com.e2u.thread.timeout;

import java.util.concurrent.TimeoutException;

public class ExecThreadTimeout
{
    public static void execute(Thread task, long timeout) throws TimeoutException
    {
        task.start();
        try
        {
            task.join(timeout);
        }
        catch (InterruptedException e)
        {
            /* if somebody interrupts us he knows what he is doing */
        }
        if (task.isAlive())
        {
            task.interrupt();
            throw new TimeoutException();
        }
    }
}
