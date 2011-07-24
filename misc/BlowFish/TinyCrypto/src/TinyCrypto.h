#ifndef _TINY_CRYPTO_H_
#define _TINY_CRYPTO_H_

#pragma once

int CalcMD5(const unsigned char* in, int nInLen, unsigned char* out);
int Encrypt(const unsigned char* in, int nInLen, unsigned char* out, int nOutLen, unsigned char* key, int nKeyLen);
int Validate(const unsigned char* in, int nInLen, const unsigned char* pBase32, int nBase32Len, unsigned char* key, int nKeyLen);

#endif