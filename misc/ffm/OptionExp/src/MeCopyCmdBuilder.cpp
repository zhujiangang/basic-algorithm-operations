#include "StdAfx.h"
#include "MeCopyCmdBuilder.h"
#include "OptionExpDef.h"
#include "log4cplus_config.h"
#include "OptionExpUtils.h"

MeCopyCmdBuilder::MeCopyCmdBuilder()
{
	Reset();
}
void MeCopyCmdBuilder::Reset()
{
	m_nCopyTrack = -1;
	m_szBinFile.erase();
	m_szInput.erase();
	m_szOutputFolder.erase();
	m_pContext = NULL;
}

MeCopyCmdBuilder& MeCopyCmdBuilder::SetCopyTrack(int nTrack)
{
	m_nCopyTrack = nTrack;
	return *this;
}
MeCopyCmdBuilder& MeCopyCmdBuilder::SetBinFile(const TCHAR* szBinFile)
{
	m_szBinFile.assign(szBinFile);
	return *this;
}
MeCopyCmdBuilder& MeCopyCmdBuilder::SetInput(const TCHAR* szInput)
{
	m_szInput = szInput;
	return *this;
}
MeCopyCmdBuilder& MeCopyCmdBuilder::SetOutputFolder(const TCHAR* szOutputFolder)
{
	m_szOutputFolder.assign(szOutputFolder);
	return *this;
}
MeCopyCmdBuilder& MeCopyCmdBuilder::SetOptionContext(OptionContext* pContext)
{
	m_pContext = pContext;
	return *this;
}

bool MeCopyCmdBuilder::Build(CmdInfo& cmdInfo, StrObjPtrContext& context)
{
	bool bRet = false;
	
	cmdInfo.m_szBinFQPath = m_szBinFile;
	cmdInfo.m_szInputFQPath = m_szInput;
	if(m_nCopyTrack == VIDEO)
	{
		std::string val, outFileExt, outFileFQPath;
		bRet = m_pContext->Get(OVC, val);
		if(!bRet)
		{
			return false;
		}
		
		if(val.compare("x264") == 0)
		{
			outFileExt = ".264";
		}
		else if( (val.compare("xvid") == 0) || (val.compare("mpeg4") == 0) )
		{
			outFileExt = ".m4v";
		}
		else
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("Unknown Output Video Codec: %s"), CFL_STRING_TO_T_STR(val));
			LOG4CPLUS_WARN_STR(THE_LIB_LOGGER, szLog)
				return false;
		}
		
		OptionExpUtils::BuildFullFilePath(outFileFQPath, CFL_TSTRING_TO_C_STR(m_szOutputFolder), outFileExt.c_str(), 
			CFL_TSTRING_TO_C_STR(m_szInput), false);
		cmdInfo.m_szOutputFQPath = CFL_STRING_TO_TSTRING(outFileFQPath);
		
		cfl::tformat(cmdInfo.m_szCmdLine, _T("\"%s\" -ovc copy -nosound -of rawvideo \"%s\" -o \"%s\""),
			m_szBinFile.c_str(), m_szInput.c_str(), cmdInfo.m_szOutputFQPath.c_str());
		
		bRet = true;
	}
	else if(m_nCopyTrack == AUDIO)
	{
		std::string val, outFileExt, outFileFQPath;
		bRet = m_pContext->Get(OAC, val);
		if(!bRet)
		{
			return false;
		}
		
		if((val.compare("aac") == 0) || (val.compare("faac") == 0))
		{
			outFileExt = ".aac";
		}
		else if(val.compare("mp3") == 0)
		{
			outFileExt = ".mp3";
		}
		else if(val.compare("ac3") == 0)
		{
			outFileExt = ".ac3";
		}
		else
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("Unknown Output Audio Codec: %s"), CFL_STRING_TO_T_STR(val));
			LOG4CPLUS_WARN_STR(THE_LIB_LOGGER, szLog)
				return false;
		}
		
		OptionExpUtils::BuildFullFilePath(outFileFQPath, CFL_TSTRING_TO_C_STR(m_szOutputFolder), outFileExt.c_str(), 
			CFL_TSTRING_TO_C_STR(m_szInput), false);
		cmdInfo.m_szOutputFQPath = CFL_STRING_TO_TSTRING(outFileFQPath);
		
		cfl::tformat(cmdInfo.m_szCmdLine, _T("\"%s\" -ovc frameno -oac copy -of rawaudio \"%s\" -o \"%s\""),
			m_szBinFile.c_str(), m_szInput.c_str(), cmdInfo.m_szOutputFQPath.c_str());
		
		bRet = true;
	}
	return bRet;
}
/*
bool MeCopyCmdBuilder::Build(std::vector<CmdInfo>& commands, std::vector<cfl::tstring>& delFiles)
{
	bool bRet = false;

	CmdInfo cmdInfo;
	cmdInfo.m_szBinFQPath = m_szBinFile;
	cmdInfo.m_szInputFQPath = m_szInput;
	if(m_nCopyTrack == VIDEO)
	{
		std::string val, outFileExt, outFileFQPath;
		bRet = m_pContext->Get(OVC, val);
		if(!bRet)
		{
			return false;
		}

		if(val.compare("x264") == 0)
		{
			outFileExt = ".264";
		}
		else if( (val.compare("xvid") == 0) || (val.compare("mpeg4") == 0) )
		{
			outFileExt = ".m4v";
		}
		else
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("Unknown Output Video Codec: %s"), CFL_STRING_TO_T_STR(val));
			LOG4CPLUS_WARN_STR(THE_LIB_LOGGER, szLog)
			return false;
		}

		OptionExpUtils::BuildFullFilePath(outFileFQPath, CFL_TSTRING_TO_C_STR(m_szOutputFolder), outFileExt.c_str(), 
			CFL_TSTRING_TO_C_STR(m_szInput), false);
		cmdInfo.m_szOutputFQPath = CFL_STRING_TO_TSTRING(outFileFQPath);

		cfl::tformat(cmdInfo.m_szCmdLine, _T("\"%s\" -ovc copy -nosound -of rawvideo \"%s\" -o \"%s\""),
			m_szBinFile.c_str(), m_szInput.c_str(), cmdInfo.m_szOutputFQPath.c_str());

		commands.push_back(cmdInfo);
		bRet = true;
	}
	else if(m_nCopyTrack == AUDIO)
	{
		std::string val, outFileExt, outFileFQPath;
		bRet = m_pContext->Get(OAC, val);
		if(!bRet)
		{
			return false;
		}
		
		if((val.compare("aac") == 0) || (val.compare("faac") == 0))
		{
			outFileExt = ".aac";
		}
		else if(val.compare("mp3") == 0)
		{
			outFileExt = ".mp3";
		}
		else if(val.compare("ac3") == 0)
		{
			outFileExt = ".ac3";
		}
		else
		{
			cfl::tstring szLog;
			cfl::tformat(szLog, _T("Unknown Output Audio Codec: %s"), CFL_STRING_TO_T_STR(val));
			LOG4CPLUS_WARN_STR(THE_LIB_LOGGER, szLog)
			return false;
		}
		
		OptionExpUtils::BuildFullFilePath(outFileFQPath, CFL_TSTRING_TO_C_STR(m_szOutputFolder), outFileExt.c_str(), 
			CFL_TSTRING_TO_C_STR(m_szInput), false);
		cmdInfo.m_szOutputFQPath = CFL_STRING_TO_TSTRING(outFileFQPath);
		
		cfl::tformat(cmdInfo.m_szCmdLine, _T("\"%s\" -ovc frameno -oac copy -of rawaudio \"%s\" -o \"%s\""),
			m_szBinFile.c_str(), m_szInput.c_str(), cmdInfo.m_szOutputFQPath.c_str());

		commands.push_back(cmdInfo);
		bRet = true;
	}
	return bRet;
}
*/
