#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "FileOper.h"

void FindFile1(char * pFilePath)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char DirSpec[MAX_PATH + 1];// 指定路径
    DWORD dwError;
	
    strncpy (DirSpec, pFilePath, strlen(pFilePath) + 1);
    strncat (DirSpec, "\\*", 3);
	
    hFind = FindFirstFile(DirSpec, &FindFileData);
	
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("Invalid file handle. Error is %u ", GetLastError());
        return ;
    }
    else
    {
        if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
        {
            printf ("%s\\%s\n", pFilePath, FindFileData.cFileName);   //找到文件
        }
        else if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
            && strcmp(FindFileData.cFileName, ".") != 0
            && strcmp(FindFileData.cFileName, "..") != 0)
        {   //找到目录
            char Dir[MAX_PATH + 1];
            strcpy(Dir, pFilePath);
            strncat(Dir, "\\", 2);
			strcat(Dir, FindFileData.cFileName);
			
            FindFile(Dir);
        }
		
        while (FindNextFile(hFind, &FindFileData) != 0)
        {
            if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
            {   //找到文件
                printf ("%s\\%s\n", pFilePath, FindFileData.cFileName);   //
            }
            else if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
                && strcmp(FindFileData.cFileName, ".") != 0
                && strcmp(FindFileData.cFileName, "..") != 0)
            { //找到目录
                char Dir[MAX_PATH + 1];
                strcpy(Dir, pFilePath);
                strncat(Dir, "\\", 2);
				strcat(Dir, FindFileData.cFileName);
                FindFile(Dir);
            }
			
        }
		
        dwError = GetLastError();
        FindClose(hFind);
        if (dwError != ERROR_NO_MORE_FILES)
        {
            printf ("FindNextFile error. Error is %u ", dwError);
            return;
        }
    }
}


void FindFile2(char * pFilePath)
{
    char DirSpec[MAX_PATH + 1];// 指定
	strncpy (DirSpec, pFilePath, strlen(pFilePath) + 1);
    strncat (DirSpec, "\\*", 3);

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(DirSpec, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("Invalid file handle. Error is %u ", GetLastError());
        return ;
    }

    DWORD dwError;
	BOOL hasMore;  

	for(hasMore = (hFind != INVALID_HANDLE_VALUE); hasMore; hasMore = FindNextFile(hFind, &FindFileData))
	{
		//1. File
		if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
        {
            printf ("%s\\%s\n", pFilePath, FindFileData.cFileName);   //找到文件
        }
		else if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
            && strcmp(FindFileData.cFileName, ".") != 0
            && strcmp(FindFileData.cFileName, "..") != 0)
		{
			//找到目录
            char Dir[MAX_PATH + 1];
			sprintf(Dir, "%s\\%s", pFilePath, FindFileData.cFileName);
			
            FindFile(Dir);
		}
	}
	dwError = GetLastError();
	FindClose(hFind);
    if (dwError != ERROR_NO_MORE_FILES)
    {
		printf ("FindNextFile error. Error is %u ", dwError);
        return;
    }
}


void FindFile(char * pFilePath)
{
    char DirSpec[MAX_PATH + 1];
	sprintf(DirSpec, "%s\\*", pFilePath);
	
	DWORD dwError;
	BOOL hasMore; 

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(DirSpec, &FindFileData);
	
	if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("Invalid file handle. Error is %u ", GetLastError());
        return ;
    }
	
     
	
	for(hasMore = (hFind != INVALID_HANDLE_VALUE); hasMore; hasMore = FindNextFile(hFind, &FindFileData))
	{
		//1. File
		if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
        {
            printf ("%s\\%s\n", pFilePath, FindFileData.cFileName);   //找到文件
        }
	}
	dwError = GetLastError();
	FindClose(hFind);
    if (dwError != ERROR_NO_MORE_FILES)
    {
		printf ("FindNextFile error. Error is %u ", dwError);
        return;
    }


	//Fine directories
	hFind = FindFirstFile(DirSpec, &FindFileData);
	
	if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("Invalid file handle. Error is %u ", GetLastError());
        return ;
    }
	
	for(hasMore = (hFind != INVALID_HANDLE_VALUE); hasMore; hasMore = FindNextFile(hFind, &FindFileData))
	{
		if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
            && strcmp(FindFileData.cFileName, ".") != 0
            && strcmp(FindFileData.cFileName, "..") != 0)
		{
			//找到目录
            char Dir[MAX_PATH + 1];
			sprintf(Dir, "%s\\%s", pFilePath, FindFileData.cFileName);
			
            FindFile(Dir);
		}
	}
	dwError = GetLastError();
	FindClose(hFind);
    if (dwError != ERROR_NO_MORE_FILES)
    {
		printf ("FindNextFile error. Error is %u ", dwError);
        return;
    }
}


int writeIntToFile(const char* filename, int value)
{
	FILE* pf = fopen(filename, "wb");

	if(pf == NULL)
	{
		printf("open file %s failed.\n", filename);
		return -1;
	}

	//converts a int value to a byte array
	char buf[4];
	unsigned int x = (unsigned int)value;
	buf[0] = (x & 0xFF);
	buf[1] = ((x >> 8) & 0xFF);
	buf[2] = ((x >> 16) & 0xFF);
	buf[3] = ((x >> 24) & 0xFF);

	size_t len = fwrite(buf, 1, 4, pf);

	if(len != 4)
	{
		printf("write failed.\n");
		fclose(pf);
		return -2;
	}

	fclose(pf);
	return 0;
}

int readIntFromFile(const char* filename, int* value, int offset)
{
	FILE* pf = fopen(filename, "rb");
	
	if(pf == NULL)
	{
		printf("open file %s failed.\n", filename);
		return -1;
	}

	fseek(pf, offset, SEEK_SET);
	
	//converts a int value to a byte array
	char buf[4];
	size_t len = fread(buf, 1, 4, pf);

	
	if(len != 4)
	{
		printf("read failed.\n");
		fclose(pf);
		return -2;
	}

	unsigned int x = ( (buf[3] & 0xFF) << 24) | ( (buf[2] & 0xFF) << 16) | ( (buf[1] & 0xFF) << 8) | (buf[0] & 0xFF);
	*value = (int)x;
	
	fclose(pf);
	return 0;
}