#include "stdafx.h"
#include "CmdBuilder.h"
#include "OptionExpTree.h"
#include "cflbase/tstring.h"
#include "SysUtils.h"
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
	m_szOutput.erase();
	m_szOutputFolder.erase();
	m_pContext = NULL;
	m_pBuilder = NULL;
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
MeCmdBuilder& MeCmdBuilder::SetOutput(const TCHAR* szOutput)
{
	m_szOutput.assign(szOutput);
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetOutputFolder(const char* szOutputFolder)
{
	m_szOutputFolder.assign(szOutputFolder);
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetOptionContext(OptionContext* pContext)
{
	m_pContext = pContext;
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetOptionExpBuilder(OptionExpBuilder* pOptExpBuilder)
{
	m_pBuilder = pOptExpBuilder;
	return *this;
}

bool MeCmdBuilder::Build(cfl::tstring& szCmdLine)
{
	if(m_pContext == NULL || m_pBuilder == NULL)
	{
		if(IS_LOG_ENABLED(THE_LIB_LOGGER, log4cplus::DEBUG_LOG_LEVEL))
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("MeCmdBuilder::Build failed. m_pContext=0x%8X, m_pBuilder=0x%8X"), 
				(int)m_pContext, (int)m_pBuilder);
			LOG4CPLUS_DEBUG_STR(THE_LIB_LOGGER, szLog)
		}
		return false;
	}

	std::string szInFile;
	if(!m_pContext->Get(IFILE, szInFile))
	{
		return false;
	}
	//add quota to input file
	cfl::format(szInFile, "\"%s\"", szInFile.c_str());
	m_pContext->Put(IFILE, szInFile.c_str());

	std::string str;
	if(m_nPass == 1)
	{
		m_pContext->Put(PASS, "1");

		SysUtils::BuildFullFilePath(str, m_szOutputFolder.c_str(), "divx2pass.log", true);
		m_pContext->Put(PASS_LOG_FILE, str.c_str());

		m_pContext->Put(OFILE, "NUL:");
	}
	else if(m_nPass == 2)
	{
		m_pContext->Put(PASS, "2");
		
		SysUtils::BuildFullFilePath(str, m_szOutputFolder.c_str(), "divx2pass.log", true);
		m_pContext->Put(PASS_LOG_FILE, str.c_str());

		SysUtils::BuildFullFilePath(str, m_szOutputFolder.c_str(), ".avi", szInFile.c_str(), true);
		m_pContext->Put(OFILE, str.c_str());
	}
	else
	{
		SysUtils::BuildFullFilePath(str, m_szOutputFolder.c_str(), ".avi", szInFile.c_str(), true);
		m_pContext->Put(OFILE, str.c_str());
	}

	OptionExpTree tree;
	if(!tree.Build(m_pContext, m_pBuilder))
	{
		return false;
	}
	std::string szOption;
	if(!tree.Evaluate(m_pContext, szOption))
	{
		return false;
	}

	cfl::tformat(szCmdLine, _T("\"%s\" %s"), m_szBinFile.c_str(), CFL_STRING_TO_T_STR(szOption));

	return true;
}