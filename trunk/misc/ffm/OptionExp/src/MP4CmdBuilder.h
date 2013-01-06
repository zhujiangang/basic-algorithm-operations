#ifndef __MP4_CMD_BUILDER_H__
#define __MP4_CMD_BUILDER_H__

#include "CmdBuilder.h"
#include "OptionContext.h"

class MP4CmdBuilder : public CmdBuilder
{
public:
	MP4CmdBuilder();
	//virtual bool Build(std::vector<CmdInfo>& commands, std::vector<cfl::tstring>& delFiles);
	virtual bool Build(CmdInfo& cmdInfo, StrObjPtrContext& context);
	virtual MP4CmdBuilder& SetTrack(int nTrack);
	virtual MP4CmdBuilder& SetBinFile(const TCHAR* szBinFile);
	virtual MP4CmdBuilder& SetInput(const TCHAR* szInput);
	virtual MP4CmdBuilder& SetOutput(const TCHAR* szOutput);
	virtual MP4CmdBuilder& SetOptionContext(OptionContext* pContext);
	
protected:
	void Reset();
	
	int			 m_nTrack;
	cfl::tstring m_szBinFile;
	cfl::tstring m_szInput;
	cfl::tstring m_szOutput;
	OptionContext* m_pContext;
};

#endif