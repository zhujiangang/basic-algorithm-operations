// Lcs.cpp: implementation of the CLcs class.
//
//////////////////////////////////////////////////////////////////////

#include "config.h"
#include "MyUtil.h"
#include "Lcs.h"
#include "stdio.h"  
#include "string.h"  
#include "stdlib.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLcs::CLcs()
{

}

CLcs::~CLcs()
{

}

int CLcs::longest_common_substring(char *str1, char *str2)
{
	int i, j, len1, len2, max, max_i, max_j;
	len1 = strlen(str1);
	len2 = strlen(str2);
	
	//save the max length of the substring str1 and str2
	/*
	c[i][j] is the longest common string length with str1[i] and str2[j] as its last character
	*/
	int** c = new int*[len1 + 1];
	for(i = 0; i <= len1; i++)
	{
		c[i] = new int[len2 + 1];
	}
	for(i = 0; i <= len1; i++)
	{
		c[i][0] = 0;
	}
	for(i = 0; i <= len2; i++)
	{
		c[0][i] = 0;
	}

	max = -1;
	for(i = 1; i <= len1; i++)
	{
		for(j = 1; j <= len2; j++)
		{
			if(str1[i - 1] == str2[j - 1])
			{
				c[i][j] = c[i - 1][j - 1] + 1;
			}
			else
			{
				c[i][j] = 0;
			}

			if(c[i][j] > max)
			{
				max = c[i][j];
				max_i = i;
				max_j = j;
			}
		}
	}

	if(max > 0)
	{
		printf("lcs in str1: (%d-%d)\n", max_i - max, max_i - 1);
		for(i = max_i - max; i < max_i; i++)
		{
			printf("%c", str1[i]);
		}
		printf("\n");

		printf("lcs in str2: (%d-%d)\n", max_j - max, max_j - 1);
		for(i = max_j - max; i < max_j; i++)
		{
			printf("%c", str2[i]);
		}
		printf("\n");
	}

	for(i = 0; i <= len1; i++)
	{
		delete[] c[i];
	}
	delete[] c;

	return max;
}

int CLcs::longest_common_substring2(char *str1, char *str2)
{
	int i,k,len1,len2,len,s1_start,s2_start,idx,curmax,max;  
    len1 = strlen(str1);  
    len2 = strlen(str2);  
    len = len1 + len2;  
    max = 0;  
    for(i = 0 ; i < len ; i++)  
    {  
        s1_start = s2_start = 0;  
        if(i < len1)  
            s1_start = len1 - i;    //???????????  
        else  
            s2_start = i - len1;  
        curmax = 0;  
        for(idx = 0 ; ( s1_start + idx < len1 ) && ( s2_start + idx < len2 ); idx++ )  
        {  
            if(str1[s1_start+idx]==str2[s2_start+idx])  
                curmax++;  
            else     //????????,????????????,????,???curmax?max?????,??curmax???0  
            {  
                //max = curmax > max ? curmax : max;  
                if(curmax > max)  
                {  
                    max = curmax;  
                    k = s1_start+idx-1;      //??????????????????????str1?????????,??????????  
                }
                curmax = 0;  
            }  
        }  
        //max = curmax > max ? curmax : max;  
        if(curmax > max)  
        {  
            max = curmax;  
            k = s1_start+idx-1;  
        }  
    }  
	
    //??????  
    char s[1000];  
    for(i=0;i<max;i++)  
        s[i]=str1[k-max+1+i];     //?????str1?????????k-max+1,?????k  
    s[i]='\0';  
    printf("lcs string:");  
    puts(s);  
    return max;  
}

void testLCS()
{
#if LCS == 1
	char str1[1000] = "shaohuisfsfwfwefsdlajfeifjsalfje";
	char str2[1000] = "ahuifwef38893jfiekfleurteg";  
	int len = CLcs::longest_common_substring2(str1, str2);
	printf("length=%d\n", len);

	len = CLcs::longest_common_substring(str1, str2);
	printf("length=%d\n", len);

	printSep(__FILE__);
#endif
}
