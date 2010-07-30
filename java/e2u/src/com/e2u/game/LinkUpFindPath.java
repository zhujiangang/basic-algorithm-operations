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
        return "x=" + x + ",y=" + y + ",minCrossing=" + minCrossing + ",minDistance=" +
            minDistance + ",picIndex=" + picIndex;
    }
}

public class LinkUpFindPath
{
    private int rowNum = 3;
    private int colNum = 4;
    private static final int MAX_PIC_NUMBER = 6;
    public static final int MAX_INT_VALUE = 99;

    private static final int[][] directions = {
    // left<--
        { -1, 0 },
        // top
        { 0, -1 },
        // -->right
        { 1, 0 },
        // bottom
        { 0, 1 } };
    private LinkUpMap[][] map;

    private int[] picNumHash = null;
    private LinkedList[] blockIndexList = null;

    public LinkUpFindPath(int row, int col)
    {
        this.rowNum = row;
        this.colNum = col;

        map = new LinkUpMap[virtualRow()][virtualCol()];
        for (int i = 0; i < virtualRow(); i++)
        {
            for (int j = 0; j < virtualCol(); j++)
            {
                map[i][j] = new LinkUpMap(j, i);
            }
        }

        picNumHash = new int[MAX_PIC_NUMBER];
        
        blockIndexList = new LinkedList[MAX_PIC_NUMBER];
        
        for(int i = 0; i < MAX_PIC_NUMBER; i++)
        {
            blockIndexList[i] = new LinkedList();
        }
    }
    public void initMap()
    {
        for (int i = 0; i < MAX_PIC_NUMBER; i++)
        {
            picNumHash[i] = 0;
        }

        Random random = new Random();
        int rand = 0;
        int numPerPic = (rowNum * colNum) / MAX_PIC_NUMBER;
        for (int i = 1; i <= rowNum; i++)
        {
            for (int j = 1; j <= colNum; j++)
            {
                while (true)
                {
                    rand = random.nextInt(MAX_PIC_NUMBER);
                    if (picNumHash[rand] < numPerPic)
                    {
                        map[i][j].picIndex = rand + 1;
                        picNumHash[rand]++;
                        blockIndexList[rand].add(map[i][j]);
                        break;
                    }
                }
            }
        }

        System.out.println();
    }
    public void showMap(int field)
    {
        for (int i = 0; i < virtualRow(); i++)
        {
            for (int j = 0; j < virtualCol(); j++)
            {
                // picIndex
                if (field == 0)
                {
                    if (map[i][j].picIndex < 10)
                    {
                        System.out.print(" ");
                    }
                    System.out.print(map[i][j].picIndex);
                    System.out.print(" ");
                }
                // minCrossing
                else if (field == 1)
                {
                    if (map[i][j].minCrossing < 10)
                    {
                        System.out.print(" ");
                    }
                    System.out.print(map[i][j].minCrossing);
                    System.out.print(" ");
                }
                // minDistance
                else if (field == 2)
                {
                    if (map[i][j].minDistance < 10)
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
        for (int i = 0; i < virtualRow(); i++)
        {
            for (int j = 0; j < virtualCol(); j++)
            {
                map[i][j].minCrossing = MAX_INT_VALUE;
                map[i][j].minDistance = MAX_INT_VALUE;
            }
        }
        // Temp make the minCrossing equals to -1
        map[startY][startX].minCrossing = -1;
        map[startY][startX].minDistance = 0;
        map[startY][startX].prev_x = 0;
        map[startY][startX].prev_y = 0;

        LinkedList<LinkUpMap> que = new LinkedList<LinkUpMap>();
        que.add(map[startY][startX]);

        LinkUpMap linkUpMap = null;
        int x, y, curMinCrossing, curMinDistance, distance;

        while (!que.isEmpty())
        {
            linkUpMap = que.removeFirst();

            x = linkUpMap.x;
            y = linkUpMap.y;
            curMinCrossing = linkUpMap.minCrossing;
            curMinDistance = linkUpMap.minDistance;

            // 4 directions
            for (int d = 0; d < directions.length; d++)
            {
                distance = curMinDistance + 1;
                x = linkUpMap.x + directions[d][0];
                y = linkUpMap.y + directions[d][1];
                while (checkBounds(x, y))
                {
                    // visited, the previous block has less crossing. Better
                    if (map[y][x].minCrossing < curMinCrossing + 1)
                    {
                        break;
                    }
                    // visited, the previous block has the same crossing count.
                    if (map[y][x].minCrossing == curMinCrossing + 1)
                    {
                        // This means the current block has a bigger distance
                        // than the previous one
                        if (map[y][x].minDistance <= distance)
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
                    if (map[y][x].picIndex != 0)
                    {
                        break;
                    }
                    que.add(map[y][x]);
                    x += directions[d][0];
                    y += directions[d][1];
                }
            }
        }

        // Restore
        map[startY][startX].minCrossing = 0;
    }

    public void showPath(int fromX, int fromY, int toX, int toY)
    {
        int x = toX, y = toY;
        int tempX, tempY;
        while (x != fromX || y != fromY)
        {
            System.out.printf("(%d,%d) ", x, y);
            tempX = map[y][x].prev_x;
            tempY = map[y][x].prev_y;
            x = tempX;
            y = tempY;
        }
        System.out.printf("(%d,%d) ", fromX, fromY);
        System.out.println();
    }

    public boolean findPath(int x1, int y1, int x2, int y2)
    {
        return findPath(x1, y1, x2, y2, 2);
    }
    public boolean findPath(int x1, int y1, int x2, int y2, int maxCrossing)
    {
        for (int i = 0; i < virtualRow(); i++)
        {
            for (int j = 0; j < virtualCol(); j++)
            {
                map[i][j].minCrossing = MAX_INT_VALUE;
                map[i][j].minDistance = MAX_INT_VALUE;
                map[i][j].prev_x = -1;
                map[i][j].prev_y = -1;
            }
        }

        // Temp make the minCrossing equals to -1
        map[y1][x1].minCrossing = -1;
        map[y1][x1].minDistance = 0;

        LinkedList<LinkUpMap> que = new LinkedList<LinkUpMap>();
        que.add(map[y1][x1]);

        LinkUpMap linkUpMap = null;
        int x, y, curMinCrossing, distance;

        while (!que.isEmpty())
        {
            linkUpMap = que.removeFirst();
            
            //No need to calculate the blocks that has more crossings.
            if(linkUpMap.minCrossing >= maxCrossing)
            {
                que.clear();
                break;
            }

            x = linkUpMap.x;
            y = linkUpMap.y;
            curMinCrossing = linkUpMap.minCrossing;

            // 4 directions
            for (int d = 0; d < directions.length; d++)
            {              
                distance = linkUpMap.minDistance + 1;
                x = linkUpMap.x + directions[d][0];
                y = linkUpMap.y + directions[d][1];               
                
                while (checkBounds(x, y))
                {
                    // visited, the previous block has less crossing. Better
                    if (curMinCrossing + 1 > map[y][x].minCrossing)
                    {
                        break;
                    }
                    // visited, the previous block has the same crossing count.
                    if (curMinCrossing + 1 == map[y][x].minCrossing)
                    {
                        // This means the current block has a bigger distance
                        // than the previous one
                        if (distance >= map[y][x].minDistance)
                        {
                            break;
                        }
                    }
                    map[y][x].minDistance = distance++;
                    map[y][x].minCrossing = curMinCrossing + 1;
                    map[y][x].prev_x = x - directions[d][0];
                    map[y][x].prev_y = y - directions[d][1];
                    if (map[y][x].picIndex != 0)
                    {
                        break;
                    }
                    que.add(map[y][x]);
                    x += directions[d][0];
                    y += directions[d][1];
                }
            }
        }
        // Restore
        map[y1][x1].minCrossing = 0;
        
        return (map[y2][x2].minCrossing <= maxCrossing);
    }
    
    public boolean hint(boolean autoClear)
    {
        boolean result = false;
        for(int i = 0; i < MAX_PIC_NUMBER; i++)
        {
            LinkedList blockList = blockIndexList[i];
            
            for(int j = 0; j < blockList.size(); j++)
            {
                LinkUpMap linkUpMap1 = (LinkUpMap)blockList.get(j);
                for(int k = j + 1; k < blockList.size(); k++)
                {
                    LinkUpMap linkUpMap2 = (LinkUpMap)blockList.get(k);
                    
                    if(findPath(linkUpMap1.x, linkUpMap1.y, linkUpMap2.x, linkUpMap2.y))
                    {
                        System.out.print("Hint path: ");
                        showPath(linkUpMap1.x, linkUpMap1.y, linkUpMap2.x, linkUpMap2.y);
                        
                        if(autoClear)
                        {
                            linkUpMap1.picIndex = 0;
                            linkUpMap2.picIndex = 0;
                            
                            //System.out.print("Auto Cleared");
                        }
                        blockList.remove(k);
                        blockList.remove(j);
                        return true;
                    }
                }
            }
        }
        
        return result;
    }
    
    public void autoClear()
    {
        while(true)
        {
            boolean flag = false;
            for(int i = 0; i < MAX_PIC_NUMBER; i++)
            {
                if(!blockIndexList[i].isEmpty())
                {
                    flag = true;
                    break;
                }
            }
            if(!flag)
            {
                System.out.println("Finished clear");
                break;
            }
            if(!hint(true))
            {
                System.out.println("Can't clear now.");
                break;
            }
            else
            {
                System.out.println();
                showMap(0);
                System.out.println();
            }
        }
    }

    public boolean checkLinkCondition(int x1, int y1, int x2, int y2)
    {
        return map[x1][y1].picIndex == map[x2][y2].picIndex;
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
        LinkUpFindPath linkUp = new LinkUpFindPath(6, 4);
        linkUp.initMap();
        linkUp.showMap(0);
        System.out.println();
        
        /*
        linkUp.calMinCrosssing(2, 1);
        linkUp.showMap(1);
        System.out.println();
        linkUp.showPath(2, 1, 4, 3);
        System.out.println();
        linkUp.showMap(2);
        */
        
        linkUp.autoClear();
    }
}
