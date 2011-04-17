package com.e2u.thread.readwriter;

/*
 * Created on 2005-1-9 TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
/*
 * @author Michelangelo TODO To change the template for this generated type
 * comment go to Window - Preferences - Java - Code Style - Code Templates
 */
public class Reader extends Thread
{
    /**
 * 
 */
    private Database server;
    private int readerNum;
    public Reader(int r, Database db)
    {
        super();
        readerNum = r;
        server = db;

        // TODO Auto-generated constructor stub
    }
    public void run()
    {
        int c;
        while (true)
        {
            System.out.println("reader " + readerNum + " is sleeping");
            Database.napping();
            System.out.println("reader " + readerNum + " wants to read");
            c = server.startRead();

            System.out.println("reader " + readerNum + " is reading. Count=" + c);
            Database.napping();
            c = server.endReading();
            System.out.println("It is reader " + readerNum +
                " who has done reading according to count=" + c);

        }
    }
}
