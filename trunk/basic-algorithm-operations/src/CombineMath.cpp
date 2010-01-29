#include <iostream.h>
#include "MyUtil.h"
#include "CombineMath.h"

void perm(int a[], int begin, int end)
{
	static int count = 0;
	if(begin == end)
	{
		count++;
		cout<<count<<") ";
		output(a, 0, end);
		return;
	}
	else
	{
		for(int i = begin; i <= end; i++)
		{
			swap(a[begin], a[i]);
			perm(a, begin + 1, end);
			swap(a[begin], a[i]);
		}
	}
}