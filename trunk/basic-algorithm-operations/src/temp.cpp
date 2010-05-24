#include <stdio.h>   
#define	MAXN 100   
int	a[MAXN];   
int maxM = 0;
int maxK = 0;
void comb(int m, int k)   
{	
	if(k >= maxK - 1)
	{
		printf("[DEBUG]:called comb(%d, %d)\n", m, k);
	}

	int i,j;
	for(i=m;i>=k;i--)
	{	
		a[k]=i;   
		if(k>1)
			comb(i-1,k-1);   
		else   
		{	
			for(j=a[0];j>0;j--)  
				printf("%4d",a[j]);   
			printf("\n");   
		}   
	}   
}   

void main()   
{	
	int x = 4;
	a[0]=x; 
	maxM = 7;
	maxK = x;
	comb(maxM,x);   
}  