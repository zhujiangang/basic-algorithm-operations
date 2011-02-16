#ifndef _STRING_OPER_H_
#define _STRING_OPER_H_

/************************************************************************/
/* Add a "g" prefix to differ with system functions in <string.h>       */
/************************************************************************/

unsigned int gstrlen(const char* str);
char* gstrcat(char* dst, const char* src);
char* gstrcpy(char* dst, const char* src);
int gstrcmp(const char* str1, const char* str2);
void* gmemcpy(void* dst, const void* src, unsigned int count);
void* gmemmove(void* dst, const void* src, unsigned int count);
int gmemcmp(const void* buf1, const void* buf2, unsigned int count);
void* gmemset(void* dst, int val, unsigned int count);
void testStringOper();
#endif