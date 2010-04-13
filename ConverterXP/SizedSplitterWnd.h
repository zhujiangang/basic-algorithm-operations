#if !defined(AFX_SIZEDSPLITTERWND_H__AAB08AA4_AED1_4C41_A6FA_AB1369F734B7__INCLUDED_)
#define AFX_SIZEDSPLITTERWND_H__AAB08AA4_AED1_4C41_A6FA_AB1369F734B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SizedSplitterWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSizedSplitterWnd view

class CSizedSplitterWnd : public CSplitterWnd
{
public:
	CSizedSplitterWnd();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSizedSplitterWnd)
	virtual ~CSizedSplitterWnd();
// Attributes
public:

private:
	CSize minSize;
// Operations
public:
	void setMinSize(CSize size);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSizedSplitterWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSizedSplitterWnd)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIZEDSPLITTERWND_H__AAB08AA4_AED1_4C41_A6FA_AB1369F734B7__INCLUDED_)
