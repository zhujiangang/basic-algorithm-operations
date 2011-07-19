// LCTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LCTester.h"
#include "FileParser.h"
#include "LangGrammar.h"
#include "TimeCost.h"
#include <log4cplus/configurator.h>
#include "gtb.h"
#include "./impl/TinyVector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;
using namespace log4cplus;

DECLARE_THE_LOGGER_NAME("LCTester.cpp")

CString szBaseDir;
bool gIsBatchCount = false;

typedef void (*func)(LPCTSTR lpFileName);

void test(LPCTSTR lpFileName);

void InitBaseDir(LPCTSTR lpBaseDir)
{
	gtb::GetAbsolutePath(lpBaseDir, szBaseDir);
	LOG4CPLUS_INFO(THE_LOGGER, "Base Dir = "<<(LPCTSTR)szBaseDir)
}

void InitLog4cplus()
{
#ifdef ENABLE_LOG4CPLUS

	const char* pEnvName = "LOG_FILE_SUFFIX";
	CString szOldValue = getenv(pEnvName);

	//Set the env to the current time str
	CString sTime = CTime::GetCurrentTime().Format("_%Y_%m_%d_%H_%M_%S");

	CString szValue;
	szValue.Format("%s=%s", pEnvName, sTime);
	_putenv(szValue);

	//This will take effect in the configure process
	PropertyConfigurator::doConfigure("log4cplus.properties");
	
	//After configuration, restore the envrionment varialble.
	szValue.Format("%s=%s", pEnvName, szOldValue);
	_putenv(szValue);

// 	SharedAppenderPtr fileAppender(new RollingFileAppender(".\\log\\test.log", 20*1024*1024, 500));
// 	fileAppender->setName("file");
// 	
// 	std::auto_ptr<Layout> pPatternLayout(new PatternLayout("[%d{%Y-%m-%d %H:%M:%S,%q}][%p][%t][%c][%l] - %m%n"));
// 	fileAppender->setLayout(pPatternLayout);
// 	
// 	ROOT_LOGGER.addAppender(fileAppender);
// 	
// 	ROOT_LOGGER.setLogLevel(DEBUG_LOG_LEVEL);
#endif
}

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
		pFileParser = CFileParserFactory::GetFileParser(LANG_TYPE_FSM, NULL);
	}
	else if(type == FP_PLC)
	{
		pFileParser = CFileParserFactory::GetFileParser(LANG_TYPE_PLC, NULL);
	}
	else if(type == FP_GEN)
	{
		pFileParser = CFileParserFactory::GetGenericFileParser(pGLangGrammar, NULL);
	}
	else if(type == FP_CPP)
	{
		pFileParser = CFileParserFactory::GetFileParser(LANG_TYPE_CPP, NULL);
	}

#ifdef ENABLE_LOG4CPLUS
	LogLevel ll = (gIsBatchCount ? INFO_LOG_LEVEL : DEBUG_LOG_LEVEL);
	GET_LOGGER("FileParser.cpp").setLogLevel(ll);
	GET_LOGGER("CFileParser.cpp").setLogLevel(ll);
#endif

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
		LOG4CPLUS_ERROR(THE_LOGGER, "[Failed]: "<<lpFileName)
	}	
	delete pFileInfoPlc;
	delete pFileInfoGen;

	delete pFileParserPlc;
	delete pFileParserGen;
}

void testWorkable()
{
	CString szFullPath = szBaseDir;

	LPCTSTR lpDir = "cosps";
	if(lstrlen(lpDir) > 0)
	{
		szFullPath += "\\";
		szFullPath += lpDir;
	}
	
	EnumDirectoryIt(szFullPath, test);
}

void testSingleFile()
{
	LPCTSTR lpFileName = NULL;
 	lpFileName = "cosps\\Misc\\PLC\\PLC221Src\\Help\\IDH_ABOUT.htm";

	CString szFullPath;
	szFullPath.Format("%s\\%s", szBaseDir, lpFileName);
	test(szFullPath);
}

void testBatchFiles()
{
	LPCTSTR lpDir = szBaseDir;
	
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

struct xyz
{
	CString sStr;
	int nType;
};
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
	InitLog4cplus();

	LPCTSTR lpBaseDir = _T("..\\..\\..\\..");
	if(argc > 1)
	{
		lpBaseDir = argv[1];
	}
	InitBaseDir(lpBaseDir);

	int testType = 2;
	if(argc > 2)
	{
		testType = atoi(argv[2]);
	}

	gIsBatchCount = false;
	InitGrammar();

	LOG4CPLUS_INFO(THE_LOGGER, "TestType="<<testType)
	for(int i = 1; i <= 100; i++)
	{
		LOG4CPLUS_INFO(THE_LOGGER, "("<<i<<")")
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
			testBatchFiles();
		}
	}

	if(pGLangGrammar != NULL)
	{
		delete pGLangGrammar;
	}

	return nRetCode;
}


