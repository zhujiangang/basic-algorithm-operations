// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0AA0619F_36B9_45D7_958F_C93CD7AC33A1__INCLUDED_)
#define AFX_STDAFX_H__0AA0619F_36B9_45D7_958F_C93CD7AC33A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define ARRAY_LEN(x)	sizeof(x)/sizeof(x[0])

#ifndef PATH_SEP
#define PATH_SEP	"\\"
#endif

#ifndef ARG_SEP
#define ARG_SEP		" "
#endif

//-lavcopts vcodec=mpeg4:mbd=2:trell:autoaspect:vbitrate=512:vhq
#ifndef PROP_SEP
#define PROP_SEP	"="
#endif

//-lavcopts vcodec=mpeg4:mbd=2:trell:autoaspect:vbitrate=512:vhq
#ifndef OPTION_SEP
#define OPTION_SEP	":"
#endif

//-vf-add scale=480:272,harddup
#ifndef	FILTER_SEP
#define FILTER_SEP	","
#endif

#define SEEK_TIME			"SeekTime"
#define END_TIME			"EndTime"
#define MAX_AV_SYNC			"MaxAVSync"
#define NO_SKIP				"NoSkip"
#define SWS					"SWS"
#define AUDIO_FILTER		"AudioFilter"
#define VIDEO_FILTER		"VideoFilter"
#define VIDEO_SIZE			"Video Size"
#define VIDEO_WIDTH			"VideoWidth"
#define VIDEO_HEIGHT		"VideoHeight"
#define OSRATE				"OutputSampleRate"
#define OFPS				"OFPS"
#define OAC					"OAC"
#define OAC_OPTS			"__OACOpts__"
#define OVC					"OVC"
#define OVC_OPTS			"__OVCOpts__"
#define FOURCC				"FourCC"

#define FORCE_OF			"ForceOutputFormat"
#define OF					"OutputFormat"
#define OF_OPTS				"__OFOpts__"

#define IFILE				"InputFile"
#define OFILE				"OutputFile"

#define VIDEO_BITRATE		"VideoBitrate"
#define AUDIO_BITRATE		"AudioBitrate"
#define MAX_B_FRAMES		"MaxBFrames"

#define ERROR_STR			"__ErrorStr__"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0AA0619F_36B9_45D7_958F_C93CD7AC33A1__INCLUDED_)
