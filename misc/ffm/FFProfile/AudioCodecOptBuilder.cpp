// AudioCodecOptBuilder.cpp: implementation of the AudioCodecOptBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FFProfile.h"
#include "AudioCodecOptBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-oac faac -faacopts br=128:mpeg=4:object=2
static const CmdParam aacOptsBase[] = 
{
	{"br",			COF_DEFAULT,		AUDIO_BITRATE,	"128"		},
	{"mpeg",		COF_INTERNAL_SET,	NULL,			"4"			},
	{"object",		COF_INTERNAL_SET,	NULL,			"2"			},
};

//-oac mp3lame -lameopts abr:br=128
static const CmdParam mp3OptsBase[] = 
{
	{"br",			COF_DEFAULT,		AUDIO_BITRATE,	"128"		},
	{"abr",			COF_FLAG_TYPE,		NULL,			NULL		}
};

//-oac lavc -lavcopts acodec=ac3:abitrate=128
static const CmdParam ac3OptsBase[] = 
{
	{"acodec",		COF_INTERNAL_SET,	NULL,			"ac3"		},
	{"abitrate",	COF_DEFAULT,		AUDIO_BITRATE,	"128"		}
};

//-oac lavc -lavcopts acodec=mp2:abitrate=128 
static const CmdParam mp2OptsBase[] = 
{
	{"acodec",		COF_INTERNAL_SET,	NULL,			"mp2"		},
	{"abitrate",	COF_DEFAULT,		AUDIO_BITRATE,	"128"		}
};

//-oac lavc -lavcopts acodec=wmav1:abitrate=128 
static const CmdParam wmav1OptsBase[] = 
{
	{"acodec",		COF_INTERNAL_SET,	NULL,			"wmav1"		},
	{"abitrate",	COF_DEFAULT,		AUDIO_BITRATE,	"128"		}
};

//-oac lavc -lavcopts acodec=wmav1:abitrate=128 
static const CmdParam wmav2OptsBase[] = 
{
	{"acodec",		COF_INTERNAL_SET,	NULL,			"wmav2"		},
	{"abitrate",	COF_DEFAULT,		AUDIO_BITRATE,	"128"		}
};

static const CodecOptsCmdParam acodecOptsParams[] = 
{
	{"aac",		"faac",		"-faacopts",		aacOptsBase,	ARRAY_LEN(aacOptsBase)		},
	{"faac",	"faac",		"-faacopts",		aacOptsBase,	ARRAY_LEN(aacOptsBase)		},
	{"mp3",		"mp3lame",	"-lameopts",		mp3OptsBase,	ARRAY_LEN(mp3OptsBase)		},
	{"ac3",		"lavc",		"-lavcopts",		ac3OptsBase,	ARRAY_LEN(ac3OptsBase)	},
	{"mp2",		"lavc",		"-lavcopts",		mp2OptsBase,	ARRAY_LEN(mp2OptsBase)	},
	{"wmav1",	"lavc",		"-lavcopts",		wmav1OptsBase,	ARRAY_LEN(wmav1OptsBase)	},
	{"wmav2",	"lavc",		"-lavcopts",		wmav2OptsBase,	ARRAY_LEN(wmav2OptsBase)	},
	{"pcm",		"",		"",		NULL,		0	}
};


AudioCodecOptBuilder::AudioCodecOptBuilder()
{

}

AudioCodecOptBuilder::~AudioCodecOptBuilder()
{
	
}

bool AudioCodecOptBuilder::Build(std::string& szCmdLine, PropMap* pPropMap, const char* vcodecName)
{
	int n = ARRAY_LEN(acodecOptsParams);
	for(int i = 0; i < n; i++)
	{
		if(strcmp(acodecOptsParams[i].szCodecName, vcodecName) == 0)
		{
			std::string szCodecOpts;
			bool bRet = Build(szCodecOpts, acodecOptsParams[i].pOptsParams, acodecOptsParams[i].nOptsParamCount, pPropMap);
			if(!bRet)
			{
				return false;
			}

			//update -ovc option value
			pPropMap->PutProp(OAC, acodecOptsParams[i].szOVC);

			//set command line
			szCmdLine.assign(acodecOptsParams[i].szCodecOptsCmdOption);
			if(!szCodecOpts.empty())
			{
				szCmdLine.append(ARG_SEP).append(szCodecOpts);
			}

			return true;
		}
	}
	return false;
}


bool AudioCodecOptBuilder::Build(std::string& szCodecOpts, const CmdParam* pParams, int n, PropMap* pPropMap)
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
			szCodecOpts.append(OPTION_SEP);
		}
		szCodecOpts.append(option);
	}
	
	return true;
}
bool AudioCodecOptBuilder::BuildCmdOption(std::string& option, const CmdParam* pParam, PropMap* pPropMap)
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

