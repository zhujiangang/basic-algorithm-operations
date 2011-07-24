#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TinyCrypto.h"

#pragma comment(lib, "TinyCrypto.lib")

char* key = "Key is in my hand.ddddddddddddddddd";
char* in = "Test Input Stringsdddddddddddddddddddddd";
char out[27] = { 0 };

void test_case();
void test_case1();
void test_case2();

int main(int argc, char* argv[])
{
	test_case1();
	test_case2();
	return 0;
}

void test_case()
{
	int nInLen = strlen(in);
	int nKeyLen = strlen(key);
	int nResult = 0;
	nResult = Encrypt((const unsigned char*)in, nInLen, (unsigned char*)out, 26, (unsigned char*)key, nKeyLen);
	if(nResult >= 26)
	{
		printf("%s\n", out);
	}
	nResult = Validate((const unsigned char*)in, nInLen, (unsigned char*)out, 26, (unsigned char*)key, nKeyLen);
	if(nResult > 0)
	{
		printf("Success\n");
	}
	else
	{
		printf("Failed\n");
	}
}

void test_case1()
{
	key = "123456088fsdsf";
	in = "xyz...123455";
	test_case();
}

void test_case2()
{
	key = "csdwwewew";
	in = "zxcdfvvfdeee";
	test_case();
}