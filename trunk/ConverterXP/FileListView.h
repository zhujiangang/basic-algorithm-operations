#if !defined(AFX_FILELISTVIEW_H__8B3DE8AD_FDAA_4929_BAA6_5977A1A1B211__INCLUDED_)
#define AFX_FILELISTVIEW_H__8B3DE8AD_FDAA_4929_BAA6_5977A1A1B211__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileListView view

class CFileListView : public CScrollView
{
protected:
	CFileListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFileListView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFileListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFileListView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnDirTreeViewSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private:
	CString m_curPath;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILELISTVIEW_H__8B3DE8AD_FDAA_4929_BAA6_5977A1A1B211__INCLUDED_)
