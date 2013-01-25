#include "StdAfx.h"
#include "MP4CmdListBuilder.h"
#include "OptionExpDef.h"
#include "log4cplus_config.h"
#include "OptionExpUtils.h"
#include "MeCmdBuilder.h"
#include "MeCopyCmdBuilder.h"
#include "MP4CmdBuilder.h"
#include "OptionContext.h"

MP4CmdListBuilder::MP4CmdListBuilder()
{
}
CmdListBuilder* MP4CmdListBuilder::Clone()
{
	return new MP4CmdListBuilder();
}
bool MP4CmdListBuilder::Build(std::vector<CmdInfo>& commands, StrObjPtrContext& context)
{
	//Find OptionContext Pointer
	OptionContext** pOptCxtPtrObj = GetObjPtrData<OptionContext*>(context, PARAM_OPT_CXT);
	
	//Find other parameters
	cfl::tstring* pBinMEncoder = GetObjPtrData<cfl::tstring>(context, PARAM_BIN_MENCODER);
	cfl::tstring* pBinMP4Creator = GetObjPtrData<cfl::tstring>(context, PARAM_BIN_MP4CREATOR);
	cfl::tstring* pInputFile = GetObjPtrData<cfl::tstring>(context, PARAM_INPUT_FILE);
	cfl::tstring* pOutputFolder = GetObjPtrData<cfl::tstring>(context, PARAM_OUTPUT_FOLDER);
	cfl::tstring* pOutputFileExt = GetObjPtrData<cfl::tstring>(context, PARAM_OUT_EXT);
	
	if(pOptCxtPtrObj == NULL || *pOptCxtPtrObj == NULL || pBinMEncoder == NULL || pBinMP4Creator == NULL 
		|| pInputFile == NULL || pOutputFolder == NULL || pOutputFileExt == NULL)
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("MeCmdListBuilder::Build failed."));
		LOG4CPLUS_WARN_STR(THE_LIB_LOGGER, szLog)
			return false;
	}

	//Find the del list
	TStrVector* pDelList = GetObjPtrData<TStrVector>(context, PARAM_DEL_LIST);

	int passNumber[2];
	int nPassCount = 0, i;
	std::string val;
	//MEncoder Command
	if((*pOptCxtPtrObj)->Get(PASS_COUNT, val) && val.compare("2") == 0)
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
		builder.SetBinFile(pBinMEncoder->c_str()).SetInput(pInputFile->c_str());
		builder.SetOutputFolder(pOutputFolder->c_str()).SetOutputFileExt(_T(".avi"));
		
		mutableCxt = **pOptCxtPtrObj;
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
	OptionExpUtils::BuildFullFilePath(mp4file, CFL_TSTRING_TO_C_STR(*pOutputFolder), 
		CFL_TSTRING_TO_C_STR(*pOutputFileExt), CFL_TSTRING_TO_C_STR(*pInputFile), false);

	//MeCopyCmdBuilder
	for(i = VIDEO; i <= AUDIO; i++)
	{
		MeCopyCmdBuilder builder;
		builder.SetCopyTrack(i).SetBinFile(pBinMEncoder->c_str()).SetInput(szAviFile.c_str());

		mutableCxt = **pOptCxtPtrObj;
		builder.SetOptionContext(&mutableCxt).SetOutputFolder(pOutputFolder->c_str());

		cmdInfo.Clear();
		if(!builder.Build(cmdInfo, context))
		{
			return false;
		}
		
		pDelList->push_back(cmdInfo.m_szOutputFQPath);
		cmdInfo.m_nWeight = 1;
		commands.push_back(cmdInfo);

		mutableCxt = **pOptCxtPtrObj;
		MP4CmdBuilder mp4Builder;
		mp4Builder.SetTrack(i).SetBinFile(pBinMP4Creator->c_str()).SetInput(cmdInfo.m_szOutputFQPath.c_str());
		mp4Builder.SetOutput(CFL_STRING_TO_T_STR(mp4file)).SetOptionContext(&mutableCxt);

		cmdInfo.Clear();
		if(!mp4Builder.Build(cmdInfo, context))
		{
			return false;
		}
		cmdInfo.m_nWeight = 1;
		commands.push_back(cmdInfo);
	}

	//flush weight
	OptionExpUtils::FlushCmdWeight(commands);

	return true;
}
