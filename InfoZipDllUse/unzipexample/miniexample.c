#pragma comment(lib, "unzip32")

#include "miniexample.h"

/* Forward References */
int WINAPI dllPrint(LPSTR, unsigned long);
int WINAPI dllServCallBack(LPCSTR, unsigned long);
int WINAPI dllReplace(LPSTR);
int WINAPI dllPassword(LPSTR, int, LPCSTR, LPCSTR);
void WINAPI dllSendApplicationMessage(unsigned long, unsigned long, unsigned,
									  unsigned, unsigned, unsigned, unsigned, unsigned,
									  char, LPSTR, LPSTR, unsigned long, char);

int main1(int argc, char* argv[])
{
	LPUSERFUNCTIONS lpUserFunctions;	
	LPDCL lpDCL = NULL;
	
	
	int exfc = 0, infc = 0;
	char **exfv = NULL, **infv = NULL;
	int retcode = 0;
	
	if (argc < 2)   /* We must have an archive to unzip */
	{
		printf("usage: %s <zipfile>\n", argv[0]);
		return -1;
	}
	
	lpDCL = (LPDCL)malloc(sizeof(DCL));	
	lpUserFunctions = (LPUSERFUNCTIONS)malloc(sizeof(USERFUNCTIONS));
	
	lpUserFunctions->password = dllPassword;
	lpUserFunctions->print = dllPrint;
	lpUserFunctions->sound = NULL;
	lpUserFunctions->replace = dllReplace;
	lpUserFunctions->SendApplicationMessage = dllSendApplicationMessage;
	lpUserFunctions->ServCallBk = dllServCallBack;
	
	lpDCL->ncflag = 0; /* Write to stdout if true */
	lpDCL->fQuiet = 0; /* We want all messages. 1 = fewer messages, 2 = no messages */
	lpDCL->ntflag = 0; /* test zip file if true */
	lpDCL->nvflag = 1; /* give a verbose listing if true */
	lpDCL->nzflag = 0; /* display a zip file comment if true */
	lpDCL->ndflag = 1; /* Recreate directories != 0, skip "../" if < 2 */
	lpDCL->naflag = 0; /* Do not convert CR to CRLF */
	lpDCL->nfflag = 0; /* Do not freshen existing files only */
	lpDCL->noflag = 1; /* Over-write all files if true */
	lpDCL->ExtractOnlyNewer = 0; /* Do not extract only newer */
	lpDCL->PromptToOverwrite = 0; /* "Overwrite all" selected -> no query mode */
	lpDCL->nZIflag = FALSE; /*get ZipInfo if TRUE*/
	lpDCL->lpszZipFN = argv[1]; /* The archive name */
	lpDCL->lpszExtractDir = NULL; /* The directory to extract to. This is set to NULL if you are extracting to the current directory. */
	
	retcode = Wiz_SingleEntryUnzip(infc, infv, exfc, exfv, lpDCL, lpUserFunctions);
	if (retcode != 0)
		printf("Error unzipping...\n");
	
	free(lpDCL);
	free(lpUserFunctions);
	
	return 0;
}

/* 
This is where you will decide if you want to replace, rename etc existing
files.
*/
int WINAPI dllReplace(LPSTR filename)
{
	printf("dllReplace (%s)\n", filename);
	return 1;
}

/* This is a very stripped down version of what is done in Wiz. Essentially
what this function is for is to do a listing of an archive contents. It
is actually never called in this example, but a dummy procedure had to
be put in, so this was used.
*/
void WINAPI dllSendApplicationMessage(unsigned long ucsize, unsigned long csiz,
									  unsigned cfactor,
									  unsigned month, unsigned day, unsigned year, unsigned hour, unsigned minute,
									  char c, LPSTR filename, LPSTR methbuf, unsigned long crc, char fCrypt)
{
	char psLBEntry[_MAX_PATH];
	char LongHdrStats[] =
		"%7lu  %7lu %4s  %02u-%02u-%02u  %02u:%02u  %c%s";
	char CompFactorStr[] = "%c%d%%";
	char CompFactor100[] = "100%%";
	char szCompFactor[10];
	char sgn;
	
	if (csiz > ucsize)
		sgn = '-';
	else
		sgn = ' ';
	if (cfactor == 100)
		lstrcpy(szCompFactor, CompFactor100);
	else
		sprintf(szCompFactor, CompFactorStr, sgn, cfactor);
	
	wsprintf(psLBEntry, LongHdrStats, ucsize, csiz, szCompFactor, month, day, year, hour, minute, c, filename);
	printf("%s\n", psLBEntry);
}

/* Password entry routine - see password.c in the wiz directory for how
this is actually implemented in WiZ. If you have an encrypted file,
this will probably give you great pain.
*/
int WINAPI dllPassword(LPSTR p, int n, LPCSTR m, LPCSTR name)
{
	printf("dllPassword called\n");
	return 1;
}

/* Dummy "print" routine that simply outputs what is sent from the dll */
int WINAPI dllPrint(LPSTR buf, unsigned long size)
{
	printf("%s", (char *)buf);
	return (int)(unsigned int) size;
}

int WINAPI dllServCallBack(LPCSTR buf, unsigned long size)
{
	printf("dllServCallBack[%s]-[%d]\n", (char *)buf, size);
	return 0;
}

