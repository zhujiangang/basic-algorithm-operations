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
	{"",			COF_SET_PROP,		OVC,			"xvid"		}
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
	{"",			COF_SET_PROP,		OVC,			"x264"		}
};

static const CmdParam mpeg4OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"mpeg4"		},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"mbd",			COF_INTERNAL_SET,	NULL,			"2"			},
	{"trell",		COF_FLAG_TYPE,		NULL,			NULL		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"-ffourcc",	COF_SET_PROP,		FOURCC,			"DIVX"		},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
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
	{"vstrict",	COF_INTERNAL_SET,		NULL,	"0"			},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
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
	{"vstrict",	COF_INTERNAL_SET,		NULL,	"-1"			},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
};

static const CmdParam ffvhuffOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"ffvhuff"	},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"format",	COF_INTERNAL_SET,		NULL,	"422p"		},
	{"pred",	COF_INTERNAL_SET,		NULL,	"2"		},
	{"context",	COF_INTERNAL_SET,		NULL,	"1"		},
	{"vstrict",	COF_INTERNAL_SET,		NULL,	"-1"			},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
};

static const CmdParam ffv1OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"ffv1"	},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"format",	COF_INTERNAL_SET,		NULL,	"422p"		},
	{"coder",	COF_INTERNAL_SET,		NULL,	"0"		},
	{"context",	COF_INTERNAL_SET,		NULL,	"1"		},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
};

static const CmdParam h263pOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"h263p"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
};

static const CmdParam msmpeg4OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"msmpeg4"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
};

static const CmdParam msmpeg4v2OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"msmpeg4v2"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
};

static const CmdParam mjpegOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"mjpeg"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
};

static const CmdParam wmv1OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"wmv1"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
};

static const CmdParam wmv2OptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"wmv2"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
};

static const CmdParam vpxOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"libvpx"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"-ffourcc",	COF_SET_PROP,		FOURCC,			"VP80"		},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
};

static const CmdParam flvOptsBase[] = 
{
	{"vcodec",		COF_INTERNAL_SET,	NULL,			"flv"	},
	{"vbitrate",	COF_DEFAULT,		VIDEO_BITRATE,	"800"		},
	{"mbd",			COF_INTERNAL_SET,	NULL,			"2"			},
	{"trell",		COF_FLAG_TYPE,		NULL,			NULL		},
	{"autoaspect",	COF_FLAG_TYPE,		NULL,			NULL		},
	{"",			COF_SET_PROP,		OVC,			"lavc"		}
};

/*
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
*/

static const SubOptsParam vcodecOptsParams1[] = 
{
	{OVC, "xvid", OVC_OPTS, "-xvidencopts",		xvidOptsBase,	ARRAY_LEN(xvidOptsBase)		},
	{OVC, "x264", OVC_OPTS, "-x264encopts",		x264OptsBase,	ARRAY_LEN(x264OptsBase)		},
	{OVC, "mpeg4", OVC_OPTS, "-lavcopts",		mpeg4OptsBase,	ARRAY_LEN(mpeg4OptsBase)	},
	{OVC, "mpeg2video", OVC_OPTS, "-lavcopts",		mpeg2videoOptsBase,	ARRAY_LEN(mpeg2videoOptsBase)	},
	{OVC,"mpeg1",	OVC_OPTS, "-lavcopts",		mpeg1videoOptsBase,	ARRAY_LEN(mpeg1videoOptsBase)	},
	{OVC,"ffvhuff",	OVC_OPTS, "-lavcopts",		ffvhuffOptsBase,	ARRAY_LEN(ffvhuffOptsBase)	},
	{OVC,"ffv1",	OVC_OPTS, "-lavcopts",		ffv1OptsBase,	ARRAY_LEN(ffv1OptsBase)	},
	{OVC,"h263p",	OVC_OPTS, "-lavcopts",		h263pOptsBase,	ARRAY_LEN(h263pOptsBase)	},
	{OVC,"msmpeg4",	OVC_OPTS, "-lavcopts",		msmpeg4OptsBase,	ARRAY_LEN(msmpeg4OptsBase)	},
	{OVC,"msmpeg4v2",OVC_OPTS, "-lavcopts",		msmpeg4v2OptsBase,	ARRAY_LEN(msmpeg4v2OptsBase)	},
	{OVC,"mjpeg",	OVC_OPTS, "-lavcopts",		mjpegOptsBase,	ARRAY_LEN(mjpegOptsBase)	},
	{OVC,"wmv1",	OVC_OPTS, "-lavcopts",		wmv1OptsBase,	ARRAY_LEN(wmv1OptsBase)	},
	{OVC,"wmv2",	OVC_OPTS, "-lavcopts",		wmv2OptsBase,	ARRAY_LEN(wmv2OptsBase)	},
	{OVC,"VP8",		OVC_OPTS, "-lavcopts",		vpxOptsBase,	ARRAY_LEN(vpxOptsBase)	},
	{OVC,"flv",		OVC_OPTS, "-lavcopts",		flvOptsBase,	ARRAY_LEN(flvOptsBase)	}
};

VideoCodecOptBuilder::VideoCodecOptBuilder()
{

}

VideoCodecOptBuilder::~VideoCodecOptBuilder()
{

}

bool VideoCodecOptBuilder::Build(std::string& szCmdLine, PropMap* pPropMap, const char* vcodecName)
{
	return ::BuildSubOptions(szCmdLine, vcodecOptsParams1, ARRAY_LEN(vcodecOptsParams1), pPropMap);
	/*
	int n = ARRAY_LEN(vcodecOptsParams);
	for(int i = 0; i < n; i++)
	{
		if(strcmp(vcodecOptsParams[i].szCodecName, vcodecName) == 0)
		{
			std::string szCodecOpts;
			//bool bRet = Build(szCodecOpts, vcodecOptsParams[i].pOptsParams, vcodecOptsParams[i].nOptsParamCount, pPropMap);
			bool bRet = ::BuildOptions(szCodecOpts, vcodecOptsParams[i].pOptsParams, vcodecOptsParams[i].nOptsParamCount, 
				OPTION_SEP, PROP_SEP, pPropMap);
			if(!bRet)
			{
				return false;
			}

			//update -ovc option value
			pPropMap->PutProp(OVC, vcodecOptsParams[i].szOVC);

			//set command line
			szCmdLine.assign(vcodecOptsParams[i].szCodecOptsCmdOption).append(ARG_SEP).append(szCodecOpts);

			pPropMap->PutProp(OVC_OPTS, szCmdLine.c_str());

			return true;
		}
	}
	return false;
	*/
}


bool VideoCodecOptBuilder::Build(std::string& szCodecOpts, const CmdParam* pParams, int n, PropMap* pPropMap)
{
	/*
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
	*/

	return ::BuildOptions(szCodecOpts, pParams, n, OPTION_SEP, PROP_SEP, pPropMap);
}
bool VideoCodecOptBuilder::BuildCmdOption(std::string& option, const CmdParam* pParam, PropMap* pPropMap)
{
	return ::BuildCmdOption(option, pParam, PROP_SEP, pPropMap);
}
