package com.e2u.util;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * A heap is usually used as Priority-Queue. Etheir max-heap or min-heap
 * @param <E>
 */
public class Heap<E extends Comparable<E>>
{
	private ArrayList<E> heapData = null;
	private boolean isMinHeap;
	
	public Heap(List<? extends E> initData, boolean minHeap)
	{
		heapData = new ArrayList<E>(initData.size());		
		isMinHeap = minHeap;
		
		heapData.addAll(initData);		
		initHeap();
	}
	
	public Heap(List<? extends E> initData)
	{
		this(initData, true);
	}
	
	private void initHeap()
	{
		for(int i = size() / 2 - 1; i >= 0; i--)
		{
			shiftDown(i);
		}
	}
	public int size()
	{
		return heapData.size();
	}
	public boolean isLeaf(int pos)
	{
		return pos > (size() / 2 - 1);
	}
	public int leftChild(int pos)
	{
		return pos * 2 + 1;
	}
	public int rightChild(int pos)
	{
		return pos * 2 + 2;
	}
	
	public void shiftUp(int pos)
	{
		if(pos <= 0)
		{
			return;
		}
		int parent = (pos + 1) / 2 - 1;
		
		//has parent
		while(parent >= 0)
		{
			//parent is bigger than child, do nothing
			if(compare(heapData.get(parent), heapData.get(pos)) >= 0)
			{
				break;
			}
			Collections.swap(heapData, parent, pos);
			pos = parent;
			parent = (pos + 1) / 2 - 1;
		}
	}
	
	public void shiftDown(int pos)
	{
		int c = pos * 2 + 1;

		while(c < size())
		{
			if(c < size() - 1 && compare(heapData.get(c), heapData.get(c + 1)) < 0)
			{
				c++;
			}

			if(compare(heapData.get(pos), heapData.get(c)) >= 0)
			{
				break;
			}
			Collections.swap(heapData, pos, c);
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
	/**
	 * Peek the heap-top element.
	 * @return
	 */
	public E peek()
	{
		if(size() <= 0)
		{
			return null;
		}
		return heapData.get(0);
	}
	/**
	 * Get the heap-top element and remove it.
	 * @return
	 */
	public E remove()
	{
		if(size() <= 0)
		{
			return null;
		}
		if(size() == 1)
		{
			return heapData.remove(0);
		}
		
		E result = heapData.get(0);
		
		Collections.swap(heapData, 0, size() - 1);
		heapData.remove(size() - 1);
		
		shiftDown(0);	
		
		return result;
	}
	/**
	 * Insert an new element to the heap
	 * @param e
	 */
	public void insert(E e)
	{
		heapData.add(e);
		
		shiftUp(size() - 1);
	}
	
	/**
	 * Unit Test
	 * @param args
	 */
	public static void main(String[] args)
	{
		List<Integer> list = new java.util.ArrayList<Integer>();
		
		int data[] = {49, 38, 65, 97, 76, 13, 27, 49};
		for(int i = 0; i < data.length; i++)
		{
			list.add(data[i]);
		}
		
		Heap<Integer> hp = new Heap<Integer>(list, true);
		
		int x, y, z;
		while(hp.size() > 1)
		{
			x = hp.remove();
			y = hp.remove();
			System.out.println("Remove: " + x + ", " + y);
			
			z = x + y;
			hp.insert(z);
			System.out.println("Insert: " + z);
		}
		if(hp.size() == 1)
		{
			System.out.println("Last: " + hp.remove());
		}
		
		//Re-init with max-heap
		hp = new Heap<Integer>(list, false);
		while(hp.size() > 0)
		{
			x = hp.remove();
			System.out.println("Remove: " + x);
		}
	}
}
