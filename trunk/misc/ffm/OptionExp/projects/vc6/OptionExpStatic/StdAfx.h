// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7EE367FF_9165_40B8_BCD8_E5E39392646A__INCLUDED_)
#define AFX_STDAFX_H__7EE367FF_9165_40B8_BCD8_E5E39392646A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>

// TODO: reference additional headers your program requires here
#ifndef ARRAY_LEN
#define ARRAY_LEN(x)	(sizeof(x)/sizeof(x[0]))
#endif

#define ENABLE_LOG4CPLUS

#define LIB_LOGGER_PREFIX	_T("lib.OptionExp")
#define THE_LIB_LOGGER		GET_LOGGER(LIB_LOGGER_PREFIX)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7EE367FF_9165_40B8_BCD8_E5E39392646A__INCLUDED_)
