package com.e2u.game;

import java.util.LinkedList;
import java.util.Random;

class LinkUpMap
{
	public int x = 0, y = 0;
	public int prev_x, prev_y;
	public int picIndex = 0;
	public int minCrossing = LinkUpFindPath.MAX_INT_VALUE;
	public int minDistance = LinkUpFindPath.MAX_INT_VALUE;
	public LinkUpMap(int x, int y)
	{
		this.x = x;
		this.y = y;
		picIndex = 0;
		minCrossing = LinkUpFindPath.MAX_INT_VALUE;
		minDistance = LinkUpFindPath.MAX_INT_VALUE;
	}
	public String toString()
	{
		return "x=" + x + ",y=" + y + ",minCrossing=" + minCrossing + ",minDistance=" + minDistance + ",picIndex=" + picIndex;
	}
}
public class LinkUpFindPath
{
	private int rowNum = 3;
	private int colNum = 4;
	private static final int MAX_PIC_NUMBER = 3;
	public static final int MAX_INT_VALUE = 99;
	
	private static final int[][] directions = 
	{
		//left<--
		{-1, 0},
		//top
		{0, -1},
		//-->right
		{1, 0},
		//bottom
		{0, 1}
	};
	private LinkUpMap[][] map;
	
	private int[] picNumHash = null;
	
	public LinkUpFindPath(int row, int col)
	{
		this.rowNum = row;
		this.colNum = col;
		
		map = new LinkUpMap[virtualRow()][virtualCol()];
		for(int i = 0; i < virtualRow(); i++)
		{
			for(int j = 0; j < virtualCol(); j++)
			{
				map[i][j] = new LinkUpMap(j, i);
			}
		}
		
		picNumHash = new int[MAX_PIC_NUMBER];
	}
	public void initMap()
	{	
		for(int i = 0; i < MAX_PIC_NUMBER; i++)
		{
			picNumHash[i] = 0;
		}
		
		Random random = new Random();
		int rand = 0;
		int numPerPic = (rowNum * colNum) / MAX_PIC_NUMBER;
		for(int i = 1; i <= rowNum; i++)
		{
			for(int j = 1; j <= colNum; j++)
			{				
				while(true)
				{
					rand = random.nextInt(MAX_PIC_NUMBER);
//					if(rand % 3 == 0)
//					{
//						break;
//					}
					if(picNumHash[rand] < numPerPic)
					{
						map[i][j].picIndex = rand + 1;
						picNumHash[rand]++;
						break;
					}
				}				
			}
		}
		
		System.out.println();
	}
	public void showMap(int field)
	{
		for(int i = 0; i < virtualRow(); i++)
		{
			for(int j = 0; j < virtualCol(); j++)
			{
				//picIndex
				if(field == 0)
				{
					if(map[i][j].picIndex < 10)
					{
						System.out.print(" ");
					}
					System.out.print(map[i][j].picIndex);
					System.out.print(" ");					
				}
				//minCrossing
				else if(field == 1)
				{
					if(map[i][j].minCrossing < 10)
					{
						System.out.print(" ");
					}
					System.out.print(map[i][j].minCrossing);
					System.out.print(" ");						
				}
				//minDistance
				else if(field == 2)
				{
					if(map[i][j].minDistance < 10)
					{
						System.out.print(" ");
					}
					System.out.print(map[i][j].minDistance);
					System.out.print(" ");						
				}

			}
			System.out.println();
		}
	}
	
	public void calMinCrosssing(int startX, int startY)
	{
		for(int i = 0; i < rowNum; i++)
		{
			for(int j = 0; j < colNum; j++)
			{
				map[i][j].minCrossing = MAX_INT_VALUE;
				map[i][j].minDistance = MAX_INT_VALUE;
			}
		}
		//Temp make the minCrossing equals to -1
		map[startY][startX].minCrossing = -1;
		map[startY][startX].minDistance = 0;
		map[startY][startX].prev_x = 0;
		map[startY][startX].prev_y = 0;
		
		LinkedList<LinkUpMap> que = new LinkedList<LinkUpMap>();
		que.add(map[startY][startX]);
		
		LinkUpMap linkUpMap = null;
		int x, y, curMinCrossing, curMinDistance, distance;
		
		while(!que.isEmpty())
		{
			linkUpMap = que.removeFirst();
			
			x = linkUpMap.x;
			y = linkUpMap.y;
			curMinCrossing = linkUpMap.minCrossing;
			curMinDistance = linkUpMap.minDistance;
			
			//4 directions
			for(int d = 0; d < directions.length; d++)
			{
				distance = curMinDistance + 1;
				x = linkUpMap.x + directions[d][0];
				y = linkUpMap.y + directions[d][1];
				while(checkBounds(x, y))
				{
					//visited, the previous block has less crossing. Better
					if(map[y][x].minCrossing < curMinCrossing + 1)
					{
						break;
					}
					//visited, the previous block has the same crossing count.
					if(map[y][x].minCrossing == curMinCrossing + 1)
					{
						//This means the current block has a bigger distance than the previous one
						if(map[y][x].minDistance <= distance)
						{
							break;
						}
						else
						{
							System.out.println("[Update Distance]: " + map[y][x]);
						}
					}
					map[y][x].minDistance = distance++;
					map[y][x].minCrossing = curMinCrossing + 1;
					map[y][x].prev_x = x - directions[d][0];
					map[y][x].prev_y = y - directions[d][1];
					if(map[y][x].picIndex != 0)
					{
						break;
					}
					que.add(map[y][x]);									
					x += directions[d][0];
					y += directions[d][1];
				}
			}
		}
		
		//Restore
		map[startY][startX].minCrossing = 0;
	}
	
	public void showPath(int fromX, int fromY, int toX, int toY)
	{
		int x = toX, y = toY;
		int tempX, tempY;
		while(x != fromX || y != fromY)
		{
			System.out.printf("(%d,%d) ", x, y);
			tempX = map[y][x].prev_x;
			tempY = map[y][x].prev_y;
			x = tempX;
			y = tempY;
		}
		System.out.println();
	}
	
	public void findPath(int x1, int y1, int x2, int y2)
	{
		
	}
	
	public boolean checkLinkCondition(int x1, int y1, int x2, int y2)
	{
		return map[x1][y1].picIndex == map[x2][y2].picIndex;
		//return (map[x1][y1] == map[x2][y2]) || (Math.abs(map[x1][y1] - map[x2][y2]) ==  (MAX_PIC_NUMBER / 2));
	}
	
	private boolean checkBounds(int x, int y)
	{
		return (x >= 0 && x < virtualCol()) && (y >= 0 && y < virtualRow());
	}
	
	private int virtualRow()
	{
		return rowNum + 2;
	}
	private int virtualCol()
	{
		return colNum + 2;
	}
	
	public static void main(String[] args)
	{
		LinkUpFindPath linkUp = new LinkUpFindPath(3, 4);
		linkUp.initMap();
		linkUp.showMap(0);
		System.out.println();
		linkUp.calMinCrosssing(2, 1);
		linkUp.showMap(1);
		System.out.println();
		linkUp.showPath(2, 1, 4, 3);
		System.out.println();
		linkUp.showMap(2);
	}
}
