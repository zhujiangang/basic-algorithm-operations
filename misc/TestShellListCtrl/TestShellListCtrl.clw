; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CShellListCtrl
LastTemplate=CListCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TestShellListCtrl.h"

ClassCount=4
Class1=CTestShellListCtrlApp
Class2=CTestShellListCtrlDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CShellListCtrl
Resource3=IDD_TESTSHELLLISTCTRL_DIALOG
Resource4=IDR_RCLICK_POPUP

[CLS:CTestShellListCtrlApp]
Type=0
HeaderFile=TestShellListCtrl.h
ImplementationFile=TestShellListCtrl.cpp
Filter=N

[CLS:CTestShellListCtrlDlg]
Type=0
HeaderFile=TestShellListCtrlDlg.h
ImplementationFile=TestShellListCtrlDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_SHELL_LIST

[CLS:CAboutDlg]
Type=0
HeaderFile=TestShellListCtrlDlg.h
ImplementationFile=TestShellListCtrlDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TESTSHELLLISTCTRL_DIALOG]
Type=1
Class=CTestShellListCtrlDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SHELL_LIST,SysListView32,1350631424

[CLS:CShellListCtrl]
Type=0
HeaderFile=ShellListCtrl.h
ImplementationFile=ShellListCtrl.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[MNU:IDR_RCLICK_POPUP]
Type=1
Class=?
Command1=IDM_VIEW_LARGE_ICON
Command2=IDM_VIEW_SMALL_ICON
Command3=IDM_VIEW_LIST
Command4=ID_VIEW_DETAIL
Command5=IDM_ARRANGE_BY_NAME
Command6=IDM_ARRANGE_BY_SIZE
Command7=IDM_ARRANGE_BY_TYPE
Command8=IDM_ARRANGE_BY_DATE
CommandCount=8

