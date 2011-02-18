#include <stdio.h>
#include <string.h>
#include "StringOper.h"
#include "MyUtil.h"
#include "config.h"

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

void testStringOper()
{
#if ((STRING_OPER_TEST) == 1)
	char string1[60] = "The quick brown dog jumps over the lazy fox";
	char string2[60] = "The quick brown fox jumps over the lazy dog";
	/*                           1         2         3         4         5
	*                   12345678901234567890123456789012345678901234567890
	*/

	printf( "Function:\tmemcpy without overlap\n" );
	printf( "Source:\t\t%s\n", string1 + 40 );
	printf( "Destination:\t%s\n", string1 + 16 );
	gmemcpy( string1 + 16, string1 + 40, 3 );
	printf( "Result:\t\t%s\n", string1 );
	printf( "Length:\t\t%d characters\n\n", strlen( string1 ) );
	
	/* Restore string1 to original contents */
	gmemcpy( string1 + 16, string2 + 40, 3 );
	
	printf( "Function:\tmemmove with overlap\n" );
	printf( "Source:\t\t%s\n", string2 + 4 );
	printf( "Destination:\t%s\n", string2 + 10 );
	gmemmove( string2 + 10, string2 + 4, 40 );
	printf( "Result:\t\t%s\n", string2 );
	printf( "Length:\t\t%d characters\n\n", strlen( string2 ) );
	
	printf( "Function:\tmemcpy with overlap\n" );
	printf( "Source:\t\t%s\n", string1 + 4 );
	printf( "Destination:\t%s\n", string1 + 10 );
	gmemcpy( string1 + 10, string1 + 4, 40 );
	printf( "Result:\t\t%s\n", string1 );
	printf( "Length:\t\t%d characters\n\n", strlen( string1 ) );
	printSep(__FILE__);
#endif
}