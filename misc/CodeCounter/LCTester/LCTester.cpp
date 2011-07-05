// LCTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LCTester.h"
#include "FileParser.h"
#include "LangGrammar.h"
#include "TimeCost.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

bool gIsBatchCount = false;

typedef void (*func)(LPCTSTR lpFileName);

void test(LPCTSTR lpFileName);

ILangGrammar* pGLangGrammar = NULL;
void InitGrammar()
{	
	CLangGrammarBuilder builder;
	builder.AddSingleComment("//");
	builder.AddMultiComment("/*", "*/");
	builder.AddStringMark("\"", "\"");
	builder.AddCharMark("'", "'");
	builder.AddEscapeStr("\\");

	pGLangGrammar = builder.GetResult();
}

int EnumDirectoryIt(LPCTSTR lpszDirName, func pFunc)
{
	ASSERT(lpszDirName);
	gIsBatchCount = true;
	int nCount = 0;
	
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
				nCount++;
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
	
	return nCount;
}

#define FP_FSM		0
#define FP_PLC		1
#define FP_GEN		2
#define FP_CPP		3

IFileParser* BuildFileParser(int type)
{
	IFileParser* pFileParser = NULL;
	LPCTSTR lpLogFile = NULL;
	if(type == FP_FSM)
	{
		lpLogFile = "C:\\temp\\fsm_log.txt";
//		pFileParser = new CCFileParser(NULL);
		pFileParser = CFileParserFactory::GetFileParser(LANG_TYPE_FSM, NULL);
	}
	else if(type == FP_PLC)
	{
		lpLogFile = "C:\\temp\\plc_log.txt";
//		pFileParser = new CPlcFileParser(NULL);
		pFileParser = CFileParserFactory::GetFileParser(LANG_TYPE_PLC, NULL);
	}
	else if(type == FP_GEN)
	{
		lpLogFile = "C:\\temp\\gen_log.txt";
//		pFileParser = new CGenericFileParser(pGLangGrammar, NULL);
		pFileParser = CFileParserFactory::GetGenericFileParser(pGLangGrammar, NULL);
	}
	else if(type == FP_CPP)
	{
		lpLogFile = "C:\\temp\\cpp_log.txt";		
//		pFileParser = new CCPPFileParser(NULL);
		pFileParser = CFileParserFactory::GetFileParser(LANG_TYPE_CPP, NULL);
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

void parseSingle(LPCTSTR lpFileName, int type)
{
	CFileInfo* pFileInfoGen = NULL;
	UINT nTimeCostGen = 0;
	IFileParser* pFileParserGen = BuildFileParser(type);
	test(lpFileName, pFileParserGen, pFileInfoGen, nTimeCostGen);
	
	printf("type=%d, Time: %d. File=%s\n", type, nTimeCostGen, lpFileName);
	delete pFileInfoGen;
	delete pFileParserGen;
}

#define CREATE_FILE_PARSER(CLASSNAME) CFileInfo fileInfo(lpFileName); \
	CLASSNAME fileParser(&fileInfo); \
	fileParser.ParseFile()

void parseByPlcBatch(LPCTSTR lpFileName)
{
	CFileInfo fileInfo(lpFileName);
	IFileParser* pFileParser = CFileParserFactory::GetFileParser(LANG_TYPE_PLC, &fileInfo);
	pFileParser->ParseFile();
	delete pFileParser;
}
void parseByGenBatch(LPCTSTR lpFileName)
{
	CFileInfo fileInfo(lpFileName);
	IFileParser* pFileParser = CFileParserFactory::GetGenericFileParser(pGLangGrammar, &fileInfo);
	pFileParser->ParseFile();
	delete pFileParser;
}

void parseByCppBatch(LPCTSTR lpFileName)
{
//	CREATE_FILE_PARSER(CCPPFileParser);
	CFileInfo fileInfo(lpFileName);
	IFileParser* pFileParser = CFileParserFactory::GetFileParser(LANG_TYPE_CPP, &fileInfo);
	pFileParser->ParseFile();
	delete pFileParser;
}

void test(LPCTSTR lpFileName)
{
	CFileInfo* pFileInfoPlc = NULL;
	UINT nTimeCostPlc = 0;
	IFileParser* pFileParserPlc = BuildFileParser(FP_CPP);
	test(lpFileName, pFileParserPlc, pFileInfoPlc, nTimeCostPlc);

	CFileInfo* pFileInfoGen = NULL;
	UINT nTimeCostGen = 0;
	IFileParser* pFileParserGen = BuildFileParser(FP_PLC);
	test(lpFileName, pFileParserGen, pFileInfoGen, nTimeCostGen);

	if( !(*pFileInfoPlc == *pFileInfoGen) )
	{
		printf("[Failed ]: %s\n", lpFileName);
	}	
	delete pFileInfoPlc;
	delete pFileInfoGen;

	delete pFileParserPlc;
	delete pFileParserGen;
}

void testWorkable()
{
	LPCTSTR lpDir = "C:\\lgao1\\87svnwc\\cosps";
	EnumDirectoryIt(lpDir, test);
}

void testSingleFile()
{
	LPCTSTR lpFileName = NULL;
	lpFileName = "C:\\diskf\\workspace3.4.1\\e2u\\src\\com\\e2u\\test\\test.txt";
// 	lpFileName = "F:\\googlecode\\cosps\\CxImage\\Temp\\cximage600_full\\mng\\libmng_pixels.c";
// 	lpFileName = "F:\\googlecode\\cosps\\CxImage\\Temp\\cximage600_full\\mng\\libmng_chunk_io.c";
// 	lpFileName = "C:\\lgao1\\87svnwc\\cosps\\CxImage\\CxImage6.0\\cximage600_full\\jbig\\jbig.c";
 	lpFileName = "C:\\lgao1\\87svnwc\\cosps\\CxImage\\CxImage5.99\\png\\CHANGES";

	test(lpFileName);
}

void testBatchFiles()
{
	LPCTSTR lpDir = "C:\\lgao1\\87svnwc";
	
	CTimeCost timeCost;
	int nCount = 0;
	nCount = EnumDirectoryIt(lpDir, parseByCppBatch);
	timeCost.UpdateCurrClock();
	printf("Cpp Time Cost: %d, nCount = %d\n", timeCost.GetTimeCost(), nCount);
	
	timeCost.Reset();
	nCount = EnumDirectoryIt(lpDir, parseByPlcBatch);
	timeCost.UpdateCurrClock();
	printf("Plc Time Cost: %d, nCount = %d\n", timeCost.GetTimeCost(), nCount);
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

	int testType = 3;

	if(testType == 1)
	{
		testWorkable();
	}
	else if(testType == 2)
	{
		testSingleFile();
	}
 	else if(testType == 3)
	{
		for(int i = 1; i <= 100; i++)
		{
			printf("(%d)\n", i);
			testBatchFiles();
		}	
	}

	if(pGLangGrammar != NULL)
	{
		delete pGLangGrammar;
	}

	return nRetCode;
}


