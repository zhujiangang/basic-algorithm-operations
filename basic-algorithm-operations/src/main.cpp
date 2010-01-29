#include <iostream.h>

#include "MyUtil.h"
#include "CombineMath.h"
#include "ArraySort.h"

void testCombineMath()
{
	const int n = 4;
	int a[n];
	genseq(a, n);

	output(a, n);
	perm(a, 0, n - 1);
}


void testArraySort()
{
	const int n = 20000;
	int datasource[n], copya[n];
	genrand(datasource, n, n);
//	output(datasource, n);

	arryCopy(datasource, 0, copya, 0, n);
//	output(copya, n);

	sorter sorters[] = {bubbleSort, insertSort, mergeSort, quickSort, heapSort};
	
	int ret;
	clock_t start, finish;
	for(int i = 0; i < COUNTOF(sorters); i++)
	{
		start = clock();
		sorters[i](copya, n);
		finish = clock();

		if( (ret = validateInOrder(copya, n)) != -1)
		{
			cout<<"Failed ("<<i<<": "<<ret<<endl;
			//output(copya, n);
			break;
		}
		else
		{
			cout<<"Success ("<<i<<") time = "<<(finish - start)<<"("<<(double)(finish - start)/CLOCKS_PER_SEC<<"s)"<<endl;
		}
		arryCopy(datasource, 0, copya, 0, n);
	}
}
int main(int argc, char* argv[])
{
//	testCombineMath();
	testArraySort();
	return 0;
}

