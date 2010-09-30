package com.e2u.serialize;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class SerializationDemo
{
	public static void main(String args[])
	{
		// Object serialization
		try
		{
			MyClass object1 = new MyClass("Hello", -7, 2.7e10);
			ArrayList<MyClass> list = new ArrayList<MyClass>();
			for(int i = 0; i < 10; i++)
			{
				list.add(new MyClass("Hello" + i, 100 + i, 110.6789d + i));
			}
			System.out.println("object1:" + object1);
			System.out.println("list1:" + list);
			FileOutputStream fos = new FileOutputStream("serial");
			ObjectOutputStream oos = new ObjectOutputStream(fos);
			oos.writeObject(object1);
			oos.writeObject(list);
			oos.flush();
			oos.close();
		}
		catch(Exception e)
		{
			System.out.println("Exception during serialization:" + e);
			System.exit(0);
		}
		// Object deserialization
		try
		{
			MyClass object2;
			FileInputStream fis = new FileInputStream("serial");
			ObjectInputStream ois = new ObjectInputStream(fis);
			object2 = (MyClass) ois.readObject();
			List list = (List) ois.readObject();
			ois.close();
			System.out.println("object2:" + object2);
			System.out.println("list2:" + list);
		}
		catch(Exception e)
		{
			System.out.println("Exception during deserialization:" + e);
			System.exit(0);
		}
	}
}

class MyClass implements Serializable
{
	String s;
	int i;
	double d;

	public MyClass(String s, int i, double d)
	{
		this.s = s;
		this.i = i;
		this.d = d;
	}

	public String toString()
	{
		return "s=" + s + ";i=" + i + ";d=" + d;
	}
}
