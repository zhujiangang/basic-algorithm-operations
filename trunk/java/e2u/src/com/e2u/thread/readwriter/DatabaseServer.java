package com.e2u.thread.readwriter;

/*
 * Created on 2005-1-9 TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
/*
 * @author Michelangelo TODO To change the template for this generated type
 * comment go to Window - Preferences - Java - Code Style - Code Templates
 */
public class DatabaseServer
{
    /**
 * 
 */
    public DatabaseServer()
    {
        super();
        // TODO Auto-generated constructor stub
    }
    public static void main(String[] args)
    {
        Database db = new Database();
        Reader r1 = new Reader(1, db);
        Reader r2 = new Reader(2, db);
        Reader r3 = new Reader(3, db);
        Reader r4 = new Reader(4, db);
        Writer w1 = new Writer(1, db);
        Writer w2 = new Writer(2, db);

        r1.start();
        r2.start();
        r3.start();
        w1.start();
        r4.start();
        w2.start();

    }
}
