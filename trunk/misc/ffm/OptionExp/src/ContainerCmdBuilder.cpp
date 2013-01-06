#include "StdAfx.h"
#include "ContainerCmdBuilder.h"
#include "OptionExpDef.h"
#include "log4cplus_config.h"
#include "OptionExpUtils.h"
#include "MeCmdBuilder.h"
#include "MeCopyCmdBuilder.h"
#include "MP4CmdBuilder.h"
#include "OptionContext.h"

ContainerCmdBuilder::CmdBuildInfo::CmdBuildInfo()
{
	pBuilder = NULL;
	pMutableContext = NULL;
	bDelOutput = false;
}

ContainerCmdBuilder::CmdBuildInfo::~CmdBuildInfo()
{
	if(pBuilder)
	{
		delete pBuilder;
		pBuilder = NULL;
	}
	if(pMutableContext)
	{
		delete pMutableContext;
		pMutableContext = NULL;
	}
}

void ContainerCmdBuilder::AddCmdBuildInfo(CmdBuilder* pBuilder, OptionContext* pMutableContext, bool bDelOutput)
{
	CmdBuildInfo* pInfo = new CmdBuildInfo;
	pInfo->pBuilder = pBuilder;
	pInfo->pMutableContext = pMutableContext;
	pInfo->bDelOutput = bDelOutput;
	m_pCmdBuildInfos.push_back(pInfo);
}

ContainerCmdBuilder::ContainerCmdBuilder()
{
	Reset();
}
void ContainerCmdBuilder::Reset()
{
	m_szContainer.erase();
	m_szMEncoder.erase();
	m_szMP4Creator.erase();
	m_szInput.erase();
	m_szOutputFolder.erase();
	m_pContext = NULL;

	for(int i = 0; i < m_pCmdBuildInfos.size(); i++)
	{
		CmdBuildInfo* pCmdBuildInfo = (CmdBuildInfo*)m_pCmdBuildInfos.at(i);
		delete pCmdBuildInfo;
	}
	m_pCmdBuildInfos.clear();
}

ContainerCmdBuilder& ContainerCmdBuilder::SetContainer(const TCHAR* szContainer)
{
	m_szContainer = szContainer;
	return *this;
}
ContainerCmdBuilder& ContainerCmdBuilder::SetMEncoderBin(const TCHAR* szBinFile)
{
	m_szMEncoder = szBinFile;
	return *this;
}
ContainerCmdBuilder& ContainerCmdBuilder::SetMP4CreatorBin(const TCHAR* szBinFile)
{
	m_szMP4Creator = szBinFile;
	return *this;
}
ContainerCmdBuilder& ContainerCmdBuilder::SetInput(const TCHAR* szInput)
{
	m_szInput = szInput;
	return *this;
}
ContainerCmdBuilder& ContainerCmdBuilder::SetOutputFolder(const TCHAR* szOutputFolder)
{
	m_szOutputFolder.assign(szOutputFolder);
	return *this;
}
ContainerCmdBuilder& ContainerCmdBuilder::SetOptionContext(OptionContext* pContext)
{
	m_pContext = pContext;
	return *this;
}
bool ContainerCmdBuilder::Build(std::vector<CmdInfo>& commands, StrObjPtrContext& context)
{
	if(m_szContainer.compare(_T("mp4")) != 0)
	{
		return false;
	}

	//Find the del list
	TStrVectorObj* pStrVecObj = NULL;
	if(!context.Get(PARAM_DEL_LIST, (cfl::Object**)&pStrVecObj))
	{
		pStrVecObj = new TStrVectorObj();
		context.Put(PARAM_DEL_LIST, pStrVecObj);
	}
	ASSERT(pStrVecObj != NULL);
	TStrVector* pDelList = (TStrVector*)pStrVecObj->GetData();
	ASSERT(pDelList != NULL);

	int passNumber[2];
	int nPassCount = 0, i;
	std::string val;
	//MEncoder Command
	if(m_pContext->Get(PASS_COUNT, val) && val.compare("2") == 0)
	{
		nPassCount = 2;
		passNumber[0] = 1;
		passNumber[1] = 2;
	}
	else
	{
		nPassCount = 1;
		passNumber[0] = -1;
	}

	OptionContext mutableCxt;
	CmdInfo cmdInfo;
	for(i = 0; i < nPassCount; i++)
	{
		MeCmdBuilder builder;
		builder.SetBinFile(m_szMEncoder.c_str()).SetInput(m_szInput.c_str());
		builder.SetOutputFolder(m_szOutputFolder.c_str());
		
		mutableCxt = *m_pContext;
		builder.SetPass(passNumber[i]).SetOptionContext(&mutableCxt);

		cmdInfo.Clear();
		if(!builder.Build(cmdInfo, context))
		{
			return false;
		}

		if(!cmdInfo.m_szOutputFQPath.empty())
		{
			pDelList->push_back(cmdInfo.m_szOutputFQPath);
		}

		commands.push_back(cmdInfo);
	}

	//last output file is the avi file
	cfl::tstring szAviFile = cmdInfo.m_szOutputFQPath;

	std::string mp4file;
	OptionExpUtils::BuildFullFilePath(mp4file, CFL_TSTRING_TO_C_STR(m_szOutputFolder), ".mp4", 
		CFL_TSTRING_TO_C_STR(m_szInput), false);

	//MeCopyCmdBuilder
	for(i = VIDEO; i <= AUDIO; i++)
	{
		MeCopyCmdBuilder builder;
		builder.SetCopyTrack(i).SetBinFile(m_szMEncoder.c_str()).SetInput(szAviFile.c_str());

		mutableCxt = *m_pContext;
		builder.SetOptionContext(&mutableCxt).SetOutputFolder(m_szOutputFolder.c_str());

		cmdInfo.Clear();
		if(!builder.Build(cmdInfo, context))
		{
			return false;
		}

		pDelList->push_back(cmdInfo.m_szOutputFQPath);
		commands.push_back(cmdInfo);

		mutableCxt = *m_pContext;
		MP4CmdBuilder mp4Builder;
		mp4Builder.SetTrack(i).SetBinFile(m_szMP4Creator.c_str()).SetInput(cmdInfo.m_szOutputFQPath.c_str());
		mp4Builder.SetOutput(CFL_STRING_TO_T_STR(mp4file)).SetOptionContext(&mutableCxt);

		cmdInfo.Clear();
		if(!mp4Builder.Build(cmdInfo, context))
		{
			return false;
		}

		commands.push_back(cmdInfo);
	}

	return true;
}
/*
bool ContainerCmdBuilder::Build(std::vector<CmdInfo>& commands, std::vector<cfl::tstring>& delFiles)
{
	if(m_szContainer.compare(_T("mp4")) != 0)
	{
		return false;
	}

	bool bRet = false;
	std::string val;

	//OptionContext* pMutableContext = NULL;

	int passNumber[2];
	int nPassCount = 0, i;
	//MEncoder Command
	if(m_pContext->Get(PASS_COUNT, val) && val.compare("2") == 0)
	{
		nPassCount = 2;
		passNumber[0] = 1;
		passNumber[1] = 2;
	}
	else
	{
		nPassCount = 1;
		passNumber[0] = -1;
	}

	std::vector<CmdInfo> tmpCmdList;
	std::vector<CmdInfo>::iterator tmpCmdListIter;
	std::vector<cfl::tstring> tmpDelList;
	cfl::tstring szLastOutput, szAviFile;
	OptionContext mutableCxt;
	for(i = 0; i < nPassCount; i++)
	{
		MeCmdBuilder builder;
		builder.SetBinFile(m_szMEncoder.c_str()).SetInput(m_szInput.c_str());
		builder.SetOutputFolder(m_szOutputFolder.c_str());
		
		mutableCxt = *m_pContext;
		builder.SetPass(passNumber[i]).SetOptionContext(&mutableCxt);
		
		//AddCmdBuildInfo(pMeBuilder, pMutableContext, true);

		tmpCmdList.clear();
		tmpDelList.clear();
		if(!builder.Build(tmpCmdList, tmpDelList))
		{
			return false;
		}

		commands.insert(commands.end(), tmpCmdList.begin(), tmpCmdList.end());
		delFiles.insert(delFiles.end(), tmpDelList.begin(), tmpDelList.end());

		//all the output file should be deleted
		for(tmpCmdListIter = tmpCmdList.begin(); tmpCmdListIter != tmpCmdList.end(); tmpCmdListIter++)
		{
			if(!tmpCmdListIter->m_szOutputFQPath.empty())
			{
				delFiles.push_back(tmpCmdListIter->m_szOutputFQPath);
			}
		}

		//find the last output
		if((i == nPassCount - 1) && !tmpCmdList.empty())
		{
			szLastOutput = tmpCmdList.at(tmpCmdList.size() - 1).m_szOutputFQPath;
		}
	}

	szAviFile = szLastOutput;

	std::string mp4file;
	OptionExpUtils::BuildFullFilePath(mp4file, CFL_TSTRING_TO_C_STR(m_szOutputFolder), ".mp4", 
		CFL_TSTRING_TO_C_STR(szLastOutput), false);

	//MeCopyCmdBuilder
	for(i = VIDEO; i <= AUDIO; i++)
	{
		MeCopyCmdBuilder builder;
		builder.SetCopyTrack(i).SetBinFile(m_szMEncoder.c_str()).SetInput(szAviFile.c_str());

		mutableCxt = *m_pContext;
		builder.SetOptionContext(&mutableCxt).SetOutputFolder(m_szOutputFolder.c_str());

		tmpCmdList.clear();
		tmpDelList.clear();
		if(!builder.Build(tmpCmdList, tmpDelList))
		{
			return false;
		}

		commands.insert(commands.end(), tmpCmdList.begin(), tmpCmdList.end());
		delFiles.insert(delFiles.end(), tmpDelList.begin(), tmpDelList.end());

		//all the output file should be deleted
		for(tmpCmdListIter = tmpCmdList.begin(); tmpCmdListIter != tmpCmdList.end(); tmpCmdListIter++)
		{
			if(!tmpCmdListIter->m_szOutputFQPath.empty())
			{
				delFiles.push_back(tmpCmdListIter->m_szOutputFQPath);
			}
		}
		//find the last output
		if(!tmpCmdList.empty())
		{
			szLastOutput = tmpCmdList.at(tmpCmdList.size() - 1).m_szOutputFQPath;
		}

		mutableCxt = *m_pContext;
		MP4CmdBuilder mp4Builder;
		mp4Builder.SetTrack(i).SetBinFile(m_szMP4Creator.c_str()).SetInput(szLastOutput.c_str());
		mp4Builder.SetOutput(CFL_STRING_TO_T_STR(mp4file)).SetOptionContext(&mutableCxt);

		tmpCmdList.clear();
		tmpDelList.clear();
		if(!mp4Builder.Build(tmpCmdList, tmpDelList))
		{
			return false;
		}
		
		commands.insert(commands.end(), tmpCmdList.begin(), tmpCmdList.end());
		delFiles.insert(delFiles.end(), tmpDelList.begin(), tmpDelList.end());
	}

	return true;
}
*/