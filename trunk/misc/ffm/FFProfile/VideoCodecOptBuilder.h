// VideoCodecOptBuilder.h: interface for the VideoCodecOptBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEOCODECOPTBUILDER_H__7FA6A938_76FB_48C3_AE2F_C145CFC285F1__INCLUDED_)
#define AFX_VIDEOCODECOPTBUILDER_H__7FA6A938_76FB_48C3_AE2F_C145CFC285F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBuilder.h"

class VideoCodecOptBuilder  
{
public:
	VideoCodecOptBuilder();
	virtual ~VideoCodecOptBuilder();
	virtual bool Build(std::string& szCmdLine, PropMap* pPropMap, const char* vcodecName);
private:
	bool Build(std::string& szCmdLine, const CmdParam* pParams, int n, PropMap* pPropMap);
	bool BuildCmdOption(std::string& option, const CmdParam* pParam, PropMap* pPropMap);
};

#endif // !defined(AFX_VIDEOCODECOPTBUILDER_H__7FA6A938_76FB_48C3_AE2F_C145CFC285F1__INCLUDED_)
