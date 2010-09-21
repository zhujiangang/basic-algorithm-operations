package com.e2u.alg.misc;

/**
 * 一个农夫养了一头牛，三年后(第三年)，这头牛每年会生出1头牛，生出来的牛三年后，又可以每年生出一头牛…… 
 * 问农夫10年后有多少头牛?n年呢？ 
 * n = 1, cow = 1 
 * n = 2, cow = 1 
 * n = 3, cow = 2 
 * n = 4, cow = 3 
 * n = 5, cow = 4 
 * n = 6, cow = 6
 */

/*
 * 此题为斐波那契数列的变种 F(n) = F(n - 1) + F(n - 3)
 */

public class FeedCow
{
    public static void main(String[] args)
    {
    	int n = 20;
        System.out.println("Cow1: ");
        //Cow1
        for(int i = 3; i <= n; i++)
        {
            System.out.println("year = " + i + ", cows = " + Cow1.getCowCount(i));
        }
        System.out.println("\n\n===============\n\n");
        
        System.out.println("Cow2: ");
        //Cow2
        for(int i = 3; i <= n; i++)
        {
            System.out.println("year = " + i + ", cows = " + Cow2.getCowCount(i));
        }
        System.out.println("\n\n===============\n\n");
        
        System.out.println("Cow3: ");
        //Cow3
        for(int i = 3; i <= n; i++)
        {
            System.out.println("year = " + i + ", cows = " + Cow3.getCowCount(i));
        }
        System.out.println("\n\n===============\n\n");
    }
}

class Cow1
{
    static int count = 1;

    private static void feedCow(int year, int age, int n)
    {
        year++;
        age++;
        if (year <= n)
        {
        	if (age >= 3)
            {
                count++;
                feedCow(year, 0, n);
            }
            feedCow(year, age, n);
        }
    }
    public static int getCowCount(int n)
    {
        count = 1;
        feedCow(0, 0, n);
        return count;
    }
}

/**
 * 非常妙的面向对象思想解法
 *
 */
class Cow2
{
    public static int count = 0;

    public Cow2(int year, int n)
    {
        count++;
        for (int i = 3 + year; i <= n; i++)
        {
            new Cow2(i, n);
        }
    }

    public static int getCowCount(int n)
    {
        count = 0;
        new Cow2(0, n);
        return count;
    }
}
/**
 * 纯数学问题的计算，迭代计算
 *
 */
class Cow3
{
    public static int getCowCount(int n)
    {
        if(n < 0)
        {
            return 0;
        }
        if(n <= 2)
        {
            return 1;
        }
        
        int fn_3 = 1, fn_2 = 1, fn_1 = 1, fn = 1;
        
        for(int i = 3; i <= n; i++)
        {
            fn = fn_1 + fn_3;
            
            fn_3 = fn_2;
            fn_2 = fn_1;
            fn_1 = fn;
        }
        
        return fn;
    }
}

