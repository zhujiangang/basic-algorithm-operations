// AudioCodecOptBuilder.h: interface for the AudioCodecOptBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDIOCODECOPTBUILDER_H__C76CF31F_2D4A_4541_8DCF_1C779419DEAE__INCLUDED_)
#define AFX_AUDIOCODECOPTBUILDER_H__C76CF31F_2D4A_4541_8DCF_1C779419DEAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBuilder.h"

class AudioCodecOptBuilder  
{
public:
	AudioCodecOptBuilder();
	virtual ~AudioCodecOptBuilder();
	virtual bool Build(std::string& szCmdLine, PropMap* pPropMap, const char* vcodecName);
private:
	bool Build(std::string& szCmdLine, const CmdParam* pParams, int n, PropMap* pPropMap);
	bool BuildCmdOption(std::string& option, const CmdParam* pParam, PropMap* pPropMap);
};

#endif // !defined(AFX_AUDIOCODECOPTBUILDER_H__C76CF31F_2D4A_4541_8DCF_1C779419DEAE__INCLUDED_)
