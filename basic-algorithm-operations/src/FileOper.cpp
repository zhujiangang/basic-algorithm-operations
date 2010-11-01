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


void simpleIterator(char* pData, unsigned int len, bool begin, bool end)
{
	unsigned int i;
	char c;
	for(i = 0; i < len; i++)
	{
		c = *(pData + i);
	}
}

void printHeadTailIter(char* pData, unsigned int len, bool begin, bool end)
{
	unsigned int i;
	char c;
	for(i = 0; i < len; i++)
	{
		c = *(pData + i);
	}
	int x;
	if(begin && (len >= 2))
	{
		x = (int)(*(pData + 1)) & 0xFF;
		printf("%2X ", x);
	}


	if(end && (len >= 2))
	{
		x = (int)(*(pData + len - 2)) & 0xFF;
		printf("%2X ", x);
		printf("\n");
	}	
}

int readFileByMap(const char* lpFileName, unsigned int offset, unsigned int len, processFileData operCallBack)
{
	// Open the file
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("hFile is NULL\n");
		printf("Target file is %s\n", lpFileName);
		return 4;
	}
	
	SYSTEM_INFO SysInfo;          // system information; used to get the granularity
	DWORD dwSysGran;              // system allocation granularity
	// Get the system allocation granularity.
	GetSystemInfo(&SysInfo);
	dwSysGran = SysInfo.dwAllocationGranularity;
	
	DWORD dwFileSize;             // temporary storage for file sizes
	// Let the user know that the resulting file is more than large enough
	// for the experiment.
	dwFileSize = GetFileSize(hFile,  NULL);
	
	if(offset >= dwFileSize)
	{
		offset = 0;
	}
	if(len <= 0)
	{
		len = dwFileSize;
	}
	
	// Now calculate a few variables. Calculate the file offsets as
	// 64-bit values, and then get the low-order 32 bits for the
	// function calls.
	DWORD dwFileMapSize;          // size of the file mapping
	DWORD dwMapViewSize;          // the size of the view
	DWORD dwFileMapStart;         // where in the file to start the file map view
	// To calculate where to start the file mapping, round down the
	// offset of the data into the file to the nearest multiple of the
	// system allocation granularity. 
	dwFileMapStart = (offset / dwSysGran) * dwSysGran;
	
	// Calculate the size of the file mapping view.
	dwMapViewSize = (offset % dwSysGran) + len;
	
	// How large will the file-mapping object be?
	dwFileMapSize = offset + len;
	
	int iViewDelta;               // the offset into the view where the data shows up
	// The data of interest isn't at the beginning of the
	// view, so determine how far into the view to set the pointer.
	iViewDelta = offset - dwFileMapStart;
	
	
	HANDLE hMapFile;              // handle for the test file's memory-mapped region
	// Create a file-mapping object for the file.
	hMapFile = CreateFileMapping( hFile, // current file handle
		NULL, // default security
		PAGE_READWRITE, // read/write permission
		0,  // size of mapping object, high
		dwFileMapSize, // size of mapping object, low
		NULL); // name of mapping object
	
	if (hMapFile == NULL) {
		printf("hMapFile is NULL: last error: %d\n", GetLastError() );
		return 5;
	}
	
	LPVOID lpMapAddress;          // pointer to the base address of the memory-mapped region
	// Map the view and test the results.	
	lpMapAddress = MapViewOfFile(hMapFile, // handle to mapping object
		FILE_MAP_ALL_ACCESS, // read/write permission 
		0, // high-order 32 bits of file offset
		dwFileMapStart, // low-order 32 bits of file offset
		dwMapViewSize); // number of bytes to map
	if (lpMapAddress == NULL) {
		printf("lpMapAddress is NULL: last error: %d\n", GetLastError());
		return 6;
	}
	
	char* pData = (char *) lpMapAddress + iViewDelta;
	operCallBack(pData, len, true, true);
	
	// Close the file-mapping object and the open file.
	if (!UnmapViewOfFile(lpMapAddress)) 
	{ 
		printf("Could not unmap view of file. last error: %d\n", GetLastError());
	} 
	
	BOOL bFlag;                   // a result holder
	bFlag = CloseHandle(hMapFile); // close the file-mapping object
	
	if(!bFlag) 
	{
		printf("\nOops! Error # %ld occurred closing the mapping object!", GetLastError());
	}
	
	bFlag = CloseHandle(hFile);   // close the file itself
	
	if(!bFlag) 
	{
		printf("\nOops! Error # %ld occurred closing the file!", GetLastError());
	}
	
	return 0;
}

int readFileByIO(const char* lpFileName, unsigned int offset, unsigned int len, processFileData operCallBack)
{
	FILE* pf = fopen(lpFileName, "rb");
	
	if(pf == NULL)
	{
		printf("open file %s failed.\n", lpFileName);
		return -1;
	}

	long file_length = 0;

	fseek(pf, 0, SEEK_END);
	file_length = ftell(pf);
	rewind(pf);

	if(len == 0)
	{
		len = (size_t)file_length;
	}
	if(offset > len)
	{
		offset = 0;
	}

#define BUFFER_SIZE 4096
	char buffer[BUFFER_SIZE];
	
	size_t bytesRead = 0;  //actual bytes number by fread function returns
	size_t bytesOffset = 0;  //current file offset
	size_t bytesProcessed = 0; //how many bytes processed already
	while( (bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, pf)) > 0 )
	{
		//
		if( (bytesOffset + bytesRead) < offset )
		{
			bytesOffset += bytesRead;
			continue;
		}
		//Not start yet
		else if(bytesOffset <= offset)
		{
			size_t index = offset - bytesOffset;

			if(bytesOffset + bytesRead - offset >= len)
			{
				operCallBack(buffer + index, len, true, true);
				break;
			}
			else
			{
				operCallBack(buffer + index, bytesOffset + bytesRead - offset, true, false);				
				bytesProcessed += bytesOffset + bytesRead - offset;
				bytesOffset += bytesRead;
			}
		}
		//Started
		else
		{
			if(len - bytesProcessed <= bytesRead)
			{
				operCallBack(buffer, len - bytesProcessed, false, true);
				break;
			}
			else
			{
				operCallBack(buffer, bytesRead, false, false);
				bytesProcessed += bytesRead;
				bytesOffset += bytesRead;
			}
		}
	}
	
	fclose(pf);
	return 0;
}