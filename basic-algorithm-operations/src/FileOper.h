#ifndef _FILE_OPER_H_
#define _FILE_OPER_H_


typedef void (*processFileData)(char* pData, unsigned int len, bool begin, bool end);
typedef int (*readFile)(const char* lpFileName, unsigned int offset, unsigned int len, processFileData operCallBack);

void FindFile(char * pFilePath);

int writeIntToFile(const char* filename, int value);
int readIntFromFile(const char* filename, int* value, int offset = 0);

void simpleIterator(char* pData, unsigned int len, bool begin = true, bool end = true);
void printHeadTailIter(char* pData, unsigned int len, bool begin = true, bool end = true);

int readFileByMap(const char* lpFileName, unsigned int offset, unsigned int len, processFileData operCallBack);
int readFileByIO(const char* lpFileName, unsigned int offset, unsigned int len, processFileData operCallBack);

#endif