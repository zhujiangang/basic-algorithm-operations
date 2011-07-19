// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__61FBF82C_EB11_4D0A_8EEA_9638710D664A__INCLUDED_)
#define AFX_STDAFX_H__61FBF82C_EB11_4D0A_8EEA_9638710D664A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning(disable : 4786)

#include <afx.h>
#include <afxwin.h>
#include <afxtempl.h>

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#ifdef _DEBUG
#ifndef ENABLE_LOG4CPLUS
#define ENABLE_LOG4CPLUS
#define SCC_LOGGER_NAME		"lib.SCC"
#endif
#else
#undef ENABLE_LOG4CPLUS
#define SCC_LOGGER_NAME
#endif

#include "log4cplus_config.h"
#define THE_LIB_LOGGER		GET_LOGGER(SCC_LOGGER_NAME)

#endif // !defined(AFX_STDAFX_H__61FBF82C_EB11_4D0A_8EEA_9638710D664A__INCLUDED_)
