#ifndef __ME_COPY_CMD_BUILDER_H__
#define __ME_COPY_CMD_BUILDER_H__

#pragma once

#include "CmdBuilder.h"
#include "OptionContext.h"

class MeCopyCmdBuilder : public CmdBuilder
{
public:
	MeCopyCmdBuilder();
	//virtual bool Build(std::vector<CmdInfo>& commands, std::vector<cfl::tstring>& delFiles);
	virtual bool Build(CmdInfo& cmdInfo, StrObjPtrContext& context);
	virtual MeCopyCmdBuilder& SetCopyTrack(int nTrack);
	virtual MeCopyCmdBuilder& SetBinFile(const TCHAR* szBinFile);
	virtual MeCopyCmdBuilder& SetInput(const TCHAR* szInput);
	virtual MeCopyCmdBuilder& SetOutputFolder(const TCHAR* szOutputFolder);
	virtual MeCopyCmdBuilder& SetOptionContext(OptionContext* pContext);
	
protected:
	void Reset();
	
	int			 m_nCopyTrack;
	cfl::tstring m_szBinFile;
	cfl::tstring m_szInput;
	cfl::tstring m_szOutputFolder;
	OptionContext* m_pContext;
};

#endif