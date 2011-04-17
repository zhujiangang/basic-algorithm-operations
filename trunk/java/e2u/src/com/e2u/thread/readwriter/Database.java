package com.e2u.thread.readwriter;

/*
 * Created on 2005-1-9 TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
/*
 * @author Michelangelo TODO To change the template for this generated type
 * comment go to Window - Preferences - Java - Code Style - Code Templates
 */
public class Database
{
    /**
 * 
 */
    private static final int NAP_TIME = 5;
    private int readerCount;
    private int writerCount;
    private boolean dbReading;
    private boolean dbWriting;
    public Database()
    {
        super();
        readerCount = 0;
        writerCount = 0;
        dbReading = false;
        dbWriting = false;
        // TODO Auto-generated constructor stub
    }

    public static void napping()
    {
        int sleepTime = (int) (NAP_TIME * Math.random());
        try
        {
            Thread.sleep(sleepTime * 1000);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
    public synchronized int startRead()
    {
        while (writerCount > 0)
        {
            try
            {
                System.out.println("reader is waiting");
                wait();
            }
            catch (Exception e)
            {
                System.out.println(e.toString());
                e.printStackTrace();
            }
        }
        ++readerCount;
        if (readerCount == 1)
        {
            dbReading = true;
        }
        return readerCount;

    }
    public synchronized int endReading()
    {
        --readerCount;
        if (readerCount == 0)
        {
            dbReading = false;
        }
        notifyAll();
        System.out.println("one reader is done reading. Count=" + readerCount);
        return readerCount;
    }
    public synchronized void startWriting()
    {
        ++writerCount;
        while (dbReading == true || dbWriting == true)
        {
            try
            {
                System.out.println("Writer is waiting");
                wait();
            }
            catch (Exception e)
            {
                System.out.println(e.toString());
            }

        }
        dbWriting = true;
    }
    public synchronized void endWriting()
    {
        --writerCount;
        dbWriting = false;
        System.out.println("one writer is done writing. Count=" + writerCount);

        notifyAll();
    }

}
