#include "StdAfx.h"
#include "MeCmdListBuilder.h"
#include "OptionExpDef.h"
#include "log4cplus_config.h"
#include "OptionExpUtils.h"
#include "MeCmdBuilder.h"

MeCmdListBuilder::MeCmdListBuilder()
{
}

CmdListBuilder* MeCmdListBuilder::Clone()
{
	return new MeCmdListBuilder();
}

bool MeCmdListBuilder::Build(std::vector<CmdInfo>& commands, StrObjPtrContext& context)
{
	//Find OptionContext Pointer
	OptionContext** pOptCxtPtrObj = GetObjPtrData<OptionContext*>(context, PARAM_OPT_CXT);
	
	//Find other parameters
	cfl::tstring* pBinMEncoder = GetObjPtrData<cfl::tstring>(context, PARAM_BIN_MENCODER);
	cfl::tstring* pInputFile = GetObjPtrData<cfl::tstring>(context, PARAM_INPUT_FILE);
	cfl::tstring* pOutputFolder = GetObjPtrData<cfl::tstring>(context, PARAM_OUTPUT_FOLDER);
	cfl::tstring* pOutputFileExt = GetObjPtrData<cfl::tstring>(context, PARAM_OUT_EXT);
	cfl::tstring* pOutputFormat = GetObjPtrData<cfl::tstring>(context, PARAM_OF);

	if(pOptCxtPtrObj == NULL || *pOptCxtPtrObj == NULL || pBinMEncoder == NULL || pInputFile == NULL
		|| pOutputFolder == NULL || pOutputFileExt == NULL)
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("MeCmdListBuilder::Build failed."));
		LOG4CPLUS_WARN_STR(THE_LIB_LOGGER, szLog)
		return false;
	}
	
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
	for(i = 0; i < nPassCount; i++)
	{
		MeCmdBuilder builder;
		builder.SetBinFile(pBinMEncoder->c_str()).SetInput(pInputFile->c_str());
		builder.SetOutputFolder(pOutputFolder->c_str()).SetOutputFileExt(pOutputFileExt->c_str());
		
		mutableCxt = **pOptCxtPtrObj;
		if(pOutputFormat != NULL)
		{
			mutableCxt.Put(FORCE_OF, CFL_TSTRING_TO_C_STR(*pOutputFormat));
		}
		builder.SetPass(passNumber[i]).SetOptionContext(&mutableCxt);

		CmdInfo cmdInfo;
		if(!builder.Build(cmdInfo, context))
		{
			return false;
		}

		commands.push_back(cmdInfo);
	}

	OptionExpUtils::FlushCmdWeight(commands);

	return true;
}
