// VideoFilterBuilder.h: interface for the VideoFilterBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEOFILTERBUILDER_H__39324447_76B2_4430_967A_05EAACDE1D00__INCLUDED_)
#define AFX_VIDEOFILTERBUILDER_H__39324447_76B2_4430_967A_05EAACDE1D00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBuilder.h"

class VideoFilterBuilder  
{
public:
	VideoFilterBuilder();
	virtual ~VideoFilterBuilder();
	virtual bool Build(std::string& szCmdLine, PropMap* pPropMap);
private:
	bool Build(std::string& szCmdLine, const FilterOptsCmdParam* pParams, int n, PropMap* pPropMap);
	bool BuildCmdOption(std::string& option, const FilterOptsCmdParam* pParam, PropMap* pPropMap);
};

#endif // !defined(AFX_VIDEOFILTERBUILDER_H__39324447_76B2_4430_967A_05EAACDE1D00__INCLUDED_)
