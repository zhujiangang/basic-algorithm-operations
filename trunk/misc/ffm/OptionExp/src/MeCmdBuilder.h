#ifndef __ME_CMD_BUILDER_H__
#define __ME_CMD_BUILDER_H__

#pragma once

#include "CmdBuilder.h"
#include "OptionContext.h"

class MeCmdBuilder : public CmdBuilder
{
public:
	MeCmdBuilder();
	//virtual bool Build(std::vector<CmdInfo>& commands, std::vector<cfl::tstring>& delFiles);
	virtual bool Build(CmdInfo& cmdInfo, StrObjPtrContext& context);
	
	virtual MeCmdBuilder& SetPass(int nPass);
	virtual MeCmdBuilder& SetBinFile(const TCHAR* szBinFile);
	virtual MeCmdBuilder& SetInput(const TCHAR* szInput);
	virtual MeCmdBuilder& SetOutputFolder(const TCHAR* szOutputFolder);
	virtual MeCmdBuilder& SetOptionContext(OptionContext* pContext);
	
protected:
	void Reset();
	
	int	m_nPass;
	cfl::tstring m_szBinFile;
	cfl::tstring m_szInput;
	cfl::tstring m_szOutputFolder;
	OptionContext* m_pContext;
};

#endif