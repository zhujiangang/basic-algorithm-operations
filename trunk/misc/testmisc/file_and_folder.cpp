#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include "file_and_folder.h"

#pragma comment(lib, "shell32.lib")

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