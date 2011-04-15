package com.e2u.thread;

import java.util.*;


public class SerializedOperBooker
{
	private static boolean isLog = true;
	
	//Simple init way to avoid multi-threading violation.
	private static SerializedOperBooker instance = new SerializedOperBooker();
	
	//The book data
	private Map<String, LinkedList> bookMap = null;
	
	private SerializedOperBooker()
	{
		bookMap = new HashMap<String, LinkedList>();
	}
	public static SerializedOperBooker getInstance()
	{
		return instance;
	}
	
	private static void info(String str)
	{
		if(isLog)
		{
			System.out.println(str);
		}
	}
	
	public synchronized void book(String operObjName, Object bookeeID)
	{
		LinkedList que = null;
        //No other operations to this object
        if(!bookMap.containsKey(operObjName))
        {
        	que = new LinkedList();
        	bookMap.put(operObjName, que);
        }
        //there are some other operations to this object
        else
        {
        	que = bookMap.get(operObjName);
        }
        que.add(bookeeID);
        info("[book]: operObjName=" + operObjName + ", bookeeID=" + bookeeID);
	}
	public synchronized void unbook(String operObjName)
	{
		LinkedList que = bookMap.get(operObjName);
		if(que == null || que.isEmpty())
		{
			info("fatal error. que is " + que + ", when unbook " + operObjName);
			return;
		}
		que.remove();
		if(que.isEmpty())
		{
			bookMap.remove(operObjName);
		}
		info("[unbook]: operObjName=" + operObjName);
		notifyAll();
	}
	public synchronized void checkExecutable(String operObjName, Object bookeeID)
	{
		LinkedList que = bookMap.get(operObjName);
		if(que == null || que.isEmpty())
		{
			info("fatal error. que is " + que + ", when checkExecutable " + operObjName);
			return;
		}
		
		Object bookee = null;
    	for(bookee = que.peek(); bookee != bookeeID; bookee = que.peek())
    	{
    		//The bookee should not be null
    		if(bookee == null)
    		{
    			info("fatal error. bookee is null when checkExecutable " + operObjName);
    			return;    			
    		}
    		info("The object [" + operObjName + "] is now operated by [" + bookee + "], wait.");
    		try
			{
    			//Wait until the condition changed
    			wait();
			}
			catch(InterruptedException e)
			{
				//ignore
			}
    	}
    	info("It's the turn of [" + bookeeID + "] now for object [" + operObjName + "]");
	}
}
