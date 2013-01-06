#ifndef __CONTAINER_CMD_BUILDER_H__
#define __CONTAINER_CMD_BUILDER_H__

#include "CmdBuilder.h"
#include "OptionContext.h"

class ContainerCmdBuilder
{
public:
	ContainerCmdBuilder();
	//virtual bool Build(std::vector<CmdInfo>& commands, std::vector<cfl::tstring>& delFiles);
	virtual bool Build(std::vector<CmdInfo>& commands, StrObjPtrContext& context);

	virtual ContainerCmdBuilder& SetContainer(const TCHAR* szContainer);
	virtual ContainerCmdBuilder& SetMEncoderBin(const TCHAR* szMEncoder);
	virtual ContainerCmdBuilder& SetMP4CreatorBin(const TCHAR* szMP4Creator);
	virtual ContainerCmdBuilder& SetInput(const TCHAR* szInput);
	virtual ContainerCmdBuilder& SetOutputFolder(const TCHAR* szOutputFolder);
	virtual ContainerCmdBuilder& SetOptionContext(OptionContext* pContext);
	
protected:
	class CmdBuildInfo
	{
	public:
		CmdBuildInfo();
		~CmdBuildInfo();
	public:
		CmdBuilder* pBuilder;
		OptionContext* pMutableContext;
		bool bDelOutput;	
	};

	void Reset();
	void AddCmdBuildInfo(CmdBuilder* pBuilder, OptionContext* pMutableContext, bool bDelOutput);
	
	cfl::tstring m_szContainer;
	cfl::tstring m_szMEncoder;
	cfl::tstring m_szMP4Creator;
	cfl::tstring m_szInput;
	cfl::tstring m_szOutputFolder;
	OptionContext* m_pContext;

	std::vector<CmdBuildInfo*> m_pCmdBuildInfos;
};

#endif