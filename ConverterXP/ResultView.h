#if !defined(AFX_RESULTVIEW_H__4C3AFEB2_C0AE_4AED_9779_EBAFB8D129DE__INCLUDED_)
#define AFX_RESULTVIEW_H__4C3AFEB2_C0AE_4AED_9779_EBAFB8D129DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResultView view

class CResultView : public CScrollView
{
protected:
	CResultView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CResultView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CResultView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CResultView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTVIEW_H__4C3AFEB2_C0AE_4AED_9779_EBAFB8D129DE__INCLUDED_)
