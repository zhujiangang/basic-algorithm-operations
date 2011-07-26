#ifndef _TINY_CRYPTO_H_
#define _TINY_CRYPTO_H_

#pragma once

#define ERROR_CODE_CALC_MD5_FAILED			-1
#define ERROR_CODE_BASE32_ENCODE_FAILED		-2
#define ERROR_CODE_BASE32_MAP_FAILED		-3
#define ERROR_CODE_BASE32_DECODE_FAILED		-4
#define ERROR_CODE_BASE32_UNMAP_FAILED		-5
#define ERROR_CODE_VALIDATION_FAILED		-6

int CalcMD5(const unsigned char* in, int nInLen, unsigned char* out);
int Encrypt(const unsigned char* in, int nInLen, unsigned char* out, int nOutLen, const unsigned char* key, int nKeyLen);
int Validate(const unsigned char* in, int nInLen, const unsigned char* pBase32, int nBase32Len, const unsigned char* key, int nKeyLen);

#endif