package com.e2u.thread.readwriter;

/*
 * Created on 2005-1-9 TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
/*
 * @author Michelangelo TODO To change the template for this generated type
 * comment go to Window - Preferences - Java - Code Style - Code Templates
 */
public class Writer extends Thread
{
    private Database server;
    private int writerNum;
    /**
 * 
 */
    public Writer(int w, Database db)
    {
        super();
        writerNum = w;
        server = db;
        // TODO Auto-generated constructor stub
    }
    public void run()
    {
        while (true)
        {
            System.out.println("Writer " + writerNum + " is sleeping");
            Database.napping();
            System.out.println("Writer " + writerNum + " wants to write");
            server.startWriting();

            System.out.println("Writer " + writerNum + " is writing");
            Database.napping();
            server.endWriting();

            System.out.println("It is Writer " + writerNum + " who has done writing .");

        }

    }
}
