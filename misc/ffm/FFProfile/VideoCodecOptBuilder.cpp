// VideoCodecOptBuilder.cpp: implementation of the VideoCodecOptBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VideoCodecOptBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static const CmdParam xvidOptsBase[] = 
{
	{"bitrate",		COF_DEFAULT,		VIDEO_BITRATE,	"687"		},
	{"max_bframes",	COF_DEFAULT,		MAX_B_FRAMES,	"0"			},
};

static const CmdParam x264OptsBase[] = 
{
	{"bitrate",		COF_OPTIONAL,		VIDEO_BITRATE,	NULL		},
	{"subq",		COF_INTERNAL_SET,	NULL,			"6"			},
	{"ref",			COF_INTERNAL_SET,	NULL,			"2"			},
	{"bframes",		COF_INTERNAL_SET,	NULL,			"0"			},
	{"level_idc",	COF_INTERNAL_SET,	NULL,			"30"		},
	{"nocabac",		COF_FLAG_TYPE,		NULL,			NULL		},
	{"no8x8dct",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"me",			COF_INTERNAL_SET,	NULL,			"umh"		},
	{"trellis",		COF_INTERNAL_SET,	NULL,			"0"			},
	{"weightp",		COF_INTERNAL_SET,	NULL,			"0"			},
	{"-ffourcc",	COF_SET_PROP,		FOURCC,			"H264"		},
};

static const CmdParam mpeg4OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"mpeg4"		},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"mbd",			COF_INTERNAL_SET,	NULL,			"2"			},
	{"trell",		COF_FLAG_TYPE,		NULL,			NULL		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"-ffourcc",	COF_SET_PROP,		FOURCC,			"DIVX"		},
};

static const CmdParam mpeg2videoOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"mpeg2video"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"			},
	{"mbd",			COF_INTERNAL_SET,	NULL,			"2"			},
	{"trell",	COF_FLAG_TYPE,		NULL,	NULL			},
	{"keyint",	COF_INTERNAL_SET,		NULL,	"15"			},
	{"aspect",	COF_INTERNAL_SET,		NULL,	"4/3"			},
	{"vrc_buf_size",	COF_INTERNAL_SET,		NULL,	"1835"			},
	{"vrc_maxrate",	COF_INTERNAL_SET,		NULL,	"9800"			},
	{"vstrict",	COF_INTERNAL_SET,		NULL,	"0"			}
};

static const CmdParam mpeg1videoOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"mpeg1video"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"			},
	{"mbd",			COF_INTERNAL_SET,	NULL,			"2"			},
	{"trell",	COF_FLAG_TYPE,		NULL,	NULL			},
	{"keyint",	COF_INTERNAL_SET,		NULL,	"15"			},
	{"aspect",	COF_INTERNAL_SET,		NULL,	"4/3"			},
	{"vrc_buf_size",	COF_INTERNAL_SET,		NULL,	"327"			},
	{"vrc_maxrate",	COF_INTERNAL_SET,		NULL,	"1152"			},
	{"vstrict",	COF_INTERNAL_SET,		NULL,	"-1"			}
};

static const CmdParam ffvhuffOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"ffvhuff"	},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"format",	COF_INTERNAL_SET,		NULL,	"422p"		},
	{"pred",	COF_INTERNAL_SET,		NULL,	"2"		},
	{"context",	COF_INTERNAL_SET,		NULL,	"1"		},
	{"vstrict",	COF_INTERNAL_SET,		NULL,	"-1"			}
};

static const CmdParam ffv1OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"ffv1"	},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"format",	COF_INTERNAL_SET,		NULL,	"422p"		},
	{"coder",	COF_INTERNAL_SET,		NULL,	"0"		},
	{"context",	COF_INTERNAL_SET,		NULL,	"1"		}
};

static const CmdParam h263pOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"h263p"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		}
};

static const CmdParam msmpeg4OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"msmpeg4"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		}
};

static const CmdParam msmpeg4v2OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"msmpeg4v2"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		}
};

static const CmdParam mjpegOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"mjpeg"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		}
};

static const CmdParam wmv1OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"wmv1"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		}
};

static const CmdParam wmv2OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"wmv2"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		}
};

static const CmdParam vpxOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"libvpx"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"-ffourcc",	COF_SET_PROP,		FOURCC,			"VP80"		}
};

static const CmdParam flvOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"flv"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"mbd",			COF_INTERNAL_SET,	NULL,			"2"			},
	{"trell",		COF_FLAG_TYPE,		NULL,			NULL		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		}
};

static const CodecOptsCmdParam vcodecOptsParams[] = 
{
	{"xvid",	"xvid",		"-xvidencopts",		xvidOptsBase,	ARRAY_LEN(xvidOptsBase)		},
	{"x264",	"x264",		"-x264encopts",		x264OptsBase,	ARRAY_LEN(x264OptsBase)		},
	{"mpeg4",	"lavc",		"-lavcopts",		mpeg4OptsBase,	ARRAY_LEN(mpeg4OptsBase)	},
	{"mpeg2",	"lavc",		"-lavcopts",		mpeg2videoOptsBase,	ARRAY_LEN(mpeg2videoOptsBase)	},
	{"mpeg1",	"lavc",		"-lavcopts",		mpeg1videoOptsBase,	ARRAY_LEN(mpeg1videoOptsBase)	},
	{"ffvhuff",	"lavc",		"-lavcopts",		ffvhuffOptsBase,	ARRAY_LEN(ffvhuffOptsBase)	},
	{"ffv1",	"lavc",		"-lavcopts",		ffv1OptsBase,	ARRAY_LEN(ffv1OptsBase)	},
	{"h263p",	"lavc",		"-lavcopts",		h263pOptsBase,	ARRAY_LEN(h263pOptsBase)	},
	{"msmpeg4",	"lavc",		"-lavcopts",		msmpeg4OptsBase,	ARRAY_LEN(msmpeg4OptsBase)	},
	{"msmpeg4v2","lavc",	"-lavcopts",		msmpeg4v2OptsBase,	ARRAY_LEN(msmpeg4v2OptsBase)	},
	{"mjpeg",	"lavc",		"-lavcopts",		mjpegOptsBase,	ARRAY_LEN(mjpegOptsBase)	},
	{"wmv1",	"lavc",		"-lavcopts",		wmv1OptsBase,	ARRAY_LEN(wmv1OptsBase)	},
	{"wmv2",	"lavc",		"-lavcopts",		wmv2OptsBase,	ARRAY_LEN(wmv2OptsBase)	},
	{"VP8",		"lavc",		"-lavcopts",		vpxOptsBase,	ARRAY_LEN(vpxOptsBase)	},
	{"flv",		"lavc",		"-lavcopts",		flvOptsBase,	ARRAY_LEN(flvOptsBase)	}
};

VideoCodecOptBuilder::VideoCodecOptBuilder()
{

}

VideoCodecOptBuilder::~VideoCodecOptBuilder()
{

}

bool VideoCodecOptBuilder::Build(std::string& szCmdLine, PropMap* pPropMap, const char* vcodecName)
{
	int n = ARRAY_LEN(vcodecOptsParams);
	for(int i = 0; i < n; i++)
	{
		if(strcmp(vcodecOptsParams[i].szCodecName, vcodecName) == 0)
		{
			std::string szCodecOpts;
			bool bRet = Build(szCodecOpts, vcodecOptsParams[i].pOptsParams, vcodecOptsParams[i].nOptsParamCount, pPropMap);
			if(!bRet)
			{
				return false;
			}

			//update -ovc option value
			pPropMap->PutProp(OVC, vcodecOptsParams[i].szOVC);

			//set command line
			szCmdLine.assign(vcodecOptsParams[i].szCodecOptsCmdOption).append(ARG_SEP).append(szCodecOpts);

			return true;
		}
	}
	return false;
}


bool VideoCodecOptBuilder::Build(std::string& szCodecOpts, const CmdParam* pParams, int n, PropMap* pPropMap)
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
bool VideoCodecOptBuilder::BuildCmdOption(std::string& option, const CmdParam* pParam, PropMap* pPropMap)
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
