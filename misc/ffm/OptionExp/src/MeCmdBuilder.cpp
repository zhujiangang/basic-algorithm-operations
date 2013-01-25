#include "stdafx.h"
#include "MeCmdBuilder.h"
#include "DefaultOptionExpBuilder.h"
#include "OptionExpTree.h"
#include "OptionExpUtils.h"
#include "OptionExpDef.h"
#include "log4cplus_config.h"

MeCmdBuilder::MeCmdBuilder()
{
	Reset();
}
void MeCmdBuilder::Reset()
{
	m_nPass = 0;
	m_szBinFile.erase();
	m_szInput.erase();
	m_szOutputFolder.erase();
	m_szOutputFileExt.assign(_T(".avi"));
	m_pContext = NULL;
}

MeCmdBuilder& MeCmdBuilder::SetPass(int nPass)
{
	m_nPass = nPass;
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetBinFile(const TCHAR* szBinFile)
{
	m_szBinFile.assign(szBinFile);
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetInput(const TCHAR* szInput)
{
	m_szInput.assign(szInput);
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetOutputFolder(const TCHAR* szOutputFolder)
{
	m_szOutputFolder.assign(szOutputFolder);
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetOutputFileExt(const TCHAR* szFileExt)
{
	m_szOutputFileExt.assign(szFileExt);
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetOptionContext(OptionContext* pContext)
{
	m_pContext = pContext;
	return *this;
}

bool MeCmdBuilder::Build(CmdInfo& cmdInfo, StrObjPtrContext& context)
{
	//TODO: validation check
	if(m_pContext == NULL)
	{
		if(IS_LOG_ENABLED(THE_LIB_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("MeCmdBuilder::Build failed. m_pContext=0x%8X"), (int)m_pContext);
			LOG4CPLUS_DEBUG_STR(THE_LIB_LOGGER, szLog)
		}
		return false;
	}
	
	//0. Bin file
	cmdInfo.m_szBinFQPath = m_szBinFile;

	//1. Input file: add quota to input file
	cmdInfo.m_szInputFQPath = m_szInput;
	std::string szInFile;
	cfl::format(szInFile, "\"%s\"", CFL_TSTRING_TO_C_STR(m_szInput));
	m_pContext->Put(IFILE, szInFile.c_str());

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
	
	std::string str;
	if(m_nPass == 1)
	{
		m_pContext->Put(PASS, "1");
		
		OptionExpUtils::BuildFullFilePath(str, CFL_TSTRING_TO_C_STR(m_szOutputFolder), "divx2pass.log.mbtree", false);
		//intermediate files
		pDelList->push_back(CFL_STRING_TO_TSTRING(str));

		//passlog file
		OptionExpUtils::BuildFullFilePath(str, CFL_TSTRING_TO_C_STR(m_szOutputFolder), "divx2pass.log", false);
		//intermediate files
		pDelList->push_back(CFL_STRING_TO_TSTRING(str));
		
		//add quota
		cfl::format(str, "\"%s\"", str.c_str());
		m_pContext->Put(PASS_LOG_FILE, str.c_str());
		
		m_pContext->Put(OFILE, "NUL:");
		
		//no output file
		cmdInfo.m_szOutputFQPath.erase();
	}
	else if(m_nPass == 2)
	{
		m_pContext->Put(PASS, "2");

		OptionExpUtils::BuildFullFilePath(str, CFL_TSTRING_TO_C_STR(m_szOutputFolder), "divx2pass.log", false);
		
		//add quota
		cfl::format(str, "\"%s\"", str.c_str());
		m_pContext->Put(PASS_LOG_FILE, str.c_str());
		
		//output file
		OptionExpUtils::BuildFullFilePath(str, CFL_TSTRING_TO_C_STR(m_szOutputFolder), 
			CFL_TSTRING_TO_C_STR(m_szOutputFileExt), szInFile.c_str(), false);
		cmdInfo.m_szOutputFQPath = CFL_STRING_TO_TSTRING(str);
		
		//add quota
		cfl::format(str, "\"%s\"", str.c_str());
		m_pContext->Put(OFILE, str.c_str());
	}
	else
	{
		OptionExpUtils::BuildFullFilePath(str, CFL_TSTRING_TO_C_STR(m_szOutputFolder), 
			CFL_TSTRING_TO_C_STR(m_szOutputFileExt), szInFile.c_str(), false);
		cmdInfo.m_szOutputFQPath = CFL_STRING_TO_TSTRING(str);
		
		//add quota
		cfl::format(str, "\"%s\"", str.c_str());
		m_pContext->Put(OFILE, str.c_str());
	}
	
	OptionExpTree tree;
	DefaultOptionExpBuilder builder;
	if(!tree.Build(m_pContext, &builder))
	{
		return false;
	}
	std::string szOption;
	if(!tree.Evaluate(m_pContext, szOption))
	{
		return false;
	}
	
	cfl::tformat(cmdInfo.m_szCmdLine, _T("\"%s\" %s"), m_szBinFile.c_str(), CFL_STRING_TO_T_STR(szOption));
	
	return true;
}

/*
bool MeCmdBuilder::Build(std::vector<CmdInfo>& commands, std::vector<cfl::tstring>& delFiles)
{
	//TODO: validation check
	if(m_pContext == NULL)
	{
		if(IS_LOG_ENABLED(THE_LIB_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("MeCmdBuilder::Build failed. m_pContext=0x%8X, m_pBuilder=0x%8X"), 
				(int)m_pContext);
			LOG4CPLUS_DEBUG_STR(THE_LIB_LOGGER, szLog)
		}
		return false;
	}

	CmdInfo cmdInfo;

	//0. Bin file
	cmdInfo.m_szBinFQPath = m_szBinFile;

	//1. Input file: add quota to input file
	std::string szInFile;
	if(!m_pContext->Get(IFILE, szInFile))
	{
		return false;
	}
	cfl::format(szInFile, "\"%s\"", szInFile.c_str());
	m_pContext->Put(IFILE, szInFile.c_str());

	//set input file
	cmdInfo.m_szInputFQPath = CFL_STRING_TO_TSTRING(szInFile);

	std::string str;
	if(m_nPass == 1)
	{
		m_pContext->Put(PASS, "1");

		//passlog file
		OptionExpUtils::BuildFullFilePath(str, CFL_TSTRING_TO_C_STR(m_szOutputFolder), "divx2pass.log", false);
		
		//intermediate files
		delFiles.push_back(CFL_STRING_TO_TSTRING(str));

		//add quota
		cfl::format(str, "\"%s\"", str.c_str());
		m_pContext->Put(PASS_LOG_FILE, str.c_str());

		m_pContext->Put(OFILE, "NUL:");

		//no output file
		cmdInfo.m_szOutputFQPath.erase();
	}
	else if(m_nPass == 2)
	{
		m_pContext->Put(PASS, "2");
		
		OptionExpUtils::BuildFullFilePath(str, CFL_TSTRING_TO_C_STR(m_szOutputFolder), "divx2pass.log", false);

		//intermediate files
		delFiles.push_back(CFL_STRING_TO_TSTRING(str));

		//add quota
		cfl::format(str, "\"%s\"", str.c_str());
		m_pContext->Put(PASS_LOG_FILE, str.c_str());

		//output file
		OptionExpUtils::BuildFullFilePath(str, CFL_TSTRING_TO_C_STR(m_szOutputFolder), ".avi", szInFile.c_str(), false);
		cmdInfo.m_szOutputFQPath = CFL_STRING_TO_TSTRING(str);

		//add quota
		cfl::format(str, "\"%s\"", str.c_str());
		m_pContext->Put(OFILE, str.c_str());
	}
	else
	{
		OptionExpUtils::BuildFullFilePath(str, CFL_TSTRING_TO_C_STR(m_szOutputFolder), ".avi", szInFile.c_str(), false);
		cmdInfo.m_szOutputFQPath = CFL_STRING_TO_TSTRING(str);

		//add quota
		cfl::format(str, "\"%s\"", str.c_str());
		m_pContext->Put(OFILE, str.c_str());
	}

	OptionExpTree tree;
	DefaultOptionExpBuilder builder;
	if(!tree.Build(m_pContext, &builder))
	{
		return false;
	}
	std::string szOption;
	if(!tree.Evaluate(m_pContext, szOption))
	{
		return false;
	}

	cfl::tformat(cmdInfo.m_szCmdLine, _T("\"%s\" %s"), m_szBinFile.c_str(), CFL_STRING_TO_T_STR(szOption));
	commands.push_back(cmdInfo);

	return true;
}
*/