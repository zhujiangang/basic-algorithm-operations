#include "StringOper.h"

unsigned int gstrlen(const char* str)
{
	unsigned int ret = 0;
	while(*str++)
	{
		ret++;
	}
	return ret;
}
char* gstrcat(char* dst, const char* src)
{
	char* ret = dst;
	while(*dst)
	{
		dst++;
	}
	while(*dst++ = *src++)
		;
	return ret;
}
char* gstrcpy(char* dst, const char* src)
{
	char* ret = dst;
	while(*dst++ = *src++)
		;
	return ret;
}

int gstrcmp(const char* str1, const char* str2)
{
	int ret = 0;
	while(*str1 == *str2 && *str1)
	{
		str1++;
		str2++;
	}
	ret = *(unsigned char*)str1 - *(unsigned char*)str2;
	if(ret > 0)
	{
		return 1;
	}
	else if(ret < 0)
	{
		return -1;
	}
	return 0;
}
void* gmemcpy(void* dst, const void* src, unsigned int count)
{
	void* ret = dst;

	while(count--)
	{
		*((char*)dst) = *((char*)src);
		dst = (char*)dst + 1;
		src = (char*)src + 1;
	}
	return ret;
}
void* gmemmove(void* dst, const void* src, unsigned int count)
{
	void* ret = dst;
	//Overlapped, copy from end to begin
	if( (char*)dst > (char*)src && (char*)dst < ((char*)src + count) )
	{
		while(count--)
		{
			*((char*)dst + count) = *((char*)src + count);
		}
	}
	else
	{
		while(count--)
		{
			*((char*)dst) = *((char*)src);
			dst = (char*)dst + 1;
			src = (char*)src + 1;
		}
	}
	return ret;
}
int gmemcmp(const void* buf1, const void* buf2, unsigned int count)
{
	int ret = 0;

	while(count--)
	{
		ret = *(unsigned char*)buf1 - *(unsigned char*)buf2;
		if(ret != 0)
		{
			break;
		}
		buf1 = (char*)buf1 + 1;
		buf2 = (char*)buf2 + 1;
	}

	if(ret > 0)
	{
		return 1;
	}
	else if(ret < 0)
	{
		return -1;
	}
	return 0;
}
void* gmemset(void* dst, int val, unsigned int count)
{
	void * ret = dst;
	while(count--)
	{
		*(char*)dst = (char)val;
		dst = (char*)dst + 1;
	}
	return ret;
}