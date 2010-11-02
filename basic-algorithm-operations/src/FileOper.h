#ifndef _FILE_OPER_H_
#define _FILE_OPER_H_

// C
#ifdef __cplusplus
extern "C" {
#endif

#define BUFFER_END_INCLUDED 0x00000001
#define BUFFER_BEG_INCLUDED 0x00000010

typedef void (*FileDataProcessor)(char* pData, __int64 len, unsigned int posMask);
typedef int (*ReadFileData)(const char* lpFileName, __int64 offset, __int64 len, FileDataProcessor operCallBack);

void FindFile(char * pFilePath);

int writeIntToFile(const char* filename, int value);
int readIntFromFile(const char* filename, int* value, int offset = 0);

void empty(char* pData, __int64 len, unsigned int posMask = BUFFER_BEG_INCLUDED | BUFFER_END_INCLUDED);
void simpleIterator(char* pData, __int64 len, unsigned int posMask = BUFFER_BEG_INCLUDED | BUFFER_END_INCLUDED);
void printHeadTailIter(char* pData, __int64 len, unsigned int posMask = BUFFER_BEG_INCLUDED | BUFFER_END_INCLUDED);

int readFileByMap(const char* lpFileName, __int64 offset, __int64 len, FileDataProcessor operCallBack);
int readFileByIO(const char* lpFileName, __int64 offset, __int64 len, FileDataProcessor operCallBack);
int readEntireFile(const char* lpFileName, __int64 offset = 0, __int64 len = 0, FileDataProcessor operCallBack = empty);

// end extern C
#ifdef __cplusplus
}
#endif

#endif