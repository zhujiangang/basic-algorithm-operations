package com.e2u.alg.misc;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


//http://www.kuqin.com/job/20080510/8301.html
//http://zhidao.baidu.com/question/38281904.html
/*
An army of ants walk on a horizontal pole of length l cm, 
each with a constant speed of 1 cm/s. When a walking ant reaches an end of the pole, 
it immediatelly falls off it. When two ants meet they turn back and start walking in 
opposite directions. We know the original positions of ants on the pole, unfortunately, 
we do not know the directions in which the ants are walking. Your task is to compute the 
earliest and the latest possible times needed for all ants to fall off the pole. 
The first line of input contains one integer giving the number of cases that follow. 
The data for each case start with two integer numbers: the length of the pole (in cm) and n, 
the number of ants residing on the pole. These two numbers are followed by n integers giving 
the position of each ant on the pole as the distance measured from the left end of the pole, 
in no particular order. All input integers are not bigger than 1000000 and they are separated 
by whitespace. 

For each case of input, output two numbers separated by a single space. 
The first number is the earliest possible time when all ants fall off the pole (if the 
directions of their walks are chosen appropriately) and the second number is the latest 
possible such time. 

Sample Input
2
10 3
2 6 7
214 7
11 12 7 13 176 23 191

Sample Output
4 8
38 207
*/
public class AntWalkOnPole
{

	private static boolean isAnyOnPole(List<Ant> antList)
	{
		for(int i = 0, size = antList.size(); i < size; i++)
		{
			if(antList.get(i).isOnPole())
			{
				return true;
			}
		}
		return false;
	}
	private static void move(List<Ant> antList)
	{
		Ant ant;
		for(int i = 0, size = antList.size(); i < size; i++)
		{
			ant = antList.get(i);
			if(ant.isOnPole())
			{
				ant.move();
			}
		}		
	}
	private static void initAntList(List<Ant> antList, int antsOrient, int[] positions, int antNumber, int poleLength)
	{
		Ant.setPoleLength(poleLength);
		antList.clear();
		
		int orient;
		for(int i = 0; i < antNumber; i++)
		{
			orient = (antsOrient >> (antNumber - i - 1)) & 1;
			Ant ant = new Ant(orient, positions[i]);
			antList.add(ant);
		}
	}
	private static void walk(int[] positions, int antNumber, int poleLength)
	{
		List<Ant> antList = new ArrayList<Ant>(antNumber);
		
		int time = 0;
		int min = Integer.MAX_VALUE;
		int minOrient = -1;
		int max = Integer.MIN_VALUE;
		int maxOrient = -1;
		
		for(int orient = 0; orient < (1 << antNumber); orient++)
		{
			initAntList(antList, orient, positions, antNumber, poleLength);
			time = 0;
			while(isAnyOnPole(antList))
			{
				move(antList);
				
				for(int j = 0; j < antNumber - 1; j++)
				{
					Ant ant1 = antList.get(j);
					Ant ant2 = antList.get(j + 1);
					
					ant1.meet(ant2);
				}
				time++;
			}
			if(time < min)
			{
				min = time;
				minOrient = orient;
			}
			if(time > max)
			{
				max = time;
				maxOrient = orient;
			}
			System.out.printf("orientation = %d, time = %d\n", orient, time);
		}
		System.out.printf("min = %d, minOrient = %d\n", min, minOrient);
		System.out.printf("max = %d, maxOrient = %d\n", max, maxOrient);
	}
	
	private static void testCase1()
	{
		int antNumber = 5;
		int length = 27;
		int positions[] = {3, 7, 11, 17, 23};
		walk(positions, antNumber, length);
	}
	private static void testCase2()
	{
		int antNumber = 3;
		int length = 10;
		int positions[] = {2, 6, 7};
		Arrays.sort(positions);
		
		walk(positions, antNumber, length);
	}
	private static void testCase3()
	{
		int antNumber = 7;
		int length = 214;
		int positions[] = {11,12,7,13,176,23,191};
		Arrays.sort(positions);
		
		walk(positions, antNumber, length);
	}
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		testCase1();
		testCase2();
		testCase3();
	}
}

class Ant
{
	public static final int ORIENT_LEFT  = 0;
	public static final int ORIENT_RIGHT = 1;
	
	private static int poleLength;
	
	private int orientation; //0-left;1-right;
	private int pos;
	

	public Ant(int orient, int pos)
	{
		this.orientation = orient;
		this.pos = pos;
	}
	
	public boolean isOnPole()
	{
		return pos > 0 && pos < poleLength;
	}
	
	public void turnBack()
	{
		orientation = (orientation == ORIENT_LEFT) ? ORIENT_RIGHT : ORIENT_LEFT;
	}
	
	public void move()
	{
		if(!isOnPole())
		{
			return;
		}
		if(orientation == ORIENT_LEFT)
		{
			pos--;
		}
		else if(orientation == ORIENT_RIGHT)
		{
			pos++;
		}
	}
	public void meet(Ant another)
	{
		if(this.orientation == another.orientation)
		{
			return;
		}
		if(this.pos - another.pos != -1)
		{
			return;
		}
		turnBack();
		another.turnBack();
	}
	
	public String toString()
	{
		return "pos = " + pos + ", orient = " + orientation;
	}
	
	public static void setPoleLength(int length)
	{
		poleLength = length;
	}
}
