#include "BitOper.h"


bool find(const unsigned char* p, int len, unsigned int x, int* byteIndex, int* bitIndex)
{
	int i, j;
	unsigned int ret = 0;
	for(i = 0; i <= (len - 4); i++)
	{
		for(j = 0; j < 8; j++)
		{
			//Index out of bound
			if(i == (len - 4) && j > 0)
			{
				break;
			}
			ret = 0;
			ret |= (p[i] << (24 + j));
			ret |= (p[i + 1] << (16 + j));
			ret |= (p[i + 2] << (8 + j));
			ret |= (p[i + 3] << j);
			ret |= (p[i + 4] >> (8 - j));

			if(x == ret)
			{
				*byteIndex = i;
				*bitIndex = j;
				return true;
			}
		}
	}

	return false;
}