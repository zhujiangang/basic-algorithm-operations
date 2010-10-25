package com.e2u.util;

import java.util.Random;  
public class ArrayUtil {  
    public static int[] generate(int n, int threshold)  
    {  
        return generate(n, threshold, true);  
    }  
    public static int[] generate(int n, int threshold, boolean isPos)  
    {  
        int[] ret = new int[n];  
        Random rand = new Random();  
          
        if(threshold <= 0)  
        {  
            for(int i = 0; i < n; i++)  
            {  
                ret[i] = rand.nextInt();  
                if(ret[i] < 0 && isPos)  
                {  
                    ret[i] *= -1;  
                }  
            }  
        }  
        else  
        {  
            for(int i = 0; i < n; i++)  
            {  
                ret[i] = rand.nextInt() % threshold;  
                if(ret[i] < 0 && isPos)  
                {  
                    ret[i] *= -1;  
                }  
            }  
        }  
          
        return ret;  
    }  
      
    public static int[] generateInSeq(int n, int start)  
    {  
        int[] ret = new int[n];  
        for(int i = 0; i < n; i++)  
        {  
            ret[i] = start + i;  
        }  
        return ret;  
    }  
      
    public static void output(int data[])  
    {  
        for(int i = 0; i < data.length; i++)  
        {  
            System.out.print(data[i] + " ");  
        }  
        System.out.println();  
    }  
      
    public static int find(int data[], int value)  
    {  
        for(int i = 0; i < data.length; i++)  
        {  
            if(data[i] == value)  
            {  
                return i;  
            }  
        }  
        return -1;  
    }  
      
    public static void swap(int data[], int i, int j)  
    {  
        int tmp = data[i];  
        data[i] = data[j];  
        data[j] = tmp;  
    }
    
    public static void swap(Object[] data, int i, int j)
    {
    	Object tmp = data[i];
    	data[i] = data[j];
    	data[j] = tmp;
    }
}  