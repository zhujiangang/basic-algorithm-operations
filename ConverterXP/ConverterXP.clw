; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFileListView
LastTemplate=CView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ConverterXP.h"
LastPage=0

ClassCount=9
Class1=CConverterXPApp
Class3=CMainFrame
Class4=CAboutDlg

ResourceCount=6
Resource1=IDR_RCLICK_POPUP
Resource2=IDR_MAINFRAME (English (U.S.))
Resource3=IDR_RCLICK_POPUP (English (U.S.))
Class2=CChildView
Class5=CDirTreeView
Class6=CFileListView
Class7=CResultView
Class8=zzz
Class9=CSizedSplitterWnd
Resource4=IDR_MAINFRAME
Resource5=IDD_ABOUTBOX (English (U.S.))
Resource6=IDD_ABOUTBOX

[CLS:CConverterXPApp]
Type=0
HeaderFile=ConverterXP.h
ImplementationFile=ConverterXP.cpp
Filter=N

[CLS:CChildView]
Type=0
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=N

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=ConverterXP.cpp
ImplementationFile=ConverterXP.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_EXIT
Command2=ID_EDIT_UNDO
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
Command4=ID_FILE_PRINT
Command5=ID_APP_ABOUT
CommandCount=5

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_APP_EXIT
Command2=ID_EDIT_UNDO
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CDirTreeView]
Type=0
HeaderFile=DirTreeView.h
ImplementationFile=DirTreeView.cpp
BaseClass=CShellTreeCtrl
Filter=C
VirtualFilter=GWC

[CLS:CFileListView]
Type=0
HeaderFile=FileListView.h
ImplementationFile=FileListView.cpp
BaseClass=CShellListCtrl
Filter=C
LastObject=CFileListView
VirtualFilter=FWC

[CLS:CResultView]
Type=0
HeaderFile=ResultView.h
ImplementationFile=ResultView.cpp
BaseClass=CScrollView
Filter=C

[CLS:zzz]
Type=0
HeaderFile=zzz.h
ImplementationFile=zzz.cpp
BaseClass=splitter
Filter=T

[CLS:CSizedSplitterWnd]
Type=0
HeaderFile=SizedSplitterWnd.h
ImplementationFile=SizedSplitterWnd.cpp
BaseClass=CSplitterWnd
Filter=C
VirtualFilter=pWC
LastObject=IDM_ARRANGE_BY_NAME

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
Command4=ID_FILE_PRINT
Command5=ID_APP_ABOUT
CommandCount=5

[MNU:IDR_RCLICK_POPUP (English (U.S.))]
Type=1
Class=?
Command1=IDM_VIEW_LARGE_ICON
Command2=IDM_VIEW_SMALL_ICON
Command3=IDM_VIEW_LIST
Command4=IDM_VIEW_DETAIL
Command5=IDM_ARRANGE_BY_NAME
Command6=IDM_ARRANGE_BY_SIZE
Command7=IDM_ARRANGE_BY_TYPE
Command8=IDM_ARRANGE_BY_DATE
CommandCount=8

[MNU:IDR_RCLICK_POPUP]
Type=1
Class=?
Command1=IDM_VIEW_LARGE_ICON
Command2=IDM_VIEW_SMALL_ICON
Command3=IDM_VIEW_LIST
Command4=IDM_VIEW_DETAIL
Command5=IDM_ARRANGE_BY_NAME
Command6=IDM_ARRANGE_BY_SIZE
Command7=IDM_ARRANGE_BY_TYPE
Command8=IDM_ARRANGE_BY_DATE
CommandCount=8

