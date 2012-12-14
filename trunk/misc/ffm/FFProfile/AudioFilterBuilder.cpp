// AudioFilterBuilder.cpp: implementation of the AudioFilterBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FFProfile.h"
#include "AudioFilterBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-af volnorm
static const CmdParam afBase[] = 
{
	{"volnorm",		COF_FLAG_TYPE,		NULL,			NULL		}
};

//-af volnorm,lavcresample=44100
static const CmdParam afCommon[] = 
{
	{"volnorm",		COF_FLAG_TYPE,		NULL,			NULL		},
	{"lavcresample",COF_OPTIONAL,		OSRATE,			NULL		}
};


AudioFilterBuilder::AudioFilterBuilder()
{

}

AudioFilterBuilder::~AudioFilterBuilder()
{

}

bool AudioFilterBuilder::Build(std::string& szCmdLine, PropMap* pPropMap)
{
	const CmdParam* pParams = afBase;
	int n = ARRAY_LEN(afBase);

	std::string ovc;
	pPropMap->GetProp(OVC, ovc);
	if(strcmp(ovc.c_str(), "mpeg4") == 0 || strcmp(ovc.c_str(), "mpeg2") == 0 || strcmp(ovc.c_str(), "mpeg1") == 0
		|| strcmp(ovc.c_str(), "msmpeg4") == 0 || strcmp(ovc.c_str(), "msmpeg4v2") == 0)
	{
		pParams = afCommon;
		n = ARRAY_LEN(afCommon);
	}

	return Build(szCmdLine, pParams, n, pPropMap);
}

bool AudioFilterBuilder::Build(std::string& szCodecOpts, const CmdParam* pParams, int n, PropMap* pPropMap)
{
	szCodecOpts.erase();
	
	std::string option;
	int i;
	for(i = 0; i < n; i++)
	{
		if(!BuildCmdOption(option, &(pParams[i]), pPropMap))
		{
			return false;
		}
		if(option.empty())
		{
			continue;
		}
		
		if(!szCodecOpts.empty())
		{
			szCodecOpts.append(FILTER_SEP);
		}
		szCodecOpts.append(option);
	}
	
	return true;
}
bool AudioFilterBuilder::BuildCmdOption(std::string& option, const CmdParam* pParam, PropMap* pPropMap)
{
	//output parameters
	if(pParam->nOptionFlag & COF_SET_PROP)
	{
		option.erase();
		pPropMap->PutProp(pParam->szName, pParam->szDefaultValue);
		return true;
	}
	
	//flag type
	if(pParam->nOptionFlag & COF_FLAG_TYPE)
	{
		option.assign(pParam->szCmdOption);
		return true;
	}
	
	//internal set
	if(pParam->nOptionFlag & COF_INTERNAL_SET)
	{
		option.assign(pParam->szCmdOption).append(PROP_SEP).append(pParam->szDefaultValue);
		return true;
	}
	
	//determinated by outside parameter
	std::string val;
	bool bRet = pPropMap->GetProp(pParam->szName, val);
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
	option.assign(pParam->szCmdOption).append(PROP_SEP).append(val);
	
	return true;
}

