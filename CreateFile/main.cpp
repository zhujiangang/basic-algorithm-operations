#include <windows.h>
#include <stdio.h>

#define INVALID_SET_FILE_POINTER  0xFFFFFFFF

__int64 LongFileSeek (HANDLE hf, __int64 distance, DWORD MoveMethod)
{
	LARGE_INTEGER li;
	
	li.QuadPart = distance;
	
	li.LowPart = SetFilePointer (hf, li.LowPart, &li.HighPart, MoveMethod);
	
	if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
	{
		li.QuadPart = -1;
	}
	
	return li.QuadPart;
}

int CreateLongFile(LPTSTR lpFileName, __int64 len)
{
	HANDLE hFile;                 // the file handle
	
	// Create the test file. Open it "Create Always" to overwrite any
	// existing file. The data is re-created below.
	hFile = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("hFile is NULL\n");
		printf("Target file is %s\n", lpFileName);
		return 4;
	}

	__int64 result = LongFileSeek(hFile, len - 1, FILE_BEGIN);

	DWORD dBytesWritten;          // number of bytes written
	char c = 0;
	WriteFile (hFile, &c, sizeof(c), &dBytesWritten, NULL);

	
	BOOL bFlag = CloseHandle(hFile);   // close the file itself
	
	if(!bFlag) {
		printf("\nOops! Error # %ld occurred closing the file!",
			GetLastError());
	}
	
	return 0;
	
}

void printUsage()
{
	printf("Usage: cbf <file name> <size in GB>\n\n");
	printf("Example: to create a 10 GB file with name \"test.dat\"\n");
	printf("       cbf test.dat 10\n\n");
}

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printUsage();
		return -1;
	}


	LPTSTR lpFileName = argv[1];
	int size = atoi(argv[2]);

	int low = size % 4;
	int high = size / 4;

	LARGE_INTEGER iFileLen;
	if(low != 0)
	{
		iFileLen.LowPart = 0x40000000 * low;
	}
	else
	{
		iFileLen.LowPart = 0;
	}
	iFileLen.HighPart = high;
	

	CreateLongFile(lpFileName, iFileLen.QuadPart);
	return 0;
}