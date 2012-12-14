// MEncoderCmdBuilder.h: interface for the MEncoderCmdBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENCODERCMDBUILDER_H__ED2D24F2_4625_4756_8B25_EF74CA712D28__INCLUDED_)
#define AFX_MENCODERCMDBUILDER_H__ED2D24F2_4625_4756_8B25_EF74CA712D28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBuilder.h"

class MEncoderCmdBuilder : public CmdBuilder
{
public:
	MEncoderCmdBuilder(PropMap* pPropMap);
	virtual ~MEncoderCmdBuilder();
	virtual bool Build(std::string& szCmdLine);

private:
	bool Build(std::string& szCmdLine, const CmdParam* pParams, int n);
	bool BuildCmdOption(std::string& option, const CmdParam* pParam);
protected:
	PropMap* m_pPropMap;
};

#endif // !defined(AFX_MENCODERCMDBUILDER_H__ED2D24F2_4625_4756_8B25_EF74CA712D28__INCLUDED_)
