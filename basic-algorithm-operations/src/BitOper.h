#ifndef _BIT_OPER_H_
#define _BIT_OPER_H_

bool find(const unsigned char* p, int len, unsigned int x, int* byteIndex, int* bitIndex);
void showBasicTypeInfo();


int doz(int a, int b);
int min(int a, int b);
int max(int a, int b);
int cmp(int a, int b);
int abs(int a);

void testBitOper();

#endif