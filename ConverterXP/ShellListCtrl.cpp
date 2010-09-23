// ShellListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ShellListCtrl.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IContextMenu2*	CShellListCtrl::m_pContextMenu2 = NULL;
/////////////////////////////////////////////////////////////////////////////
// CShellListCtrl

CShellListCtrl::CShellListCtrl()
{
	m_psfCurFolder = NULL;
	m_pidlCurFQ = NULL;
	m_bContextMenu = TRUE;
	m_hwndRelatedTree = NULL;
	m_bIsDesktop = FALSE;
	m_bNoNotify = FALSE;
	m_nTypes = (SHCONTF) (SHCONTF_FOLDERS | SHCONTF_NONFOLDERS);
}

CShellListCtrl::~CShellListCtrl()
{
	m_folderChangeObservers.RemoveAll();
}

IMPLEMENT_DYNAMIC(CShellListCtrl, CMyListCtrl)
BEGIN_MESSAGE_MAP(CShellListCtrl, CMyListCtrl)
	//{{AFX_MSG_MAP(CShellListCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_RETURN, OnReturn)
	ON_COMMAND(IDM_VIEW_LARGE_ICON, OnViewLargeIcon)
	ON_COMMAND(IDM_VIEW_SMALL_ICON, OnViewSmallIcon)
	ON_COMMAND(IDM_VIEW_LIST, OnViewList)
	ON_COMMAND(IDM_VIEW_DETAIL, OnViewDetail)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_LARGE_ICON, OnUpdateViewLargeIcon)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_SMALL_ICON, OnUpdateViewSmallIcon)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_LIST, OnUpdateViewList)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_DETAIL, OnUpdateViewDetail)
	ON_COMMAND(IDM_ARRANGE_BY_DATE, OnArrangeByDate)
	ON_COMMAND(IDM_ARRANGE_BY_NAME, OnArrangeByName)
	ON_COMMAND(IDM_ARRANGE_BY_SIZE, OnArrangeBySize)
	ON_COMMAND(IDM_ARRANGE_BY_TYPE, OnArrangeByType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellListCtrl message handlers

BOOL CShellListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.lpszName = _T("ShellListCtrl");
	cs.style = WS_CHILD | WS_VISIBLE | LVS_AUTOARRANGE | LVS_REPORT;
	cs.lpszClass = WC_LISTVIEW;
	return CMyListCtrl::PreCreateWindow(cs);
}
void CShellListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMyListCtrl::PreSubclassWindow();
	
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState ();
	if (pThreadState->m_pWndInit == NULL)
	{
		if (!InitList ())
		{
			ASSERT(FALSE);
		}
		ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	}
}

BOOL CShellListCtrl::InitList ()
{
	if (g_pShellManager == NULL)
	{
		TRACE0("You need to initialize CBCGShellManager first\n");
		return FALSE;
	}
	
	ModifyStyle (0, LVS_SHAREIMAGELISTS);
	
	//------------------
	// Set shell images:
	//------------------
	SetImageList (CImageList::FromHandle (GetShellImageList (TRUE)), LVSIL_NORMAL);
	SetImageList (CImageList::FromHandle (GetShellImageList (FALSE)), LVSIL_SMALL);
	
	//-------------
	// Add columns:
	//-------------
	OnSetColumns ();
	
	if (m_psfCurFolder == NULL)
	{
		//-----------------
		// Display desktop:
		//-----------------
		MYCBITEMINFO info;
		
		if (SUCCEEDED (SHGetSpecialFolderLocation (NULL, CSIDL_DESKTOP, &info.pidlFQ)))
		{
			DisplayFolder (&info);
			g_pShellManager->FreeItem (info.pidlFQ);
		}
	}
	
	return TRUE;
}
BOOL CShellListCtrl::GetCurrentFolder (CString& strPath) const
{
	ASSERT_VALID (this);
	
	strPath.Empty ();
	
	if (m_pidlCurFQ == NULL)
	{
		return FALSE;
	}
	
	TCHAR szPath [MAX_PATH];
	if (!SHGetPathFromIDList (m_pidlCurFQ, szPath))
	{
		return FALSE;
	}
	
	strPath = szPath;
	return TRUE;
}
//***************************************************************************************
BOOL CShellListCtrl::GetCurrentFolderName (CString& strName) const
{
	ASSERT_VALID (this);
	
	strName.Empty ();
	
	if (m_pidlCurFQ == NULL)
	{
		return FALSE;
	}
	
	SHFILEINFO	sfi;
	if (!SHGetFileInfo((LPCTSTR)m_pidlCurFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))
	{
		return FALSE;
	}
	
	strName = sfi.szDisplayName;
	return TRUE;
}

//***************************************************************************************
void CShellListCtrl::OnSetColumns ()
{
	const TCHAR* szName [] = 
	{
		_T("Name"),
			_T("Size"),
			_T("Type"),
			_T("Modified"),
	};
	
	for (int iColumn = 0; iColumn < 4; iColumn++)
	{
		int nFormat = (iColumn == SHELL_LIST_COL_SIZE) ? LVCFMT_RIGHT : LVCFMT_LEFT;
		
		InsertColumn (iColumn, szName [iColumn], nFormat, 100, iColumn);
	}
}

HRESULT CShellListCtrl::DisplayFolder (LPCITEMIDLIST lpidlFQ, BOOL sendNotification)
{
	MYCBITEMINFO info;

	if (g_pShellManager == NULL)
	{
		ASSERT (FALSE);
		return E_FAIL;
	}
	
	ASSERT (lpidlFQ != NULL);
	ASSERT_VALID (g_pShellManager);
	
	BOOL bIsDesktop = ( (g_pShellManager->GetItemCount(lpidlFQ) <= 0) ? TRUE : FALSE );

	HRESULT hr;
	if(bIsDesktop)
	{
		info.pidlFQ = (LPITEMIDLIST)lpidlFQ;
		hr = DisplayFolder (&info, sendNotification);
	}
	else
	{
		LPSHELLFOLDER pDesktopFolder;
		hr = SHGetDesktopFolder(&pDesktopFolder);
		
		if (SUCCEEDED (hr))
		{
			info.pParentFolder = pDesktopFolder;
			info.pidlFQ = (LPITEMIDLIST)lpidlFQ;
			info.pidlRel = (LPITEMIDLIST)lpidlFQ;
			
			hr = DisplayFolder (&info, sendNotification);
			pDesktopFolder->Release();
		}
	}
	return hr;
}

HRESULT CShellListCtrl::DisplayFolder (LPMYCBITEMINFO pItemInfo, BOOL sendNotification)
{
	HRESULT hr = E_FAIL;
	
	if (g_pShellManager == NULL)
	{
		ASSERT (FALSE);
		return hr;
	}
	
	if (pItemInfo != NULL)
	{
		ReleaseCurrFolder();
		hr = LockCurrentFolder(pItemInfo);
		
		if (FAILED(hr))
		{
			return hr;
		}
	}
	
	DeleteAllItems ();
	
	if (m_psfCurFolder != NULL)
	{
		CWaitCursor wait;
		SetRedraw (FALSE);
		
		hr = EnumObjects (m_psfCurFolder, m_pidlCurFQ);
		
		if (GetStyle () & LVS_REPORT)
		{
			Sort(SHELL_LIST_COL_NAME, TRUE);
		}
		
		if((GetStyle () & LVS_TYPEMASK) == LVS_SMALLICON)
		{
			OnViewLargeIcon();
			OnViewSmallIcon();
		}

		SetRedraw (TRUE);
		RedrawWindow ();
	}
	
	if (sendNotification && SUCCEEDED (hr) && pItemInfo != NULL)
	{
		POSITION ps = m_folderChangeObservers.GetHeadPosition();
		while(ps != NULL)
		{
			HWND hwnd = m_folderChangeObservers.GetNext(ps);
			::SendMessage(hwnd, MSG_SHELL_LIST_CHANGE_CURRENT_FOLDER, 0, (LPARAM)m_pidlCurFQ);
		}
	}
	
	return hr;
}
//***************************************************************************************
HRESULT CShellListCtrl::DisplayParentFolder ()
{
	ASSERT_VALID (g_pShellManager);
	
	HRESULT hr = E_FAIL;
	if (m_pidlCurFQ == NULL)
	{
		return hr;
	}
	
	MYCBITEMINFO info;
	int nLevel = g_pShellManager->GetParentItem (m_pidlCurFQ, info.pidlFQ);
	
	if (nLevel < 0)
	{
		return hr;
	}
	
	if (nLevel == 0)	// Desktop
	{
		hr = DisplayFolder (&info);
	}
	else
	{
		LPSHELLFOLDER pDesktopFolder;
		hr = SHGetDesktopFolder(&pDesktopFolder);
		
		if (SUCCEEDED (hr))
		{
			info.pParentFolder = pDesktopFolder;
			info.pidlRel = info.pidlFQ;
			
			hr = DisplayFolder (&info);
			pDesktopFolder->Release();
		}
	}
	
	g_pShellManager->FreeItem (info.pidlFQ);
	return hr;
}
HRESULT CShellListCtrl::DisplayFolder (LPCTSTR lpszPath)
{
	if (g_pShellManager == NULL)
	{
		ASSERT (FALSE);
		return E_FAIL;
	}
	
	ASSERT (lpszPath != NULL);
	ASSERT_VALID (g_pShellManager);
	
	MYCBITEMINFO info;
	HRESULT hr = g_pShellManager->ItemFromPath (lpszPath, info.pidlRel);
	
	if (FAILED (hr))
	{
		return hr;
	}
	
	LPSHELLFOLDER pDesktopFolder;
	hr = SHGetDesktopFolder(&pDesktopFolder);
	
	if (SUCCEEDED (hr))
	{
		info.pParentFolder = pDesktopFolder;
		info.pidlFQ = info.pidlRel;
		
		hr = DisplayFolder (&info);
		pDesktopFolder->Release();
	}
	
	g_pShellManager->FreeItem (info.pidlFQ);
	return hr;
}
//****************************************************************************************
HRESULT CShellListCtrl::Refresh ()
{
	return DisplayFolder ((LPMYCBITEMINFO) NULL);
}
HRESULT CShellListCtrl::EnumObjects (LPSHELLFOLDER pParentFolder,
									 LPITEMIDLIST pidlParent)
{
	ASSERT_VALID (this);
	ASSERT_VALID (g_pShellManager);
	
	LPENUMIDLIST pEnum;
	HRESULT hRes = pParentFolder->EnumObjects (NULL, m_nTypes, &pEnum);
	
	if (SUCCEEDED (hRes))
	{
		LPITEMIDLIST	pidlTemp;
		DWORD			dwFetched = 1;
		LPMYCBITEMINFO pItem;
		
		//enumerate the item's PIDLs
		while (pEnum->Next(1, &pidlTemp, &dwFetched) == S_OK && dwFetched)
		{
			LVITEM lvItem;
			ZeroMemory(&lvItem, sizeof(lvItem));
			
			//fill in the TV_ITEM structure for this item
			lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
			
			//AddRef the parent folder so it's pointer stays valid
			pParentFolder->AddRef();
			
			//put the private information in the lParam
			pItem = (LPMYCBITEMINFO)GlobalAlloc(GPTR, sizeof(MYCBITEMINFO));
			
			pItem->pidlRel = pidlTemp;
			pItem->pidlFQ = g_pShellManager->ConcatenateItem (pidlParent, pidlTemp);
			
			pItem->pParentFolder = pParentFolder;
			lvItem.lParam = (LPARAM)pItem;
			
			lvItem.pszText = _T("");
			lvItem.iImage = OnGetItemIcon (GetItemCount (), pItem);
			
			//determine if the item is shared
			DWORD dwAttr = SFGAO_DISPLAYATTRMASK;
			pParentFolder->GetAttributesOf (1, (LPCITEMIDLIST*)&pidlTemp, &dwAttr);
			
			if (dwAttr & SFGAO_SHARE)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_OVERLAYMASK;
				lvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
			}
			
			if (dwAttr & SFGAO_GHOSTED)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_CUT;
				lvItem.state |= LVIS_CUT;
			}
			
			int iItem = InsertItem (&lvItem);
			if (iItem >= 0)
			{
				//-------------
				// Set columns:
				//-------------
				const int nColumns = (GetStyle () & LVS_REPORT) ?
					//@TODO
					4 : 4;
				
				for (int iColumn = 0; iColumn < nColumns; iColumn++)
				{
					SetItemText (iItem, iColumn, 
						OnGetItemText (iItem, iColumn, pItem));
				}
			}
			
			dwFetched = 0;
		}
		
		pEnum->Release ();
	}
	
	return hRes;
}
void CShellListCtrl::DoDefault (int iItem)
{
	LVITEM lvItem;
	
	ZeroMemory(&lvItem, sizeof(lvItem));
	lvItem.mask = LVIF_PARAM;
	lvItem.iItem = iItem;
	
	if (!GetItem (&lvItem))
	{
		return;
	}

	LPMYCBITEMINFO	pInfo = (LPMYCBITEMINFO) lvItem.lParam;
	if (pInfo == NULL || pInfo->pParentFolder == NULL || pInfo->pidlRel == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	IShellFolder *psfFolder = pInfo->pParentFolder;
	if (psfFolder == NULL)
	{
		SHGetDesktopFolder (&psfFolder);
	}
	else
	{
		psfFolder->AddRef ();
	}
	
	if (psfFolder == NULL)
	{
		return;
	}

	//-----------------------------------------------------
	// If specified element is a folder, try to display it:
	//-----------------------------------------------------
	ULONG ulAttrs = SFGAO_FOLDER;
	psfFolder->GetAttributesOf (1, 
		(const struct _ITEMIDLIST **) &pInfo->pidlRel, &ulAttrs);

	if (ulAttrs & SFGAO_FOLDER)
	{
		DisplayFolder (pInfo);
	}
	else
	{
		//-------------------------------
		// Invoke a default menu command:
		//-------------------------------
		IContextMenu *pcm;
		HRESULT hr = psfFolder->GetUIObjectOf (GetSafeHwnd (),
			1, 
			(LPCITEMIDLIST*)&pInfo->pidlRel, 
			IID_IContextMenu, 
			NULL, 
			(LPVOID*)&pcm);
		
		if (SUCCEEDED (hr))
		{
			HMENU hPopup = CreatePopupMenu ();

			if (hPopup != NULL)
			{
				hr = pcm->QueryContextMenu (hPopup, 0, 1, 0x7fff, 
											CMF_DEFAULTONLY | CMF_EXPLORE);
				
				if (SUCCEEDED (hr))
				{
					UINT idCmd = ::GetMenuDefaultItem (hPopup, FALSE, 0);
					if (idCmd != 0 && idCmd != (UINT)-1)
					{
						CMINVOKECOMMANDINFO cmi;
						cmi.cbSize = sizeof (CMINVOKECOMMANDINFO);
						cmi.fMask = 0;
						cmi.hwnd = GetParent()->GetSafeHwnd ();
						cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
						cmi.lpParameters = NULL;
						cmi.lpDirectory = NULL;
						cmi.nShow = SW_SHOWNORMAL;
						cmi.dwHotKey = 0;
						cmi.hIcon = NULL;

						hr = pcm->InvokeCommand (&cmi);

						if (SUCCEEDED (hr) && GetParent () != NULL)
						{
							GetParent ()->SendMessage (MYPM_ON_AFTER_SHELL_COMMAND,
								(WPARAM) idCmd);
						}
					}
				}
			}
			
			pcm->Release ();
		}
	}
	
	psfFolder->Release ();
}

CString CShellListCtrl::OnGetItemText (int /*iItem*/, int iColumn, 
									  LPMYCBITEMINFO pItem)
{
	ASSERT_VALID (this);
	ASSERT (pItem != NULL);

	SHFILEINFO	sfi;
	TCHAR		szPath [MAX_PATH];

	switch (iColumn)
	{
	case SHELL_LIST_COL_NAME:
		if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))
		{
			return sfi.szDisplayName;
		}
		break;
		
	case SHELL_LIST_COL_TYPE:
		if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_TYPENAME))
		{
			return sfi.szTypeName;
		}
		break;

	case SHELL_LIST_COL_SIZE:
	case SHELL_LIST_COL_DATE:
		if (SHGetPathFromIDList (pItem->pidlFQ, szPath))
		{
			CFileStatus fs;
			if (CFile::GetStatus (szPath, fs))
			{
				CString str;

				if (iColumn == 1)
				{
					if ((fs.m_attribute & (CFile::directory | CFile ::volume)) == 0)
					{
						OnFormatFileSize ((long) fs.m_size, str);
					}
				}
				else
				{
					OnFormatFileDate (fs.m_mtime, str);
				}

				return str;
			}
		}
		break;

	default:
		ASSERT (FALSE);
		break;
	}

	return _T("");
}
//***************************************************************************************
int CShellListCtrl::OnGetItemIcon (int /*iItem*/, LPMYCBITEMINFO pItem)
{
	ASSERT_VALID (this);
	ASSERT (pItem != NULL);

	SHFILEINFO	sfi;
	int			iIcon = -1;
		
	if (SHGetFileInfo ((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof (sfi), 
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_LINKOVERLAY))
	{
		iIcon = sfi.iIcon;
	}

	return iIcon;
}
//***************************************************************************************
int CShellListCtrl::OnCompareItems (LPARAM lParam1, LPARAM lParam2, int iColumn)
{
	ASSERT_VALID (this);
	LPMYCBITEMINFO pItem1 = (LPMYCBITEMINFO)lParam1;
	LPMYCBITEMINFO	pItem2 = (LPMYCBITEMINFO)lParam2;
	ASSERT (pItem1 != NULL);
	ASSERT (pItem2 != NULL);

	SHFILEINFO	sfi1;
	SHFILEINFO	sfi2;

	TCHAR szPath1 [MAX_PATH];
	TCHAR szPath2 [MAX_PATH];

	CFileStatus fs1;
	CFileStatus fs2;

	int nRes = 0;

	switch (iColumn)
	{
	case SHELL_LIST_COL_NAME:
		{
			HRESULT hr = pItem1->pParentFolder->CompareIDs (0,
				pItem1->pidlRel,
				pItem2->pidlRel);
			
			if (FAILED (hr))
			{
				return 0;
			}
			
			nRes = (short) SCODE_CODE (GetScode (hr));
		}
		break;

	case SHELL_LIST_COL_TYPE:
		if (SHGetFileInfo ((LPCTSTR)pItem1->pidlFQ, 0, &sfi1, sizeof(sfi1), SHGFI_PIDL | SHGFI_TYPENAME) &&
			SHGetFileInfo ((LPCTSTR)pItem2->pidlFQ, 0, &sfi2, sizeof(sfi2), SHGFI_PIDL | SHGFI_TYPENAME))
		{
			nRes = lstrcmpi (sfi1.szTypeName, sfi2.szTypeName);
		}
		break;

	case SHELL_LIST_COL_SIZE:
	case SHELL_LIST_COL_DATE:
		if (SHGetPathFromIDList (pItem1->pidlFQ, szPath1) &&
			CFile::GetStatus (szPath1, fs1))
		{
			if (SHGetPathFromIDList (pItem2->pidlFQ, szPath2) &&
				CFile::GetStatus (szPath2, fs2))
			{
				if (iColumn == SHELL_LIST_COL_SIZE)
				{
					nRes =	fs1.m_size < fs2.m_size ? -1 : 
							fs1.m_size > fs2.m_size ? 1 : 0;
				}
				else
				{
					nRes =	fs1.m_mtime < fs2.m_mtime ? -1 : 
							fs1.m_mtime > fs2.m_mtime ? 1 : 0;
				}
			}
			else
			{
				nRes = 1;
			}
		}
		else
		{
			nRes = -1;
		}
		break;
	}

	return nRes;
}
HIMAGELIST CShellListCtrl::GetShellImageList (BOOL bLarge)
{
	TCHAR szWinDir [MAX_PATH + 1];
	if (GetWindowsDirectory (szWinDir, MAX_PATH) == 0)
	{
		return NULL;
	}
	
	SHFILEINFO sfi;
	HIMAGELIST hImageList = (HIMAGELIST) SHGetFileInfo (szWinDir,
		0,
		&sfi, 
		sizeof (SHFILEINFO), 
		SHGFI_SYSICONINDEX | (bLarge ? 0 : SHGFI_SMALLICON));
	return hImageList;
}
//***************************************************************************************
HRESULT CShellListCtrl::LockCurrentFolder (LPMYCBITEMINFO pItemInfo)
{
	ASSERT_VALID (g_pShellManager);
	
	HRESULT hr = E_FAIL;
	m_pidlCurFQ = NULL;
	
	if (pItemInfo != NULL && pItemInfo->pParentFolder != NULL)
	{
		ASSERT (pItemInfo->pidlRel != NULL);
		hr = pItemInfo->pParentFolder->BindToObject(pItemInfo->pidlRel, NULL, IID_IShellFolder, (LPVOID*)&m_psfCurFolder);
		
		m_bIsDesktop = FALSE;
	}
	else
	{
		hr = SHGetDesktopFolder (&m_psfCurFolder);
		m_bIsDesktop = TRUE;
	}
	
	if (SUCCEEDED (hr))
	{
		m_pidlCurFQ = g_pShellManager->CopyItem (pItemInfo->pidlFQ);
	}
	
	return hr;
}

void CShellListCtrl::ReleaseCurrFolder ()
{
	ASSERT_VALID (g_pShellManager);
	
	if (m_psfCurFolder != NULL)
	{
		m_psfCurFolder->Release();
		m_psfCurFolder = NULL;
		
		g_pShellManager->FreeItem (m_pidlCurFQ);
		m_pidlCurFQ = NULL;
	}
}

void CShellListCtrl::OnFormatFileDate (const CTime& tmFile, CString& str)
{
	COleDateTime dateFile (tmFile.GetTime ());
	str = dateFile.Format ();
}

void CShellListCtrl::OnFormatFileSize (long lFileSize, CString& str)
{
	str.Empty ();
	
	if (lFileSize == 0)
	{
		str = _T("0");
	}
	else
	{
		lFileSize = lFileSize / 1024 + 1;
		str.Format (_T("%ld"), lFileSize);
		
		//-------------------------------------
		// Convert number to the system format:
		//-------------------------------------
		TCHAR szNumOut [256];
		GetNumberFormat (LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE, str,
			NULL, szNumOut, 255);
		
		str = szNumOut;
		
		//----------------------------------
		// Truncate trailing fractal digits:
		//----------------------------------
		TCHAR szDec [10];
		GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, szDec, 10);
		
		int nDecLen = lstrlen (szDec);
		if (nDecLen > 0)
		{
			for (int i = str.GetLength () - nDecLen - 1; i >= 0; i--)
			{
				if (str.Mid (i, nDecLen) == szDec)
				{
					str = str.Left (i);
					break;
				}
			}
		}
	}
	
	str += _T(" KB");
}

int CShellListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMyListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	if (!InitList ())
	{
		return -1;
	}
	return 0;
}

void CShellListCtrl::OnDestroy() 
{
	//@TODO
	/*
	CBCGPShellTree* pTree = GetRelatedTree ();
	if (pTree != NULL)
	{
		ASSERT_VALID (pTree);
		pTree->m_hwndRelatedList = NULL;
	}
	*/
	ReleaseCurrFolder ();

	CMyListCtrl::OnDestroy();
	
	// TODO: Add your message handler code here

}
LRESULT CShellListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_INITMENUPOPUP:
 	case WM_DRAWITEM:
	case WM_MEASUREITEM:
		if (m_pContextMenu2 != NULL)
		{
			m_pContextMenu2->HandleMenuMsg(message, wParam, lParam);
			return 0;
		}
		break;
	}
	
	return CMyListCtrl::WindowProc(message, wParam, lParam);
}
void CShellListCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	ASSERT_VALID (this);
	ASSERT_VALID (g_pShellManager);
	ASSERT (g_pShellManager->m_pBCGMalloc != NULL);

	if (m_pContextMenu2 != NULL)
	{
		return;
	}

	if (!m_bContextMenu)
	{
		Default ();
		return;
	}

	if (m_psfCurFolder == NULL)
	{
		return;
	}

	UINT	nSelItems = GetSelectedCount ();
	int		nClickedItem = -1;

	if (point.x == -1 && point.y == -1)
	{
		//--------------------------------------------------------
		// Keyboard, show menu for the currently selected item(s):
		//--------------------------------------------------------
		if (nSelItems == 0)
		{
			return;
		}

		int nCurItem = -1;
		int nLastSelItem = -1;

		for (UINT i = 0; i < nSelItems; i++)
		{
			nCurItem = GetNextItem (nCurItem, LVNI_SELECTED);
			nLastSelItem = nCurItem;
		}

		CRect rectItem;
		if (GetItemRect (nLastSelItem, rectItem, LVIR_BOUNDS))
		{
			point.x = rectItem.left;
			point.y = rectItem.bottom + 1;

			ClientToScreen (&point);
		}
	}
	else
	{
		//--------------------------
		// Clicked on specifed item:
		//--------------------------
		LVHITTESTINFO lvhti;
		lvhti.pt = point;
		ScreenToClient(&lvhti.pt);
		
		lvhti.flags = LVHT_NOWHERE;
		
		HitTest (&lvhti);
		
		if ((lvhti.flags & LVHT_ONITEM) == 0)
		{
			//-----------------------------------
			// Click ouside of items, do nothing
			//-----------------------------------
			CMenu menu;
			CMenu* pPopup;
			
			// the font popup is stored in a resource
			menu.LoadMenu(IDR_RCLICK_POPUP);
			pPopup = menu.GetSubMenu(0);
			pPopup->TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y, this );
			return;
		}

		nClickedItem = lvhti.iItem;
	}

	LPITEMIDLIST* pPidls = (LPITEMIDLIST*) 
						g_pShellManager->m_pBCGMalloc->Alloc (sizeof(LPITEMIDLIST) * nSelItems);
	ASSERT (pPidls != NULL);

	//------------------------
	// Get the selected items:
	//------------------------
	LVITEM lvItem;
	ZeroMemory(&lvItem, sizeof(lvItem));
	lvItem.mask = LVIF_PARAM;

	LPMYCBITEMINFO pClickedInfo = (LPMYCBITEMINFO)lvItem.lParam;

	if (nClickedItem >= 0)
	{
		//-------------------------------------------
		// Put the item clicked on first in the list:
		//-------------------------------------------
		lvItem.iItem = nClickedItem;

		if (GetItem (&lvItem))
		{
			pClickedInfo = (LPMYCBITEMINFO)lvItem.lParam;
			pPidls [0] = pClickedInfo->pidlRel;
		}
	}
	
	int nCurItem = -1;
	for (UINT i = nClickedItem >= 0 ? 1 : 0; i < nSelItems; i++)
	{
		nCurItem = GetNextItem (nCurItem, LVNI_SELECTED);
		if (nCurItem != nClickedItem)
		{
			lvItem.iItem = nCurItem;

			if (GetItem (&lvItem))
			{
				LPMYCBITEMINFO pInfo = (LPMYCBITEMINFO)lvItem.lParam;
				pPidls [i] = pInfo->pidlRel;

				if (pClickedInfo == NULL)
				{
					pClickedInfo = pInfo;
				}
			}
		}
		else
		{
			i--;
		}
	}
	
	if (pPidls [0] == NULL)
	{
		g_pShellManager->m_pBCGMalloc->Free (pPidls);
		return;
	}

	IContextMenu* pcm;
	HRESULT hr = m_psfCurFolder->GetUIObjectOf (GetSafeHwnd (),
		nSelItems, 
		(LPCITEMIDLIST*)pPidls, 
		IID_IContextMenu, 
		NULL, 
		(LPVOID*)&pcm);
	
	if (SUCCEEDED (hr))
	{
		hr = pcm->QueryInterface (IID_IContextMenu2, (LPVOID*)&m_pContextMenu2);

		if (SUCCEEDED (hr))
		{
			HMENU hPopup = CreatePopupMenu ();
			if (hPopup != NULL)
			{
				hr = m_pContextMenu2->QueryContextMenu(hPopup, 0, 1, 0x7fff, CMF_NORMAL | CMF_EXPLORE);
				
				if (SUCCEEDED(hr))
				{
					UINT idCmd = TrackPopupMenu (hPopup,
							TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON, 
							point.x,
							point.y,
							0,
							GetSafeHwnd (),
							NULL);

					if (idCmd != 0)
					{
						BOOL bIsFolder = FALSE;

						if (nSelItems == 1 &&
							idCmd == ::GetMenuDefaultItem (hPopup, FALSE, 0))
						{
							//-----------------------------------------------------
							// If specified element is a folder, try to display it:
							//-----------------------------------------------------
							ULONG ulAttrs = SFGAO_FOLDER;
							m_psfCurFolder->GetAttributesOf (1, 
								(const struct _ITEMIDLIST **) &pClickedInfo->pidlRel, &ulAttrs);

							if (ulAttrs & SFGAO_FOLDER)
							{
								bIsFolder = TRUE;
								DisplayFolder (pClickedInfo);
							}
						}

						if (!bIsFolder)
						{
							CMINVOKECOMMANDINFO cmi;
							cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
							cmi.fMask = 0;
							cmi.hwnd = (HWND) GetParent();
							cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
							cmi.lpParameters = NULL;
							cmi.lpDirectory = NULL;
							cmi.nShow = SW_SHOWNORMAL;
							cmi.dwHotKey = 0;
							cmi.hIcon = NULL;

							hr = pcm->InvokeCommand (&cmi);

							if (SUCCEEDED (hr) && GetParent () != NULL)
							{
								GetParent ()->SendMessage (MYPM_ON_AFTER_SHELL_COMMAND,
									(WPARAM) idCmd);
							}
						}
					}
				}
			}

			if (m_pContextMenu2 != NULL)
			{
				m_pContextMenu2->Release();
				m_pContextMenu2 = NULL;
			}
		}
		
		pcm->Release();
	}
	
	g_pShellManager->m_pBCGMalloc->Free (pPidls);
}


void CShellListCtrl::OnViewLargeIcon() 
{
	// TODO: Add your command handler code here
	ModifyStyle(LVS_TYPEMASK, LVS_ICON);
}

void CShellListCtrl::OnViewSmallIcon() 
{
	// TODO: Add your command handler code here
	ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
}

void CShellListCtrl::OnViewList() 
{
	// TODO: Add your command handler code here
	ModifyStyle(LVS_TYPEMASK, LVS_LIST);
}

void CShellListCtrl::OnViewDetail() 
{
	// TODO: Add your command handler code here
	ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}

void CShellListCtrl::OnUpdateViewLargeIcon(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio ((GetStyle () & LVS_TYPEMASK) == LVS_ICON);
}

void CShellListCtrl::OnUpdateViewSmallIcon(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio ((GetStyle () & LVS_TYPEMASK) == LVS_SMALLICON);
}

void CShellListCtrl::OnUpdateViewList(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio ((GetStyle () & LVS_TYPEMASK) == LVS_LIST);
}

void CShellListCtrl::OnUpdateViewDetail(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio ((GetStyle () & LVS_TYPEMASK) == LVS_REPORT);
}

void CShellListCtrl::OnArrangeByDate() 
{
	// TODO: Add your command handler code here
	Sort(SHELL_LIST_COL_DATE);
}

void CShellListCtrl::OnArrangeByName() 
{
	// TODO: Add your command handler code here
	Sort(SHELL_LIST_COL_NAME);
}

void CShellListCtrl::OnArrangeBySize() 
{
	// TODO: Add your command handler code here
	Sort(SHELL_LIST_COL_SIZE);
}

void CShellListCtrl::OnArrangeByType() 
{
	Sort(SHELL_LIST_COL_TYPE);
}

void CShellListCtrl::AddObserver(HWND hwnd)
{
	m_folderChangeObservers.AddTail(hwnd);
}
void CShellListCtrl::RemoveObserver(HWND hwnd)
{
	POSITION ps = m_folderChangeObservers.Find(hwnd);
	if(ps != NULL)
	{
		m_folderChangeObservers.RemoveAt(ps);
	}
}

//*****************************************************************************************
void CShellListCtrl::OnDblclk(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	int nItem = GetNextItem(-1, LVNI_FOCUSED);
	if (nItem != -1)
	{
		DoDefault (nItem);
	}
	
	*pResult = 0;
}
//****************************************************************************************
void CShellListCtrl::OnReturn(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	int nItem = GetNextItem(-1, LVNI_FOCUSED);
	if (nItem != -1)
	{
		DoDefault(nItem);
	}
	
	*pResult = 0;
}

