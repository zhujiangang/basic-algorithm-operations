// ShellTreeCtrl.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000-2001 by Paolo Messina
// (http://www.geocities.com/ppescher - ppescher@yahoo.com)
//
// The contents of this file are subject to the Artistic License (the "License").
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.opensource.org/licenses/artistic-license.html
//
// If you find this code useful, credits would be nice!
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShellTreeCtrl.h"
#include "MyShellManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShellTreeCtrl

CShellTreeCtrl::CShellTreeCtrl()
{
	m_nCallbackMask = 0;
}

CShellTreeCtrl::~CShellTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CShellTreeCtrl, CWaitingTreeCtrl)
	//{{AFX_MSG_MAP(CShellTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteItem)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, OnGetDispInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellTreeCtrl message handlers

BOOL CShellTreeCtrl::PopulateItem(HTREEITEM hParent)
{
	if (hParent == TVI_ROOT)
	{
		// not handled yet, do nothing in Release builds
		ASSERT(FALSE);
		return TRUE;
	}

	TVITEMDATA* pData = (TVITEMDATA*)GetItemData(hParent);
	if (pData == NULL)
		return TRUE;	// invalid shell item, ignore it silently

	// get parent pidl
	ASSERT(pData->IsValid());
	CShellPidl& pidlParent = pData->pidlAbs;

	if (!EnumFolderItems(hParent, pidlParent, pData->nFlags))
		return TRUE;	// failed, won't try anymore!

// TODO: change this method!!

	// do not check for children if parent is a removable media
	// (just try: if it's a filesystem object, it has a path)
	TCHAR path[MAX_PATH];
	if (SHGetPathFromIDList(pidlParent, path))
	{
		path[3] = 0;
		UINT type = GetDriveType(path);
		if (type != DRIVE_FIXED)
			return FALSE;
	}

	return TRUE;
}

void CShellTreeCtrl::PreSubclassWindow() 
{
	InitializeControl();

	CWaitingTreeCtrl::PreSubclassWindow();
}

void CShellTreeCtrl::InitializeControl()
{
    // Attach to the system image list
	CShellPidl pidl((UINT)CSIDL_DESKTOP, m_hWnd);
    
    SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	HIMAGELIST hSysImageList = (HIMAGELIST) SHGetFileInfo((LPCTSTR)(LPCITEMIDLIST)pidl,
		0, &sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	//TreeView_SetImageList(m_hWnd, hSysImageList, TVSIL_NORMAL);
	// postpone imagelist attaching
	// (seems it doesn't like a sendmessage when dynamically created
	// maybe because it has not received the WM_CREATE message yet?)
	PostMessage(TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)hSysImageList);
}

void CShellTreeCtrl::AddRootFolderContent(LPCITEMIDLIST pidlFolder, UINT nFlags)
{
	if (pidlFolder == NULL)
	{
		CShellPidl pidl((UINT)CSIDL_DESKTOP, m_hWnd);
		InsertSubItem(TVI_ROOT, m_pDesktopFolder, NULL, pidl, nFlags);
		return;
	}

	SetRedraw(FALSE);
	EnumFolderItems(TVI_ROOT, pidlFolder, nFlags);
	SetRedraw(TRUE);
}


int CALLBACK CShellTreeCtrl::CompareFunc(LPARAM lParam1,
		LPARAM lParam2, LPARAM /*lParamSort*/)
{
	TVITEMDATA* pData1 = (TVITEMDATA*)lParam1;
	TVITEMDATA* pData2 = (TVITEMDATA*)lParam2;
	ASSERT(pData1->IsValid() && pData2->IsValid());

	// TODO: parent folders should be checked some day
	SShellFolderPtr pParentFolder = pData2->pParentFolder;

	HRESULT hr = pParentFolder->CompareIDs(0,
		pData1->pidlAbs.GetLastChild(),
		pData2->pidlAbs.GetLastChild() );
	if (FAILED(hr))
		return 0;	// error, don't sort
	
	short ret = (short)HRESULT_CODE(hr);
	if (ret < 0)
		return -1;
	if (ret > 0)
		return 1;
	return 0;
}

void CShellTreeCtrl::FillItem(TVITEM& item)
{
	DWORD dwAttributes;

	// get item data
	TVITEMDATA* pData = (TVITEMDATA*)item.lParam;
	ASSERT(pData->IsValid());

	// get a relative pidl
	LPCITEMIDLIST pidlRel = pData->pidlAbs.GetLastChild();

	if (item.mask & TVIF_TEXT)
	{
		// get display name
		CString sName;
		CShellString str;

		if (pData->nFlags & STCF_SHOWPATH)
		{
			// use an absolute or relative path, if possible
			sName = pData->pidlAbs.GetPath();
			if (!sName.IsEmpty() && !(pData->nFlags & STCF_SHOWFULLNAME))
				sName = sName.Right(sName.ReverseFind(_T('\\')));
		}
		if (sName.IsEmpty())
		{
			// use a global or contextual displayname
			DWORD uDisplayFlags = SHGDN_INFOLDER;
			if (pData->nFlags & STCF_SHOWFULLNAME)
				uDisplayFlags = SHGDN_NORMAL;

			pData->pParentFolder->GetDisplayNameOf(pidlRel, uDisplayFlags
				| SHGDN_INCLUDE_NONFILESYS, str.GetPointer(pidlRel));
			sName = str;	// copy to string
		}
		// set item text
		lstrcpyn(item.pszText, (LPCTSTR)sName, item.cchTextMax);
	}

	if (item.mask & (TVIF_IMAGE | TVIF_SELECTEDIMAGE))
	{
		// get some attributes
		dwAttributes = SFGAO_FOLDER | SFGAO_LINK | SFGAO_SHARE | SFGAO_GHOSTED;
		pData->pParentFolder->GetAttributesOf(1, &pidlRel, &dwAttributes);

		// set correct icon
		if (dwAttributes & SFGAO_GHOSTED)
		{
			item.mask |= LVIF_STATE;
			item.stateMask |= LVIS_CUT;
			item.state |= LVIS_CUT;
		}
		if (dwAttributes & SFGAO_SHARE)
		{
			item.mask |= LVIF_STATE;
			item.state &= ~LVIS_OVERLAYMASK;
			item.state |= INDEXTOOVERLAYMASK(1);
			item.stateMask |= LVIS_OVERLAYMASK;
		}
		else if (dwAttributes & SFGAO_LINK)
		{
			item.mask |= LVIF_STATE;
			item.state &= ~LVIS_OVERLAYMASK;
			item.state |= INDEXTOOVERLAYMASK(2);
			item.stateMask |= LVIS_OVERLAYMASK;
		}

		if (item.mask & TVIF_IMAGE)
		{
			item.iImage = pData->pidlAbs.GetIconIndex(SHGFI_SMALLICON);
			item.iSelectedImage = item.iImage;
		}
		if ((item.mask & TVIF_SELECTEDIMAGE)
			&& (dwAttributes & SFGAO_FOLDER))
		{
			item.iSelectedImage = pData->pidlAbs.GetIconIndex(SHGFI_SMALLICON
				|SHGFI_OPENICON);
		}
	}

	if (item.mask & TVIF_CHILDREN)
	{
		// get some attributes
		dwAttributes = SFGAO_FOLDER;
		pData->pParentFolder->GetAttributesOf(1, &pidlRel, &dwAttributes);

		// get children
		item.cChildren = 0;
		if (dwAttributes & SFGAO_FOLDER)
		{
			if (pData->nFlags & STCF_INCLUDEFILES)
				item.cChildren = 1;
			else if (dwAttributes & SFGAO_REMOVABLE)
				item.cChildren = 1;
			else
			{
				dwAttributes = SFGAO_HASSUBFOLDER;
				pData->pParentFolder->GetAttributesOf(1, &pidlRel, &dwAttributes);

				item.cChildren = (dwAttributes & SFGAO_HASSUBFOLDER) ? 1 : 0;
			}
		}
	}

}

void CShellTreeCtrl::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TVITEM& item = ((LPNMTREEVIEW)pNMHDR)->itemOld;

	// free item data, ignore invalid shell items
	if (item.lParam != 0)
		delete (TVITEMDATA*)item.lParam;

	*pResult = 0;
}

void CShellTreeCtrl::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TVITEM& item = ((LPNMTVDISPINFO)pNMHDR)->item;

	// use the provided buffer for text
	FillItem(item);

	*pResult = 0;
}

CShellPidl CShellTreeCtrl::GetItemIDList(HTREEITEM hItem)
{
	TVITEMDATA* pData = (TVITEMDATA*)GetItemData(hItem);
	if (pData != NULL)
	{
		ASSERT(pData->IsValid());
		return pData->pidlAbs;
	}
	return CShellPidl();	// invalid pidl
}

void CShellTreeCtrl::InsertSubItem(HTREEITEM hParent, LPSHELLFOLDER pParentFolder, LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidl, UINT nFlags)
{
	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(TVINSERTSTRUCT));
	tvis.hParent = hParent;
	tvis.hInsertAfter = TVI_LAST;

	// provide a buffer for the item text
	TCHAR szText[MAX_PATH];
	tvis.item.pszText = szText;
	tvis.item.cchTextMax = MAX_PATH;

	// used fields
	const UINT nTVIFlags = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE
		| TVIF_CHILDREN | TVIF_PARAM;

	// prepare item data
	TVITEMDATA* pData = new TVITEMDATA;
	pData->pidlAbs.Combine(pidlParent, pidl);
	pData->pParentFolder = pParentFolder;
	pData->nFlags = nFlags;

	// set item data
	ASSERT(pData->IsValid());
	tvis.item.lParam = (LPARAM)pData;

	// fill with pidl, text, icons and children - handle callbacks
	tvis.item.mask = nTVIFlags & ~m_nCallbackMask;
	FillItem(tvis.item);

	if (m_nCallbackMask & TVIF_IMAGE)
		tvis.item.iImage = I_IMAGECALLBACK;
	if (m_nCallbackMask & TVIF_SELECTEDIMAGE)
		tvis.item.iSelectedImage = I_IMAGECALLBACK;
	if (m_nCallbackMask & TVIF_TEXT)
		tvis.item.pszText = LPSTR_TEXTCALLBACK;
	if (m_nCallbackMask & TVIF_CHILDREN)
		tvis.item.cChildren = I_CHILDRENCALLBACK;
	tvis.item.mask |= nTVIFlags;

	// then insert new item
	InsertItem(&tvis);
}

void CShellTreeCtrl::AddRootItem(LPCITEMIDLIST pidlRoot, UINT nFlags)
{
	// not needed if pParentFolder is an argument
	CShellPidl pidlParent;
	pidlParent.CloneLastParent(pidlRoot);
	SShellFolderPtr pParentFolder(m_pDesktopFolder, pidlParent);
	
	InsertSubItem(TVI_ROOT, pParentFolder, NULL, pidlRoot, nFlags);
}

BOOL CShellTreeCtrl::EnumFolderItems(HTREEITEM hParent, LPCITEMIDLIST pidlParent, UINT nFlags)
{
	// get parent shell folder
	SShellFolderPtr pParentFolder(m_pDesktopFolder, pidlParent);

	// not a valid folder object
	if (!pParentFolder.IsValid())
		return FALSE;	//  failed!

	// enum child pidls
	SEnumIDListPtr pEnumIDList(pParentFolder, SHCONTF_FOLDERS
		| ((nFlags & STCF_INCLUDEFILES) ? SHCONTF_NONFOLDERS : 0)
		| ((nFlags & STCF_INCLUDEHIDDEN) ? SHCONTF_INCLUDEHIDDEN : 0), m_hWnd);

	if (pEnumIDList.IsValid())
	{
		SetPopulationCount(0);

		CShellPidl pidl;
		while (NOERROR == pEnumIDList->Next(1, pidl.GetPointer(), NULL))
		{
			// add child item, inherit some flags (inclusion)
			InsertSubItem(hParent, pParentFolder, pidlParent, pidl,
				nFlags & STCF_INCLUDEMASK);

			// notify progress
			IncreasePopulation();
		}
	}

	if (GetPopulationCount() > 0)
	{
		// sort items
		TVSORTCB tvscb;
		tvscb.hParent = hParent;
		tvscb.lpfnCompare = CompareFunc;
		// tvscb.lParam = 0;	// not meaningful yet
		SortChildrenCB(&tvscb);
	}

	// notify progress
	SetPopulationCount(1,1);

	// success!
	return TRUE;
}

BOOL CShellTreeCtrl::GetItemContextMenu(HTREEITEM hItem, CShellContextMenu& rCtxMenu)
{
	TVITEMDATA* pData = (TVITEMDATA*)GetItemData(hItem);
	if (!pData->IsValid())
		return FALSE;

	return rCtxMenu.Create(pData->pParentFolder,
		pData->pidlAbs.GetLastChild());
}

void CShellTreeCtrl::SetCallbackMask(UINT nMask)
{
	m_nCallbackMask = nMask &
		(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN);
}

UINT CShellTreeCtrl::GetCallbackMask()
{
	return m_nCallbackMask;
}

//***************************************************************************************
BOOL CShellTreeCtrl::SelectPath (LPCITEMIDLIST lpidl)
{
	BOOL bRes = FALSE;

	ASSERT_VALID (this);
	ASSERT_VALID (g_pShellManager);

	if (lpidl == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	HTREEITEM htreeItem = GetRootItem ();

	SetRedraw (FALSE);

	if (g_pShellManager->GetItemCount (lpidl) == 0)
	{
		// Desktop
	}
	else
	{
		LPCITEMIDLIST lpidlCurr = lpidl;
		LPITEMIDLIST lpidlParent;

		CList<LPITEMIDLIST,LPITEMIDLIST> lstItems;
		lstItems.AddHead (g_pShellManager->CopyItem (lpidl));

		while (g_pShellManager->GetParentItem (lpidlCurr, lpidlParent) > 0)
		{
			lstItems.AddHead (lpidlParent);
			lpidlCurr = lpidlParent;
		}

		for (POSITION pos = lstItems.GetHeadPosition (); pos != NULL;)
		{
			LPITEMIDLIST lpidlCurr = lstItems.GetNext (pos);

			if (htreeItem != NULL)
			{
				if (GetChildItem (htreeItem) == NULL)
				{
					Expand (htreeItem, TVE_EXPAND);
				}

				BOOL bFound = FALSE;

				for (HTREEITEM hTreeChild = GetChildItem (htreeItem); 
					!bFound && hTreeChild != NULL;
					hTreeChild = GetNextSiblingItem (hTreeChild))
				{
					TVITEMDATA* pItem = (TVITEMDATA*) GetItemData (hTreeChild);
					if (pItem == NULL)
					{
						continue;
					}

					SHFILEINFO sfi1;
					SHFILEINFO sfi2;

					LPCITEMIDLIST lpidlAbs = pItem->pidlAbs;
					if (SHGetFileInfo ((LPCTSTR) lpidlAbs, 0, &sfi1, sizeof (sfi1), SHGFI_PIDL | SHGFI_DISPLAYNAME) &&
						SHGetFileInfo ((LPCTSTR) lpidlCurr, 0, &sfi2, sizeof (sfi2), SHGFI_PIDL | SHGFI_DISPLAYNAME) &&
						lstrcmp (sfi1.szDisplayName, sfi2.szDisplayName) == 0)
					{
						bFound = TRUE;
						htreeItem = hTreeChild;
					}
				}

				if (!bFound)
				{
					htreeItem = NULL;
				}
			}

			g_pShellManager->FreeItem (lpidlCurr);
		}
	}

	if (htreeItem != NULL)
	{
		m_bNoNotify = TRUE;
		Select (htreeItem, TVGN_CARET);
		if (GetChildItem (htreeItem) == NULL)
		{
			Expand (htreeItem, TVE_EXPAND);
		}
		
		//EnsureVisible (htreeItem);
		m_bNoNotify = FALSE;
		bRes = TRUE;
	}

	SetRedraw ();
	RedrawWindow ();

	return bRes;
}
