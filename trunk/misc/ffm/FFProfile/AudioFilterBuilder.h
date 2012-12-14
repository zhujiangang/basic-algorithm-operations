// AudioFilterBuilder.h: interface for the AudioFilterBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDIOFILTERBUILDER_H__8830F155_EF28_40BF_8599_69A196A52E22__INCLUDED_)
#define AFX_AUDIOFILTERBUILDER_H__8830F155_EF28_40BF_8599_69A196A52E22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBuilder.h"

class AudioFilterBuilder  
{
public:
	AudioFilterBuilder();
	virtual ~AudioFilterBuilder();
	virtual bool Build(std::string& szCmdLine, PropMap* pPropMap);
private:
	bool Build(std::string& szCmdLine, const CmdParam* pParams, int n, PropMap* pPropMap);
	bool BuildCmdOption(std::string& option, const CmdParam* pParam, PropMap* pPropMap);
};

#endif // !defined(AFX_AUDIOFILTERBUILDER_H__8830F155_EF28_40BF_8599_69A196A52E22__INCLUDED_)
