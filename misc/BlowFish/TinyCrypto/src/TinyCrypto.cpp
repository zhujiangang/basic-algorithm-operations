#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#include <Wincrypt.h>
#include <assert.h>
#include "Blowfish.h"
#include "Base32.h"
#include "TinyCrypto.h"

#pragma comment(lib, "Advapi32.lib")

#define MD5_BYTE_LEN						16
#define ERROR_CODE_CALC_MD5_FAILED			-1
#define ERROR_CODE_BASE32_ENCODE_FAILED		-2
#define ERROR_CODE_BASE32_MAP_FAILED		-3
#define ERROR_CODE_BASE32_DECODE_FAILED		-4
#define ERROR_CODE_BASE32_UNMAP_FAILED		-5
#define ERROR_CODE_VALIDATION_FAILED		-6

static char alphabet[] = "123456789ABCDEFGHJKMNPQRSTUVWXYZ";

int CalcMD5(const unsigned char* in, int nInLen, unsigned char* out)
{
	HCRYPTPROV hCryptProv = NULL;
    HCRYPTHASH hHash = 0;
    BYTE pbData[0x7f];
    DWORD dwDataLen = MD5_BYTE_LEN; // The MD5 algorithm always returns 16 bytes.
	int result = 0;

    if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
    {
        if (CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
        {
            if (CryptHashData(hHash, in, nInLen, 0))
            {
                if (CryptGetHashParam(hHash, HP_HASHVAL, pbData, &dwDataLen, 0))
                {
					assert(dwDataLen == MD5_BYTE_LEN);
					memcpy(out, pbData, dwDataLen);
					result = dwDataLen;
                }
            }
        }
    }
	
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
	
    return result;
}

int Encrypt(const unsigned char* in, int nInLen, unsigned char* out, int nOutLen, unsigned char* key, int nKeyLen)
{
	unsigned char md5str[MD5_BYTE_LEN];
	unsigned char cipher[MD5_BYTE_LEN];
	int nResult = 0;
	
	//1. Calculate the MD5 of the key
	nResult = CalcMD5(key, nKeyLen, md5str);
	if(nResult != MD5_BYTE_LEN)
	{
		return ERROR_CODE_CALC_MD5_FAILED;
	}
	CBlowFish blowfish(md5str, MD5_BYTE_LEN);

	//2. Calculate the MD5 of the input contents
	nResult = CalcMD5(in, nInLen, md5str);
	if(nResult != MD5_BYTE_LEN)
	{
		return ERROR_CODE_CALC_MD5_FAILED;
	}
	blowfish.Encrypt(md5str, cipher, MD5_BYTE_LEN);

	//3. Convert the cipher text to Base32
	int nBase32Len = Base32::GetEncode32Length(MD5_BYTE_LEN);
	assert(nOutLen >= nBase32Len);
	if(!Base32::Encode32(cipher, MD5_BYTE_LEN, out))
	{
		return ERROR_CODE_BASE32_ENCODE_FAILED;
	}
	if(!Base32::Map32(out, nBase32Len, (unsigned char*)alphabet))
	{
		return ERROR_CODE_BASE32_MAP_FAILED;
	}

	return nBase32Len;
}
int Validate(const unsigned char* in, int nInLen, const unsigned char* pBase32, int nBase32Len, unsigned char* key, int nKeyLen)
{
	unsigned char md5str[MD5_BYTE_LEN];
	unsigned char cipher[MD5_BYTE_LEN];
	int nResult = 0;
	
	//1. Calculate the MD5 of the key
	nResult = CalcMD5(key, nKeyLen, md5str);
	if(nResult != MD5_BYTE_LEN)
	{
		return ERROR_CODE_CALC_MD5_FAILED;
	}
	CBlowFish blowfish(md5str, MD5_BYTE_LEN);
	
	//2. Calculate the MD5 of the input contents
	nResult = CalcMD5(in, nInLen, md5str);
	if(nResult != MD5_BYTE_LEN)
	{
		return ERROR_CODE_CALC_MD5_FAILED;
	}
	blowfish.Encrypt(md5str, cipher, MD5_BYTE_LEN);
	
	//3. Convert Base32 input to Byte string
	int nByteLen = Base32::GetDecode32Length(nBase32Len);
	assert(nByteLen == MD5_BYTE_LEN);
	assert(nBase32Len == 26);

	unsigned char pbData[26];
	memcpy(pbData, pBase32, nBase32Len);
	if(!Base32::Unmap32(pbData, nBase32Len, (unsigned char*)alphabet))
	{
		return ERROR_CODE_BASE32_UNMAP_FAILED;
	}
	unsigned char base32_decoded[MD5_BYTE_LEN];
	if(!Base32::Decode32(pbData, nBase32Len, base32_decoded))
	{
		return ERROR_CODE_BASE32_DECODE_FAILED;
	}

	//4. compare the base32_decoded with the encrypted data
	if(memcmp(cipher, base32_decoded, MD5_BYTE_LEN) != 0)
	{
		return ERROR_CODE_VALIDATION_FAILED;
	}
	
	return 1;
}
