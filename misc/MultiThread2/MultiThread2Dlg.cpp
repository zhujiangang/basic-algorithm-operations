// MultiThread2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiThread2.h"
#include "MultiThread2Dlg.h"
#include "ThreadMonitor.h"
#include "TestAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiThread2Dlg dialog

CMultiThread2Dlg::CMultiThread2Dlg(CWnd* pParent /*=NULL*/)
: CDialog(CMultiThread2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiThread2Dlg)
	m_nCount = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiThread2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiThread2Dlg)
	DDX_Text(pDX, IDC_COUNT, m_nCount);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMultiThread2Dlg, CDialog)
//{{AFX_MSG_MAP(CMultiThread2Dlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_START, OnStart)
ON_BN_CLICKED(IDC_TEST2, OnTest2)
ON_BN_CLICKED(IDC_TEST3, OnTest3)
ON_BN_CLICKED(IDC_TEST4, OnTest4)
ON_BN_CLICKED(IDC_END, OnEnd)
	ON_WM_CLOSE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiThread2Dlg message handlers

BOOL CMultiThread2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMultiThread2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMultiThread2Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMultiThread2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void showstr(DWORD dwResult)
{
	CString szLog;
	switch(dwResult)
	{
	case WAIT_ABANDONED:
		{
			szLog = "WAIT_ABANDONED";
		}
		break;
	case WAIT_OBJECT_0:
		{
			szLog = "WAIT_OBJECT_0";
		}
		break;
	case WAIT_TIMEOUT:
		{
			szLog = "WAIT_TIMEOUT";
		}
		break;
	case WAIT_FAILED:
		{
			szLog.Format("WAIT_FAILED: %d", GetLastError());
		}
		break;
	default:
		{
			szLog.Format("%d", dwResult);
		}
		break;
	}
	AfxTrace("%s\n", szLog);
}
void CMultiThread2Dlg::OnTest4()
{
	static int count = 10;
	count++;

	CString szThreadName;
	szThreadName.Format("Thread-%02d", count);

	int integer = m_nCount;

	HANDLE h = CreateThread(NULL,0,ThreadFunc,(LPVOID)(integer),0,NULL);
	CTestAction* pTestAction = new CTestAction(h, szThreadName);
	if(m_monitor.AddMoniteeWaitForExist(h, pTestAction) == RC_ADD_EXIST)
	{
		ASSERT(FALSE);
	}
}

void CMultiThread2Dlg::OnCancel()
{
// 	ASSERT(AfxGetMainWnd());
// 	AfxGetMainWnd()->SendMessage(WM_CLOSE);

	ASSERT(AfxGetMainWnd());
	AfxGetMainWnd()->ShowWindow(SW_HIDE);
	AfxTrace("Hided \n");
	m_monitor.StopMonitor(TRUE);
	AfxTrace("Stopped \n");

	CDialog::OnCancel();
}

void CMultiThread2Dlg::OnClose() 
{
	ASSERT(AfxGetMainWnd());
	AfxGetMainWnd()->ShowWindow(SW_HIDE);
	AfxTrace("Hided \n");
	m_monitor.StopMonitor(TRUE);
	AfxTrace("Stopped \n");

	CDialog::OnClose();
}
void CMultiThread2Dlg::OnEnd()
{
	m_monitor.StopMonitor(TRUE);
}
void CMultiThread2Dlg::OnTest3()
{
	const int nMax = 10;
	
	HANDLE handles[nMax];
	int integer[nMax];
	
	UpdateData(TRUE);

	m_monitor.StartMonitor();
	
	CString szThreadName;
	int i;
	for(i = 0; i < nMax; i++)
	{
		integer[i] = m_nCount + (i / 5);
		handles[i]=CreateThread(NULL,0,ThreadFunc,(LPVOID)(integer[i]),CREATE_SUSPENDED,&ThreadID);

		szThreadName.Format("Thread-%02d", i + 1);
		m_map.SetAt(handles[i], szThreadName);

		CTestAction* pTestAction = new CTestAction(handles[i], szThreadName);
		m_monitor.AddMonitee(handles[i], pTestAction);
	}
	
	for(i = 0; i < nMax; i++)
	{
		ResumeThread(handles[i]);
	}
	
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	
//	WaitForChildThreads(handles, nMax);
}
void CMultiThread2Dlg::OnTest2()
{
	const int nMax = 10;
	
	HANDLE handles[nMax];
	int integer[nMax];
	
	UpdateData(TRUE);
	
	CString szThreadName;
	int i;
	for(i = 0; i < nMax; i++)
	{
		integer[i] = m_nCount + (i / 5);
		handles[i]=CreateThread(NULL,0,ThreadFunc,(LPVOID)(integer[i]),CREATE_SUSPENDED,&ThreadID);
		szThreadName.Format("Thread-%02d", i + 1);
		m_map.SetAt(handles[i], szThreadName);
	}

	for(i = 0; i < nMax; i++)
	{
		ResumeThread(handles[i]);
	}
	
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	
	WaitForChildThreads(handles, nMax);
}
void CMultiThread2Dlg::OnStart() 
{
	UpdateData(TRUE);
	int integer=m_nCount;
	hThread=CreateThread(NULL,0,ThreadFunc,(LPVOID)(integer),0,&ThreadID);
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	
	DWORD dwResult = 0;
	dwResult = WaitForSingleObject(hThread,INFINITE);
	showstr(dwResult);
	
	if(GetExitCodeThread(hThread, &dwResult))
	{
		AfxTrace("Exit code: %d\n", dwResult);
	}
	
	dwResult = WaitForSingleObject(hThread,INFINITE);
	showstr(dwResult);
	
	hThread = (HANDLE)0x1213;
	
	dwResult = WaitForSingleObject(hThread,INFINITE);
	showstr(dwResult);
	
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
}
DWORD WINAPI ThreadFunc(LPVOID x)
{
	int integer = (int)x;
	int i;
	for(i=0;i<integer;i++)
	{
		Sleep(500);
	}
	
	return integer;
}

//http://baike.baidu.com/view/1424795.htm
int CMultiThread2Dlg::WaitForChildThreads(HANDLE* pHandles, const int nMax)
{
/*
DWORD dwResult = 0;
dwResult = WaitForMultipleObjects(nMax, handles, TRUE,INFINITE);

  showstr(dwResult);
  
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	*/
	
	
	DWORD dwRet = 0;
	
	int nIndex = 0;
	
	int nCount = nMax;
	HANDLE* handles = new HANDLE[nCount];
	for(nIndex = 0; nIndex < nCount; nIndex++)
	{
		handles[nIndex] = pHandles[nIndex];
	}
	int* pHandleIndex = new int[nCount];
	for(nIndex = 0; nIndex < nCount; nIndex++)
	{
		pHandleIndex[nIndex] = nIndex;
	}
	int nProcessed = 0;

	int rc = 0;

	int i, j, k;
	while(1)
    {
		if(nCount <= 0)
		{
			break;
		}
		dwRet = WaitForMultipleObjects(nCount,handles,false,INFINITE);
		switch(dwRet)
		{
		case WAIT_TIMEOUT:
			{
				rc = 0;
			}
			break;
		case WAIT_FAILED:
			{
				return 1;
			}
			break;
		default:
			{
				nIndex = dwRet - WAIT_OBJECT_0;

				ProcessHanlde(handles[nIndex]);
				pHandleIndex[nProcessed++] = nIndex;
				nIndex++;
				//同时检测其他的事件
				while(nIndex < nCount) //nCount事件对象总数
				{
					dwRet = WaitForMultipleObjects(nCount - nIndex,&handles[nIndex],false,0);
					switch(dwRet)
					{
					case WAIT_TIMEOUT:
						nIndex = nCount; //退出检测,因为没有被触发的对象了.
						break;
					case WAIT_FAILED:
						return 1;
					default:
						{
							nIndex = nIndex + dwRet - WAIT_OBJECT_0;
							ProcessHanlde(handles[nIndex]);
							pHandleIndex[nProcessed++] = nIndex;
							nIndex++;
						}
						break;
					}
				}

				CString szLog = "MultiSignal: ", szTemp;
				for(j = 0; j < nProcessed; j++)
				{
					szTemp.Format("%s ", GetThreadName(handles[pHandleIndex[j]]));
					szLog += szTemp;
				}
				AfxTrace("%s\n", szLog);

				for(i = 0, j = 0, k = 0; i < nCount; i++)
				{
					if(j >= nProcessed || i != pHandleIndex[j])
					{
						handles[k++] = handles[i];
					}
					else
					{
						j++;
					}
				}


				nCount = k;
				nProcessed = 0;
			}
			break;
		}
    }

	delete pHandleIndex;
	delete handles;

	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	return 0;
}
CString CMultiThread2Dlg::GetThreadName(HANDLE handle)
{
	CString szName;
	
	if(m_map.Lookup(handle, szName))
	{
		
	}
	else
	{
		ASSERT(FALSE);
	}

	return szName;
}
void CMultiThread2Dlg::ProcessHanlde(HANDLE handle)
{
	CString szName;

	if(m_map.Lookup(handle, szName))
	{
		AfxTrace("%s exit\n", szName);
	}
	else
	{
		ASSERT(FALSE);
	}
}
