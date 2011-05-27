// LCTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LCTester.h"
#include "CFileParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

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

	CFileInfo* pFileInfo = new CFileInfo();
//	pFileInfo->SetFileName("C:\\diskf\\workspace3.4.1\\e2u\\src\\com\\e2u\\test\\test.txt");
	pFileInfo->SetFileName("C:\\diskf\\vc6works\\msdn\\Samples\\VC98\\sdk\\graphics\\audio\\acmapp\\acmapp.h");

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

	return nRetCode;
}


