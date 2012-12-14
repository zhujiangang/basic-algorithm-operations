// MEncoderCmdBuilder.cpp: implementation of the MEncoderCmdBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MEncoderCmdBuilder.h"
#include "VideoCodecOptBuilder.h"
#include "AudioCodecOptBuilder.h"
#include "VideoFilterBuilder.h"
#include "AudioFilterBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static const CmdParam ipodParams[] = 
{
	{"-ss",			COF_OPTIONAL,		SEEK_TIME,		NULL		},
	{"-endpos",		COF_OPTIONAL,		END_TIME,		NULL		},
	{"-mc",			COF_DEFAULT,		MAX_AV_SYNC,	"0"			},
	{"-noskip",		COF_FLAG_TYPE,		NO_SKIP,		NULL		},
	{"-sws",		COF_OPTIONAL,		SWS,			NULL		},

// 	{"-of",			COF_OPTIONAL,		OUT_FORMAT,		NULL		},
// 	{"",			COF_SUB_SET,		OF_OPTS,		NULL		},
	
	{"-af",			COF_DEFAULT,		AUDIO_FILTER,	"volnorm"	},
	{"-vf",			COF_DEFAULT,		VIDEO_FILTER,	"harddup"	},

	{"-srate",		COF_DEFAULT,		OSRATE,			NULL		},
	{"-ofps",		COF_DEFAULT,		OFPS,			NULL		},

	{"-oac",		COF_DEFAULT,		OAC,			NULL		},
	//TODO: ac options?
	{NULL,			COF_SUB_SET,		OAC_OPTS,		NULL		},

	{"-ovc",		COF_DEFAULT,		OVC,			NULL		},
	{"-ffourcc",	COF_OPTIONAL,		FOURCC,			NULL		},
	//TODO: vc options?
	{NULL,			COF_SUB_SET,		OVC_OPTS,		NULL		},

	{"-o",			COF_DEFAULT,		OFILE,			NULL		}
};

MEncoderCmdBuilder::MEncoderCmdBuilder(PropMap* pPropMap)
{
	m_pPropMap = pPropMap;
}

MEncoderCmdBuilder::~MEncoderCmdBuilder()
{

}

bool MEncoderCmdBuilder::Build(std::string& szCmdLine)
{
	return Build(szCmdLine, ipodParams, sizeof(ipodParams)/sizeof(ipodParams[0]));
}

bool MEncoderCmdBuilder::Build(std::string& szCmdLine, const CmdParam* pParams, int n)
{
	bool bRet;
	std::string val;

	{
		AudioFilterBuilder afb;
		bRet = afb.Build(szCmdLine, m_pPropMap);
		if(!bRet)
		{
			return false;
		}
		m_pPropMap->PutProp(AUDIO_FILTER, szCmdLine.c_str());
	}

	{
		VideoFilterBuilder vfb;
		bRet = vfb.Build(szCmdLine, m_pPropMap);
		if(!bRet)
		{
			return false;
		}
		m_pPropMap->PutProp(VIDEO_FILTER, szCmdLine.c_str());
	}



	bRet = m_pPropMap->GetProp(OVC, val);
	if(bRet)
	{
		VideoCodecOptBuilder vcob;
		
		bRet = vcob.Build(szCmdLine, m_pPropMap, val.c_str());
		if(!bRet)
		{
			return false;
		}
		m_pPropMap->PutProp(OVC_OPTS, szCmdLine.c_str());
	}
	
	bRet = m_pPropMap->GetProp(OAC, val);
	if(bRet)
	{
		AudioCodecOptBuilder acob;
		
		bRet = acob.Build(szCmdLine, m_pPropMap, val.c_str());
		if(!bRet)
		{
			return false;
		}
		m_pPropMap->PutProp(OAC_OPTS, szCmdLine.c_str());
	}

	szCmdLine.erase();
	//TODO: add app path
	szCmdLine.append("mencoder.exe");

	std::string option;
	int i;
	for(i = 0; i < n; i++)
	{
		if(!BuildCmdOption(option, &(pParams[i])))
		{
			return false;
		}
		if(option.empty())
		{
			continue;
		}
		szCmdLine.append(ARG_SEP).append(option);
	}

	//TODO: add output file

	szCmdLine.append(ARG_SEP).append("C:\\Temp\\x264.avi");
	return true;
}

bool MEncoderCmdBuilder::BuildCmdOption(std::string& option, const CmdParam* pParam)
{
	bool bRet;
	std::string val;

	if(pParam->nOptionFlag & COF_SUB_SET)
	{
		bRet = m_pPropMap->GetProp(pParam->szName, val);
		if(!bRet)
		{
			return false;
		}
		option.assign(val);
		return true;
	}
	if(pParam->nOptionFlag & COF_FLAG_TYPE)
	{
		option.assign(pParam->szCmdOption);
		return true;
	}
	
	bRet = m_pPropMap->GetProp(pParam->szName, val);
	if(!bRet)
	{
		//use default
		if(pParam->szDefaultValue != NULL)
		{
			val = pParam->szDefaultValue;
		}
		//optional param
		else if(pParam->nOptionFlag & COF_OPTIONAL)
		{
			//not setting, no default, ignore this param
			option.erase();
			return true;
		}
		//non-optional param, but neither setting by user or having a default value
		else
		{
			return false;
		}
	}
	option.assign(pParam->szCmdOption).append(ARG_SEP).append(val);

	return true;
}
