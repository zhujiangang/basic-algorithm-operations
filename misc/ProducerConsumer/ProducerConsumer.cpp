// ProducerConsumer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ProducerConsumer.h"
#include "SynQueue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object
//http://blog.csdn.net/hangyu628/article/details/3849255
//http://www.4ucode.com/Study/Topic/1547138
//http://wenku.baidu.com/view/bde963c34028915f804dc2d0.html

CWinApp theApp;

using namespace std;

long nProductID = 0;

DWORD WINAPI ThreadProcP(LPVOID lpParameter)
{
	CSynQueue* pQueue = (CSynQueue*)lpParameter;

	int nID;
	for(int i = 0; i < 10; i++)
	{
		Sleep(100);
		nID = ::InterlockedIncrement(&nProductID);
		printf("Producer [%05d] produce [%02d]\n", GetCurrentThreadId(), nID);
		pQueue->Put((void*)nID);
	}

	return 0;
}

bool consume(CSynQueue* pQueue, void* ptr)
{
	printf("Consumer [%05d] consume [%02d]\n", GetCurrentThreadId(), (int)ptr);
	if(!ptr)
	{
		pQueue->Put(NULL);
		return false;
	}
	int id = (int)ptr;
	if(id == 50)
	{
		pQueue->Put(NULL);
		return false;
	}
	return true;
}
DWORD WINAPI ThreadProcC(LPVOID lpParameter)
{
	CSynQueue* pQueue = (CSynQueue*)lpParameter;

	void* ptr = NULL;

	while(TRUE)
	{
		Sleep(100);
		ptr = pQueue->Get();

		if(!consume(pQueue, ptr))
		{
			printf("Consumer [%05d] ended\n", GetCurrentThreadId());
			break;
		}
	}

	return 0;
}

void TestProducerConsumer(int nCountP, int nCountC, int nMax);

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

	TestProducerConsumer(5, 8, 9);

	return nRetCode;
}

void TestProducerConsumer(int nCountP, int nCountC, int nMax)
{
	CSynQueue synQueue;
	synQueue.SetMaxCount(nMax);

	HANDLE* hThreads = new HANDLE[nCountP + nCountC];

	int i, j;
	DWORD dwThreadId;
	for(i = 0; i < nCountC; i++)
	{
		hThreads[i] = ::CreateThread(NULL, 0, ThreadProcC, (LPVOID)&synQueue, 0, &dwThreadId);
	}
	for(j = 0; j < nCountP; j++)
	{
		hThreads[nCountC + j] = ::CreateThread(NULL, 0, ThreadProcP, (LPVOID)&synQueue, 0, &dwThreadId);
	}

	::WaitForMultipleObjects(nCountC + nCountP, hThreads, TRUE, INFINITE);
	printf("All Threads ended\n");
}


