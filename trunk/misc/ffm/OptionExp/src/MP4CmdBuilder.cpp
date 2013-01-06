#include "StdAfx.h"
#include "MP4CmdBuilder.h"
#include "OptionExpDef.h"
#include "log4cplus_config.h"
#include "OptionExpUtils.h"

MP4CmdBuilder::MP4CmdBuilder()
{
	Reset();
}
void MP4CmdBuilder::Reset()
{
	m_nTrack = -1;
	m_szBinFile.erase();
	m_szInput.erase();
	m_szOutput.erase();
	m_pContext = NULL;
}

MP4CmdBuilder& MP4CmdBuilder::SetTrack(int nTrack)
{
	m_nTrack = nTrack;
	return *this;
}
MP4CmdBuilder& MP4CmdBuilder::SetBinFile(const TCHAR* szBinFile)
{
	m_szBinFile.assign(szBinFile);
	return *this;
}
MP4CmdBuilder& MP4CmdBuilder::SetInput(const TCHAR* szInput)
{
	m_szInput = szInput;
	return *this;
}
MP4CmdBuilder& MP4CmdBuilder::SetOutput(const TCHAR* szOutputFolder)
{
	m_szOutput.assign(szOutputFolder);
	return *this;
}
MP4CmdBuilder& MP4CmdBuilder::SetOptionContext(OptionContext* pContext)
{
	m_pContext = pContext;
	return *this;
}

bool MP4CmdBuilder::Build(CmdInfo& cmdInfo, StrObjPtrContext& context)
{
	bool bRet = false;
	
	cmdInfo.m_szBinFQPath = m_szBinFile;
	cmdInfo.m_szInputFQPath = m_szInput;
	cmdInfo.m_szOutputFQPath = m_szOutput;
	if(m_nTrack == VIDEO)
	{
		std::string val, outFileExt, outFileFQPath;
		bRet = m_pContext->Get(OFPS, val);
		if(!bRet)
		{
			return false;
		}
		
		cfl::tformat(cmdInfo.m_szCmdLine, _T("\"%s\" -c \"%s\" -rate=%s \"%s\""),
			m_szBinFile.c_str(), m_szInput.c_str(), CFL_STRING_TO_T_STR(val), m_szOutput.c_str());
		
		bRet = true;
	}
	else if(m_nTrack == AUDIO)
	{
		cfl::tformat(cmdInfo.m_szCmdLine, _T("\"%s\" -c \"%s\" \"%s\""),
			m_szBinFile.c_str(), m_szInput.c_str(), m_szOutput.c_str());	
		bRet = true;
	}
	return bRet;
}
/*
bool MP4CmdBuilder::Build(std::vector<CmdInfo>& commands, std::vector<cfl::tstring>& delFiles)
{
	bool bRet = false;

	CmdInfo cmdInfo;
	cmdInfo.m_szBinFQPath = m_szBinFile;
	cmdInfo.m_szInputFQPath = m_szInput;
	cmdInfo.m_szOutputFQPath = m_szOutput;
	if(m_nTrack == VIDEO)
	{
		std::string val, outFileExt, outFileFQPath;
		bRet = m_pContext->Get(OFPS, val);
		if(!bRet)
		{
			return false;
		}

		cfl::tformat(cmdInfo.m_szCmdLine, _T("\"%s\" -c \"%s\" -rate=%s \"%s\""),
			m_szBinFile.c_str(), m_szInput.c_str(), CFL_STRING_TO_T_STR(val), m_szOutput.c_str());

		commands.push_back(cmdInfo);
		bRet = true;
	}
	else if(m_nTrack == AUDIO)
	{
		cfl::tformat(cmdInfo.m_szCmdLine, _T("\"%s\" -c \"%s\" \"%s\""),
			m_szBinFile.c_str(), m_szInput.c_str(), m_szOutput.c_str());
		
		commands.push_back(cmdInfo);	
		bRet = true;
	}
	return bRet;
}
*/