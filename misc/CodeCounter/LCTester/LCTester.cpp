// LCTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LCTester.h"
#include "CFileParser.h"
#include "PLCFileParser.h"
#include "BaseLogger.h"
#include "GenericFileParser.h"
#include "LangGrammar.h"
#include "TimeCost.h"
// 
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

bool gIsBatchCount = false;

typedef void (*func)(LPCTSTR lpFileName);

void test(LPCTSTR lpFileName);

CLangGrammar gLangGrammar;
void InitGrammar()
{	
	CSingleLineComment singleComment("//");
	gLangGrammar.m_singleCommentArray.Add(singleComment);
	
	CMultiLineComment multiComment("/*", "*/");
	gLangGrammar.m_multiCommentArray.Add(multiComment);
	
	CPair strPair("\"", "\"");
	gLangGrammar.m_stringMarkArray.Add(strPair);
	
	// 	CPair strPair2("'", "'");
	// 	gLangGrammar.m_stringMarkArray.Add(strPair2);
	
	gLangGrammar.m_escapeStrArray.Add("\\");
}

int EnumDirectoryIt(LPCTSTR lpszDirName, func pFunc)
{
	ASSERT(lpszDirName);
	gIsBatchCount = true;
	
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
				(*pFunc)(sCurFile);
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

#define FP_FSM		0
#define FP_PLC		1
#define FP_GEN		2

IFileParser* BuildFileParser(int type)
{
	IFileParser* pFileParser = NULL;
	LPCTSTR lpLogFile = NULL;
	if(type == FP_FSM)
	{
		lpLogFile = "C:\\temp\\fsm_log.txt";
		pFileParser = new CCFileParser(NULL);
	}
	else if(type == FP_PLC)
	{
		lpLogFile = "C:\\temp\\plc_log.txt";
		pFileParser = new CPlcFileParser(NULL);
	}
	else if(type == FP_GEN)
	{
		lpLogFile = "C:\\temp\\gen_log.txt";
		pFileParser = new CGenericFileParser(NULL, &gLangGrammar);
	}

	if(pFileParser != NULL && !gIsBatchCount)
	{
		pFileParser->SetLogger(lpLogFile);
	}
	return pFileParser;
}


void test(LPCTSTR lpFileName, IFileParser* pFileParser, CFileInfo*& pFileInfo, UINT& nTimeCost)
{
	pFileInfo = new CFileInfo(lpFileName);
	pFileParser->SetFileInfo(pFileInfo);

	CTimeCost timeCost;

	pFileParser->ParseFile();

	timeCost.UpdateCurrClock();
	nTimeCost = timeCost.GetTimeCost();
}


void parseByPlc(LPCTSTR lpFileName)
{
// 	CFileInfo* pFileInfo = new CFileInfo();
// 	pFileInfo->SetFileName(lpFileName);
// 	CPlcFileParser parser(pFileInfo);
// 
// 	LPCTSTR lpLogFile = "C:\\temp\\plc_log.txt";
// 	if(!gIsBatchCount)
// 	{
// 		parser.SetLogger(lpLogFile);
// 	}
// 
// 	parser.ParseFile();
// 	return pFileInfo;

	CFileInfo* pFileInfoGen = NULL;
	UINT nTimeCostGen = 0;
	IFileParser* pFileParserGen = BuildFileParser(FP_PLC);
	test(lpFileName, pFileParserGen, pFileInfoGen, nTimeCostGen);
	
	printf("Time-PLC: %d. File=%s\n", nTimeCostGen, lpFileName);
	delete pFileInfoGen;
	delete pFileParserGen;
}

void parseByGen(LPCTSTR lpFileName)
{
	CFileInfo* pFileInfoGen = NULL;
	UINT nTimeCostGen = 0;
	IFileParser* pFileParserGen = BuildFileParser(FP_GEN);
	test(lpFileName, pFileParserGen, pFileInfoGen, nTimeCostGen);

	printf("Time-GEN: %d. File=%s\n", nTimeCostGen, lpFileName);
	delete pFileInfoGen;
	delete pFileParserGen;
}

void test(LPCTSTR lpFileName)
{
	CFileInfo* pFileInfoPlc = NULL;
	UINT nTimeCostPlc = 0;
	IFileParser* pFileParserPlc = BuildFileParser(FP_PLC);
	test(lpFileName, pFileParserPlc, pFileInfoPlc, nTimeCostPlc);

	CFileInfo* pFileInfoGen = NULL;
	UINT nTimeCostGen = 0;
	IFileParser* pFileParserGen = BuildFileParser(FP_GEN);
	test(lpFileName, pFileParserGen, pFileInfoGen, nTimeCostGen);

	if( !(*pFileInfoPlc == *pFileInfoGen) )
	{
		printf("[Failed ]: %s\n", lpFileName);
	}
	if(nTimeCostGen > nTimeCostPlc)
	{
		printf("Time-PLC: %d, Time-GEN: %d. File=%s\n", nTimeCostPlc, nTimeCostGen, lpFileName);
	}	
	delete pFileInfoPlc;
	delete pFileInfoGen;

	delete pFileParserPlc;
	delete pFileParserGen;
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
	gIsBatchCount = false;
	InitGrammar();

	LPCTSTR lpFileName = NULL;
	lpFileName = "C:\\diskf\\workspace3.4.1\\e2u\\src\\com\\e2u\\test\\test.txt";
 	lpFileName = "C:\\lgao1\\87svnwc\\cosps\\tinyxml_2_5_3\\tinyxmlparser.cpp";
	lpFileName = "C:\\temp\\abc.txt";
	lpFileName = "C:\\lgao1\\87svnwc\\cosps\\CxImage\\CxImage6.0\\cximage600_full\\CxImage\\ximatran.cpp";

	for(int i = 0; i < 30; i++)
	{
		parseByGen(lpFileName);
		parseByPlc(lpFileName);
	}
	

// 	CTimeCost timeCost;
// 	EnumDirectoryIt(_T("C:\\lgao1\\87svnwc"), test);
// 	timeCost.UpdateCurrClock();
// 	printf("Time Cost: %d\n", timeCost.GetTimeCost());

	return nRetCode;
}


