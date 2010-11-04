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

void empty(char* pData, __int64 len, unsigned int posMask)
{
}

void simpleIterator(char* pData, __int64 len, unsigned int posMask)
{
	__int64 i;
	char c;
	for(i = 0; i < len; i++)
	{
		c = *(pData + i);
	}
}

void printHeadTailIter(char* pData, __int64 len, unsigned int posMask)
{
	__int64 i;
	char c;
	for(i = 0; i < len; i++)
	{
		c = *(pData + i);
	}
	int x;
	int offset = 0;
	if((len >= (offset + 1)) && (posMask & BUFFER_BEG_INCLUDED) != 0)
	{
		x = (int)(*(pData + offset)) & 0xFF;
		printf("%2X ", x);
	}


	if((len >= (offset + 1)) && (posMask & BUFFER_END_INCLUDED) != 0)
	{
		x = (int)(*(pData + len - offset - 1)) & 0xFF;
		printf("%2X ", x);
		printf("\n");
	}	
}

int readFileByMap(const char* lpFileName, __int64 offset, __int64 len, FileDataProcessor operCallBack)
{
	// Open the file
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		printf("hFile is NULL\n");
		printf("Target file is %s\n", lpFileName);
		return -1;
	}
	
	SYSTEM_INFO SysInfo;          // system information; used to get the granularity
	DWORD dwSysGran;              // system allocation granularity
	// Get the system allocation granularity.
	GetSystemInfo(&SysInfo);
	dwSysGran = SysInfo.dwAllocationGranularity;
	
	LARGE_INTEGER liFileSize; // temporary storage for file sizes
	DWORD dwFileSizeLow;
	DWORD dwFileSizeHigh;
	dwFileSizeLow = GetFileSize(hFile,  &dwFileSizeHigh);

	DWORD dwError;
	// If we failed ... 
	if (dwFileSizeLow == INVALID_FILE_SIZE  && (dwError = GetLastError()) != NO_ERROR )
	{ 
		printf("hFile is NULL\n");
		printf("Failed to get the size of file %s, error code = %d\n", lpFileName, dwError);
		
		CloseHandle(hFile);
		return -2;
	}
	
	liFileSize.LowPart = dwFileSizeLow;
	liFileSize.HighPart = dwFileSizeHigh;
	if(offset >= liFileSize.QuadPart)
	{
		offset = 0;
	}
	if(len == 0 || (offset + len) > liFileSize.QuadPart)
	{
		len = liFileSize.QuadPart - offset;
	}
	
	// Now calculate a few variables. Calculate the file offsets as
	// 64-bit values.
	__int64 qwFileMapSize;          // size of the file mapping
	__int64 qwFileMapStart;         // where in the file to start the file map view
	// To calculate where to start the file mapping, round down the
	// offset of the data into the file to the nearest multiple of the
	// system allocation granularity. 
	qwFileMapStart = (offset / dwSysGran) * dwSysGran;
	
	// How large will the file-mapping object be?
	qwFileMapSize = offset + len;
	
	int iViewDelta;               // the offset into the view where the data shows up
	// The data of interest isn't at the beginning of the
	// view, so determine how far into the view to set the pointer.
	iViewDelta = (int)(offset - qwFileMapStart);
	
	
	HANDLE hMapFile;              // handle for the test file's memory-mapped region
	// Create a file-mapping object for the file.
	hMapFile = CreateFileMapping( hFile, // current file handle
		NULL, // default security
		PAGE_READWRITE, // read/write permission
		(DWORD)(qwFileMapSize >> 32), // size of mapping object, high
		(DWORD)(qwFileMapSize & 0xFFFFFFFF), // size of mapping object, low
		NULL); // name of mapping object
	
	if (hMapFile == NULL) 
	{
		printf("hMapFile is NULL: last error: %d\n", GetLastError() );
		return -3;
	}

	//Usually dwSysGran is 64k, here we map 16M ( = 2^24) data at one time ( once view )
	DWORD dwNumberOfBytesToMap = dwSysGran << 8;
	
	LPVOID lpMapAddress;          // pointer to the base address of the memory-mapped region
	__int64 qwNumberOfBytesRead = 0;
	BOOL bFlag = FALSE;                   // a result holder
	for(__int64 qwFileOffset = qwFileMapStart; qwFileOffset < qwFileMapSize; qwFileOffset += dwNumberOfBytesToMap)
	{
		if(qwFileMapSize - qwFileOffset < dwNumberOfBytesToMap)
		{
			dwNumberOfBytesToMap = (DWORD)(qwFileMapSize - qwFileOffset);
		}
		// Map the view
		lpMapAddress = MapViewOfFile(hMapFile, // handle to mapping object
			FILE_MAP_ALL_ACCESS, // read/write permission 
			(DWORD)(qwFileOffset >> 32), // high-order 32 bits of file offset
			(DWORD)(qwFileOffset & 0xFFFFFFFF), // low-order 32 bits of file offset
			dwNumberOfBytesToMap); // number of bytes to map
		if (lpMapAddress == NULL) 
		{
			printf("lpMapAddress is NULL: last error: %d, at address H:%d -- L:%d\n", GetLastError(), (DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFFF));
			
			CloseHandle(hMapFile); // close the file-mapping object
			CloseHandle(hFile); 
			return -4;
		}

		//Begins here
		if(qwFileOffset <= offset)
		{
			//This buffer includes all the expected data.
			if(len <= dwNumberOfBytesToMap - iViewDelta)
			{
				operCallBack((char*)lpMapAddress + iViewDelta, len, BUFFER_BEG_INCLUDED | BUFFER_END_INCLUDED);
				bFlag = TRUE;
				break;
			}
			//continue
			else
			{
				operCallBack((char*)lpMapAddress + iViewDelta, dwNumberOfBytesToMap - iViewDelta, BUFFER_BEG_INCLUDED);
				qwNumberOfBytesRead += dwNumberOfBytesToMap - iViewDelta;
			}
		}
		//second or later data buffer
		else
		{
			//Found end
			if(qwNumberOfBytesRead + dwNumberOfBytesToMap >= len)
			{
				operCallBack((char*)lpMapAddress, len - qwNumberOfBytesRead, BUFFER_END_INCLUDED);
				bFlag = TRUE;
				break;
			}
			//Not the end
			else
			{
				operCallBack((char*)lpMapAddress, dwNumberOfBytesToMap, 0);
				qwNumberOfBytesRead += dwNumberOfBytesToMap;
			}
		}
		
		// Close the file-mapping object and the open file.
		if (!UnmapViewOfFile(lpMapAddress)) 
		{
			printf("Could not unmap view of file. last error: %d, at address H:%d -- L:%d\n", GetLastError(), (DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFFF));
		} 
	}
	
	//Need to unmap view
	if(bFlag)
	{
		// Close the file-mapping object and the open file.
		if (!UnmapViewOfFile(lpMapAddress)) 
		{
			printf("Could not unmap view of file. last error: %d, at address H:%d -- L:%d\n", GetLastError(), (DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFFF));
		} 
	}

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

int readFileByIO(const char* lpFileName, __int64 offset, __int64 len, FileDataProcessor operCallBack)
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

	if((long)offset >= file_length)
	{
		offset = 0;
	}
	if(len == 0)
	{
		len = (size_t)file_length;
	}	
	if((long)(offset + len) > file_length)
	{
		len = (size_t)file_length;
	}

#define BUFFER_SIZE 4096
	char buffer[BUFFER_SIZE];
	
	size_t num_read = 0;  //actual bytes number by fread function returns
	size_t file_pos = 0;  //current file offset
	size_t num_processed = 0; //how many bytes processed already
	while( (num_read = fread(buffer, sizeof(char), BUFFER_SIZE, pf)) > 0 )
	{
		//Has not entered into the target area
		if( (file_pos + num_read) <= offset )
		{
			file_pos += num_read;
			continue;
		}
		//This buffer includes some of the target area
		else if(file_pos <= offset)
		{
			size_t index = offset - file_pos;

			if(file_pos + num_read - offset >= len)
			{
				operCallBack(buffer + index, len, BUFFER_BEG_INCLUDED | BUFFER_END_INCLUDED);
				break;
			}
			else
			{
				operCallBack(buffer + index, file_pos + num_read - offset, BUFFER_BEG_INCLUDED);				
				num_processed += file_pos + num_read - offset;
				file_pos += num_read;
			}
		}
		//Started
		else
		{
			if(len - num_processed <= num_read)
			{
				operCallBack(buffer, len - num_processed, BUFFER_END_INCLUDED);
				break;
			}
			else
			{
				operCallBack(buffer, num_read, 0);
				num_processed += num_read;
				file_pos += num_read;
			}
		}
	}
	
	fclose(pf);
	return 0;
}


int readEntireFile(const char* lpFileName, __int64 offset, __int64 len, FileDataProcessor operCallBack)
{
	// Open the file
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		printf("hFile is NULL\n");
		printf("Target file is %s\n", lpFileName);
		return -1;
	}
	
	SYSTEM_INFO SysInfo;          // system information; used to get the granularity
	DWORD dwSysGran;              // system allocation granularity
	// Get the system allocation granularity.
	GetSystemInfo(&SysInfo);
	dwSysGran = SysInfo.dwAllocationGranularity;
	
	DWORD dwFileSizeLow, dwFileSizeHigh;             // temporary storage for file sizes
	// Let the user know that the resulting file is more than large enough
	// for the experiment.
	dwFileSizeLow = GetFileSize(hFile,  &dwFileSizeHigh);


	DWORD dwError;
	// If we failed ... 
	if (dwFileSizeLow == INVALID_FILE_SIZE  && (dwError = GetLastError()) != NO_ERROR )
	{ 
		printf("hFile is NULL\n");
		printf("Failed to get the size of file %s, error code = %d\n", lpFileName, dwError);

		CloseHandle(hFile);
		return -2;
	}
	LARGE_INTEGER liFileSize;
	liFileSize.LowPart = dwFileSizeLow;
	liFileSize.HighPart = dwFileSizeHigh;
	
	HANDLE hMapFile;              // handle for the test file's memory-mapped region
	// Create a file-mapping object for the file.
	hMapFile = CreateFileMapping( hFile, // current file handle
		NULL, // default security
		PAGE_READWRITE, // read/write permission
		dwFileSizeHigh,  // size of mapping object, high
		dwFileSizeLow, // size of mapping object, low
		NULL); // name of mapping object
	
	if (hMapFile == NULL) 
	{
		printf("hMapFile is NULL: last error: %d\n", GetLastError() );

		CloseHandle(hFile); 
		return -3;
	}
	
	LPVOID lpMapAddress;          // pointer to the base address of the memory-mapped region
	LARGE_INTEGER liFileOffset;
	liFileOffset.QuadPart = 0;
	SIZE_T dwNumberOfBytesToMap = 0x1000000; //16M = 2^24

	int posMask = 0;
	while(liFileOffset.QuadPart < liFileSize.QuadPart)
	{
		if(liFileSize.QuadPart - liFileOffset.QuadPart < dwNumberOfBytesToMap)
		{
			dwNumberOfBytesToMap = (SIZE_T)(liFileSize.QuadPart - liFileOffset.QuadPart);
		}
		// Map the view and test the results.	
		lpMapAddress = MapViewOfFile(hMapFile, // handle to mapping object
			FILE_MAP_ALL_ACCESS, // read/write permission 
			liFileOffset.HighPart, // high-order 32 bits of file offset
			liFileOffset.LowPart, // low-order 32 bits of file offset
			dwNumberOfBytesToMap); // number of bytes to map

		if (lpMapAddress == NULL) 
		{
			printf("lpMapAddress is NULL: last error: %d, at address H:%d -- L:%d\n", GetLastError(), liFileOffset.HighPart, liFileOffset.LowPart);
			
			CloseHandle(hMapFile); // close the file-mapping object
			CloseHandle(hFile); 
			return -4;
		}
		posMask = 0;
		//Begin included
		if(liFileOffset.QuadPart == 0)
		{
			posMask |= BUFFER_BEG_INCLUDED;
		}
		//End included
		if((liFileOffset.QuadPart + dwNumberOfBytesToMap) >= liFileSize.QuadPart)
		{
			posMask |= BUFFER_END_INCLUDED;
		}
		operCallBack((char*)lpMapAddress, dwNumberOfBytesToMap, posMask);

		// Close the file-mapping object and the open file.
		if (!UnmapViewOfFile(lpMapAddress)) 
		{
			printf("Could not unmap view of file. last error: %d, at address H:%d -- L:%d\n", GetLastError(), liFileOffset.HighPart, liFileOffset.LowPart);
		} 

		liFileOffset.QuadPart += dwNumberOfBytesToMap;
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