#ifndef _FILE_OPER_H_
#define _FILE_OPER_H_

void FindFile(char * pFilePath);

int writeIntToFile(const char* filename, int value);
int readIntFromFile(const char* filename, int* value, int offset = 0);

#endif