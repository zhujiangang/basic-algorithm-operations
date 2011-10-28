#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include "internet.h"

#pragma comment(lib, "wininet.lib")

//Example code from: http://support.microsoft.com/kb/226473
void show_internet_options()
{
	//IE4
	unsigned long        nSize = 4096;
	char                 szBuf[4096] = { 0 };
	INTERNET_PROXY_INFO* pInfo = (INTERNET_PROXY_INFO*)szBuf;
	
	if(!InternetQueryOption(NULL, INTERNET_OPTION_PROXY, pInfo, &nSize))
		printf("InternetQueryOption failed! (%d)\n", GetLastError());

	//IE5
	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[5];
	/*unsigned long                    */nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	
	Option[0].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
	Option[1].dwOption = INTERNET_PER_CONN_AUTODISCOVERY_FLAGS;
	Option[2].dwOption = INTERNET_PER_CONN_FLAGS;
	Option[3].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	Option[4].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
	
	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = NULL;
	List.dwOptionCount = 5;
	List.dwOptionError = 0;
	List.pOptions = Option;
	
	if(!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize))
		printf("InternetQueryOption failed! (%d)\n", GetLastError());
	if(Option[0].Value.pszValue != NULL)
		printf("%s\n", Option[0].Value.pszValue);
	
	if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_PROXY_URL) == PROXY_TYPE_AUTO_PROXY_URL)
		printf("PROXY_TYPE_AUTO_PROXY_URL\n");
	
	if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_DETECT) == PROXY_TYPE_AUTO_DETECT)
		printf("PROXY_TYPE_AUTO_DETECT\n");
	
	INTERNET_VERSION_INFO      Version;
	nSize = sizeof(INTERNET_VERSION_INFO);
	
	InternetQueryOption(NULL, INTERNET_OPTION_VERSION, &Version, &nSize);
	
	if(Option[0].Value.pszValue != NULL)
		GlobalFree(Option[0].Value.pszValue);
	
	if(Option[3].Value.pszValue != NULL)
		GlobalFree(Option[3].Value.pszValue);
	
	if(Option[4].Value.pszValue != NULL)
		GlobalFree(Option[4].Value.pszValue);
}