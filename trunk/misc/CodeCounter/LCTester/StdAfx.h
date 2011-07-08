// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__48634E98_C5EF_424B_946C_08B5747242B2__INCLUDED_)
#define AFX_STDAFX_H__48634E98_C5EF_424B_946C_08B5747242B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#include <afxtempl.h>

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#ifndef ENABLE_LOG4CPLUS
#define ENABLE_LOG4CPLUS
#endif

#include "log4cplus_config.h"

#endif // !defined(AFX_STDAFX_H__48634E98_C5EF_424B_946C_08B5747242B2__INCLUDED_)
