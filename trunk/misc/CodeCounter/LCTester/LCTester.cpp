// LCTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LCTester.h"
#include "CFileParser.h"
#include "PLCFileParser.h"
#include "BaseLogger.h"
#include "GenericFileParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;
void test(LPCTSTR lpFileName);
int EnumDirectoryIt(LPCTSTR lpszDirName)
{
	ASSERT(lpszDirName);
	
	CStringList sDirList;
	sDirList.AddTail(lpszDirName);
	
	CString sCurDir, sCurFile, sCurFindFileFilter;
	HANDLE hFind = NULL;
	WIN32_FIND_DATA FindFileData;
	BOOL hasMore;
	DWORD dwError = 0;
	
	while(!sDirList.IsEmpty())
	{
		sCurDir = sDirList.RemoveHead();
		sCurFindFileFilter.Format("%s\\*", sCurDir);
		
		hFind = FindFirstFile(sCurFindFileFilter, &FindFileData);
		for(hasMore = (hFind != INVALID_HANDLE_VALUE); hasMore; hasMore = FindNextFile(hFind, &FindFileData))
		{
			//Ignore hidden files, current, and parent directory
			if( ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0)
				|| _tcscmp(FindFileData.cFileName, ".") == 0
				|| _tcscmp(FindFileData.cFileName, "..") == 0)
			{
				continue;
			}
			//1. File
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{				
				sCurFile.Format("%s\\%s", sCurDir, FindFileData.cFileName);
				test(sCurFile);
			}
			//2. Dir
			else if( ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) )
			{
				sCurFile.Format("%s\\%s", sCurDir, FindFileData.cFileName);
				sDirList.AddTail(sCurFile);
			}
		}
		dwError = GetLastError();
		if(hFind != INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);
		}
		if (dwError != ERROR_NO_MORE_FILES)
		{
			AfxTrace ("Find File Error: %d, %s, %s, %s\n", dwError, sCurFile, sCurFindFileFilter, sCurDir);
			return dwError;
		}
	}
	
	return 0;
}

void test1(LPCTSTR lpFileName)
{
	CFileInfo* pFileInfo = new CFileInfo();
	pFileInfo->SetFileName(lpFileName);
	
	CCFileParser parser(pFileInfo);
#ifdef _DEBUG
	parser.SetLogFile("C:\\temp\\mylog.txt");
#endif
	parser.ParseFile();
	
	printf("\n====================================\n");
	printf("Total Lines:\t %d\n", pFileInfo->m_nTotalLines);
	printf("Code Lines:\t %d\n", pFileInfo->m_nCodeLines);
	printf("Comment Lines:\t %d\n", pFileInfo->m_nCommentLines);
	printf("Blank Lines:\t %d\n", pFileInfo->m_nBlankLines);
	printf("Mixed Lines:\t %d\n", pFileInfo->GetMixedLines());
	
	delete pFileInfo;
}

void test2(LPCTSTR lpFileName)
{
	CFileInfo* pFileInfo = new CFileInfo();
	pFileInfo->SetFileName(lpFileName);
	
	CPlcFileParser parser(pFileInfo);
	parser.ParseFile();
	
	printf("\n====================================\n");
	printf("Total Lines:\t %d\n", pFileInfo->m_nTotalLines);
	printf("Code Lines:\t %d\n", pFileInfo->m_nCodeLines);
	printf("Comment Lines:\t %d\n", pFileInfo->m_nCommentLines);
	printf("Blank Lines:\t %d\n", pFileInfo->m_nBlankLines);
	printf("Mixed Lines:\t %d\n", pFileInfo->GetMixedLines());
	
	delete pFileInfo;
}

CFileInfo* parseByFsm(LPCTSTR lpFileName)
{
	CFileInfo* pFileInfo = new CFileInfo();
	pFileInfo->SetFileName(lpFileName);
	CCFileParser parser(pFileInfo);
	parser.SetLogFile("C:\\temp\\fsm_log.txt");
	parser.ParseFile();
	return pFileInfo;
}

CFileInfo* parseByPlc(LPCTSTR lpFileName)
{
	CFileInfo* pFileInfo = new CFileInfo();
	pFileInfo->SetFileName(lpFileName);
	CPlcFileParser parser(pFileInfo);

	LPCTSTR lpLogFile = "C:\\temp\\plc_log.txt";
//	parser.SetLogger(lpLogFile);

	parser.ParseFile();
	return pFileInfo;
}

CFileInfo* parseByGen(LPCTSTR lpFileName)
{
	CFileInfo* pFileInfo = new CFileInfo();
	pFileInfo->SetFileName(lpFileName);

	CLangGrammar langGrammar;

	CSingleLineComment singleComment("//");
	langGrammar.m_singleCommentArray.Add(singleComment);

	CMultiLineComment multiComment("/*", "*/");
	langGrammar.m_multiCommentArray.Add(multiComment);

	CPair strPair("\"", "\"");
	langGrammar.m_strMarkArray.Add(strPair);

	CPair strPair2("'", "'");
	langGrammar.m_strMarkArray.Add(strPair2);

	langGrammar.m_escapeStrArray.Add("\\");

	CGenericFileParser parser(pFileInfo, &langGrammar);
	LPCTSTR lpLogFile = "C:\\temp\\gen_log.txt";
//	parser.SetLogger(lpLogFile);
	
	parser.ParseFile();
	return pFileInfo;
}

void test(LPCTSTR lpFileName)
{
	CFileInfo* pFileInfo1 = parseByPlc(lpFileName);
	CFileInfo* pFileInfo2 = parseByGen(lpFileName);

	if( !(*pFileInfo1 == *pFileInfo2) )
	{
		printf("[Failed ]: %s\n", lpFileName);
	}
// 	else
// 	{
// 		printf("[Success]: %s\n", lpFileName);
// 	}
	delete pFileInfo1;
	delete pFileInfo2;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		CString strHello;
		strHello.LoadString(IDS_HELLO);
		cout << (LPCTSTR)strHello << endl;
	}

	LPCTSTR lpFileName = _T("C:\\diskf\\workspace3.4.1\\e2u\\src\\com\\e2u\\test\\test.txt");
// 	lpFileName = "C:\\diskf\\vc6works\\msdn\\Samples\\VC98\\sdk\\graphics\\audio\\acmapp\\acmapp.h";
 	lpFileName = "C:\\lgao1\\87svnwc\\cosps\\CxImage\\CxImage5.99\\jbig\\jbig.c";
 	lpFileName = "C:\\lgao1\\87svnwc\\cosps\\tinyxml_2_5_3\\tinystr.cpp";
	test(lpFileName);

	EnumDirectoryIt(_T("C:\\lgao1\\87svnwc\\cosps"));

	return nRetCode;
}


