/* filemap.c,

  This program demonstrates file mapping, especially how to align a view
  with the system file allocation granularity.
*/

#include <windows.h>
#include <stdio.h>

void showHelp()
{
	printf("Usage: ReadLast <file name> <last bytes number>\n");
}

int readFileLastBytes(const char* filename, int byteNum)
{
	// Open the file
	HANDLE hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("hFile is NULL\n");
		printf("Target file is %s\n", filename);
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
	printf("hFile size: %10d\n", dwFileSize);
	
	if((DWORD)byteNum > dwFileSize)
	{
		byteNum = dwFileSize;
	}
	
	int fileMapStart = dwFileSize - byteNum;
	
	// Now calculate a few variables. Calculate the file offsets as
	// 64-bit values, and then get the low-order 32 bits for the
	// function calls.
	int i;                        // loop counter
	BOOL bFlag;                   // a result holder
	DWORD dwFileMapSize;          // size of the file mapping
	DWORD dwMapViewSize;          // the size of the view
	DWORD dwFileMapStart;         // where in the file to start the file map view
	// To calculate where to start the file mapping, round down the
	// offset of the data into the file to the nearest multiple of the
	// system allocation granularity. 
	dwFileMapStart = (fileMapStart / dwSysGran) * dwSysGran;
	printf ("The file map view starts at %ld bytes into the file.\n",
		dwFileMapStart);
	
	// Calculate the size of the file mapping view.
	dwMapViewSize = (fileMapStart % dwSysGran) + byteNum;
	printf ("The file map view is %ld bytes large.\n", dwMapViewSize);
	
	// How large will the file-mapping object be?
	dwFileMapSize = fileMapStart + byteNum;
	printf ("The file-mapping object is %ld bytes large.\n", dwFileMapSize);
	
	int iViewDelta;               // the offset into the view where the data shows up
	// The data of interest isn't at the beginning of the
	// view, so determine how far into the view to set the pointer.
	iViewDelta = fileMapStart - dwFileMapStart;
	printf ("The data is %d bytes into the view.\n", iViewDelta);
	
	
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
		return (2);
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
		return 3;
	}
	
	char * pData;                 // pointer to the data
	// Calculate the pointer to the data.
	pData = (char *) lpMapAddress + iViewDelta;
	
	//    dump((char *)pData, BUFFSIZE);
	for(i = 0; i < byteNum; i++)
	{
		if(i % 16 == 0)
		{
			printf("\n");
		}
		printf("%2X ", ((int)(*(pData + i))) & 0xFF);
	}
	printf("\n");
	
	
	// Close the file-mapping object and the open file.
	if (!UnmapViewOfFile(lpMapAddress)) 
	{ 
		printf("Could not unmap view of file. last error: %d\n", GetLastError());
	} 
	
	bFlag = CloseHandle(hMapFile); // close the file-mapping object
	
	if(!bFlag) {
		printf("\nOops! Error # %ld occurred closing the mapping object!",
			GetLastError());
	}
	
	bFlag = CloseHandle(hFile);   // close the file itself
	
	if(!bFlag) {
		printf("\nOops! Error # %ld occurred closing the file!",
			GetLastError());
	}
	
	return 0;
}

int read(LPTSTR lpFileName, DWORD offset, DWORD len)
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
	printf("hFile size: %10d\n", dwFileSize);
	
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
	printf ("The file map view starts at %ld bytes into the file.\n",
		dwFileMapStart);
	
	// Calculate the size of the file mapping view.
	dwMapViewSize = (offset % dwSysGran) + len;
	printf ("The file map view is %ld bytes large.\n", dwMapViewSize);
	
	// How large will the file-mapping object be?
	dwFileMapSize = offset + len;
	printf ("The file-mapping object is %ld bytes large.\n", dwFileMapSize);
	
	int iViewDelta;               // the offset into the view where the data shows up
	// The data of interest isn't at the beginning of the
	// view, so determine how far into the view to set the pointer.
	iViewDelta = offset - dwFileMapStart;
	printf ("The data is %d bytes into the view.\n", iViewDelta);
	
	
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
	DWORD i;
	int x;
	//    dump((char *)pData, BUFFSIZE);
// 	for(i = 0; i < len; i++)
// 	{
// 		if(i % 16 == 0)
// 		{
// 			printf("\n");
// 		}
// 		x = (int)(*(pData + i)) & 0xFF;
// 		printf("%2X ", x);
// 	}
// 	printf("\n");
	
	x = (int)(*(pData + 0)) & 0xFF;
	printf("%2X ", x);

	x = (int)(*(pData + len - 2)) & 0xFF;
	printf("%2X ", x);

	printf("\n");
	
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

int main(int argc, char* argv[])
{
	//	test();
	
	if(argc < 3)
	{
		showHelp();
		return -1;
	}
	
	char* filename = argv[1];
	int bytesNum = atoi(argv[2]);
	
	
//	readFileLastBytes(filename, bytesNum);

	int offset = 0;
	int len = 0;
	read(filename, offset, len);

	return 0;
}
