package com.e2u.util;

public class OutputUtil {  
    public static void output(char c, int size)  
    {  
        for(int i = 0; i < size; i++)  
        {  
            System.out.print(c);  
        }  
        System.out.println();  
    }  
    public static void outputLineSeparator()  
    {  
        output('=', 60);  
    } 
    
    
}  
