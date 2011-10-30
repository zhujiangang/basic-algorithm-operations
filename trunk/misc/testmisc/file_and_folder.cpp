#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <stdio.h>
#include "file_and_folder.h"

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

void show_shell_folders()
{
	TCHAR szPath[MAX_PATH];
	HRESULT hResult = 0;
	
	char buffer[1024];
	for(int i = CSIDL_DESKTOP; i <= CSIDL_COMPUTERSNEARME; i++)
	{
		hResult = SHGetFolderPath(NULL, i, NULL, SHGFP_TYPE_CURRENT, szPath);
		
		if(SUCCEEDED(hResult))
		{
			sprintf(buffer, "%04x: OK - %s", i, szPath);
		}
		else
		{
			sprintf(buffer, "%04x: NOK", i);
		}
		printf("%s\n", buffer);
	}
}

void test_create_folders()
{
	TCHAR szPath[MAX_PATH];

	HRESULT hResult = 0;
	hResult = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szPath);

	TCHAR buffer[1024];	
	sprintf(buffer, "%s\\test1", szPath);

	BOOL bResult = ::PathFileExists(buffer);
	printf("The folder %s exists: %s\n", buffer, (bResult) ? "Y" : "N");

	if(!bResult)
	{
		bResult = ::CreateDirectory(buffer, NULL);
		if(bResult)
		{
			printf("Create Folder Successfully: %s\n", buffer);
		}
		else
		{
			printf("Create Folder Failed: last error = %d, %s\n", GetLastError(), buffer);
		}
	}
}