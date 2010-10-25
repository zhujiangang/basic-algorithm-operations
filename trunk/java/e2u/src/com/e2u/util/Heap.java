package com.e2u.util;

import java.util.List;


public class Heap<E extends Comparable<E>>
{
	private E[] heapData = null;
	private int size = 0;
	private boolean isMinHeap;
	
	public Heap(List<? extends E> initData, int capacity, boolean minHeap)
	{
		@SuppressWarnings("unchecked")
		E[] es = (E[])new Comparable[capacity];
		heapData = es;
		
		size = 0;
		
		isMinHeap = minHeap;
		
		for(int i = 0, count = initData.size(); i < count; i++)
		{
			heapData[i] = initData.get(i);
		}
		size = initData.size();
		
		initHeap();
	}
	
	public Heap(List<? extends E> initData)
	{
		this(initData, initData.size(), true);
	}
	
	private void initHeap()
	{
		for(int i = size / 2 - 1; i >= 0; i--)
		{
			shiftDown(i);
		}
	}
	
	public void shiftUp(int pos)
	{
		int parent = (pos + 1) / 2 - 1;
		
		//has parent
		while(parent >= 0)
		{
			//parent is bigger than child, do nothing
			if(compare(heapData[parent], heapData[pos]) >= 0)
			{
				break;
			}
			ArrayUtil.swap(heapData, parent, pos);
			pos = parent;
			parent = (pos + 1) / 2 - 1;
		}
	}
	public void shiftDown(int pos)
	{
		int c = pos * 2 + 1;

		while(c < size)
		{
			if(c < size - 1 && compare(heapData[c], heapData[c + 1]) < 0)
			{
				c++;
			}

			if(compare(heapData[pos], heapData[c]) >= 0)
			{
				break;
			}
			ArrayUtil.swap(heapData, pos, c);
			pos = c;
			c = pos * 2 + 1;
		}
	}
	
	private int compare(E e1, E e2)
	{
		int result = e1.compareTo(e2);
		if(isMinHeap)
		{
			result = -result;
		}
		return result;
	}
	public int size()
	{
		return size;
	}
	public E remove()
	{
		if(size <= 0)
		{
			return null;
		}
		if(size == 1)
		{
			size--;
			return heapData[0];
		}
		
		E result = heapData[0];
		
		heapData[0] = heapData[size - 1];
		
		heapData[size - 1] = null;
		size--;
		
		shiftDown(0);	
		
		return result;
	}
	public void insert(E e)
	{
		int pos = size;
		
		heapData[pos] = e;
		size++;
		
		shiftUp(pos);
	}
	
	public static void main(String[] args)
	{
		List<Integer> list = new java.util.ArrayList<Integer>();
		
		int data[] = {49, 38, 65, 97, 76, 13, 27, 49};
		for(int i = 0; i < data.length; i++)
		{
			list.add(data[i]);
		}
		
		Heap<Integer> hp = new Heap<Integer>(list, list.size(), false);
		
		int x, y, z;
		while(hp.size() > 1)
		{
			x = hp.remove();
			y = hp.remove();
			System.out.println("Remove :" + x + ", " + y);
			
			z = x + y;
			hp.insert(z);
			System.out.println("Insert :" + z);
		}
		if(hp.size() == 1)
		{
			System.out.println("Last : " + hp.remove());
		}
	}
}
