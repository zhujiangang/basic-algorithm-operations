// OptionExpBuilder.cpp: implementation of the OptionExpBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionExpBuilder.h"
#include "DefaultOptionExp.h"
#include "ChoiceOptionExp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifndef PATH_SEP
#define PATH_SEP	"\\"
#endif

#ifndef ARG_SEP
#define ARG_SEP		" "
#endif

//-lavcopts vcodec=mpeg4:mbd=2:trell:autoaspect:vbitrate=512:vhq
#ifndef PROP_SEP
#define PROP_SEP	"="
#endif

//-lavcopts vcodec=mpeg4:mbd=2:trell:autoaspect:vbitrate=512:vhq
#ifndef OPTION_SEP
#define OPTION_SEP	":"
#endif

//-vf-add scale=480:272,harddup
#ifndef	FILTER_SEP
#define FILTER_SEP	","
#endif

#define SEEK_TIME			"SeekTime"
#define END_TIME			"EndTime"
#define MAX_AV_SYNC			"MaxAVSync"
#define NO_SKIP				"NoSkip"
#define SWS					"SWS"
#define AUDIO_FILTER		"AudioFilter"
#define VIDEO_FILTER		"VideoFilter"
#define VIDEO_SIZE			"Video Size"
#define VIDEO_WIDTH			"VideoWidth"
#define VIDEO_HEIGHT		"VideoHeight"
#define OSRATE				"OutputSampleRate"
#define OFPS				"OFPS"
#define OAC					"OAC"
#define OAC_OPTS			"__OACOpts__"
#define OVC					"OVC"
#define OVC_OPTS			"__OVCOpts__"
#define FOURCC				"FourCC"

#define OUT_FORMAT			"OutputFormat"
#define OF_OPTS				"__OFOpts__"

#define IFILE				"InputFile"
#define OFILE				"OutputFile"

#define VIDEO_BITRATE		"VideoBitrate"
#define AUDIO_BITRATE		"AudioBitrate"
#define MAX_B_FRAMES		"MaxBFrames"

/*
class OptionParam
{
public:
const char*		szID;
const char*		szName;
const char*		szValue;
const char*		szNameValueSep;
const char*		szSubOptionSep;
int				nEvaluateMode;
int				nEvaluateFlag;

OptionParam*	pSubOptions;
int				nSubOptionCount;
};
*/

static const OptionParam xvidParam1[] = 
{
	{VIDEO_BITRATE,	"bitrate",		"687",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE | OPTEF_MUST,	NULL},
	{MAX_B_FRAMES,	"max_bframes",	"0",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},

	{OVC,			"xvid",			NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),	OPTEF_NONE,			NULL},

	{OVC_OPTS,		"-xvidencopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam x264Param1[] = 
{
	{VIDEO_BITRATE,	"bitrate",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE | OPTEF_MUST,	NULL},
	{NULL,			"subq",		"6",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"ref",		"2",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"bframes",	"0",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"level_idc","30",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"nocabac",	"",			NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{NULL,			"no8x8dct",	"",			NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{NULL,			"me",		"umh",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"trellis",	"0",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"weightp",	"0",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{FOURCC,		"-ffourcc",	"H264",		NULL,		NULL,		MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_VALUE, CONTEXT_VALUE),	OPTEF_NONE,			NULL},
	{OVC,			"x264",		NULL,		NULL,		NULL,		MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,			NULL},
	{OVC_OPTS,		"-x264encopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam mpeg4Param1[] = 
{
	{NULL,			"vcodec",	"mpeg4",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE | OPTEF_MUST,	NULL},
	{NULL,			"mbd",		"2",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"trell",	"",			NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{FOURCC,		NULL,		"DIVX",		NULL,		NULL,		MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_VALUE, CONTEXT_VALUE),	OPTEF_NONE,			NULL},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL}
};

static const NamedOptionGroup groups[] = 
{
	{"xvid",	xvidParam1, ARRAY_LEN(xvidParam1)},
	{"x264",	x264Param1, ARRAY_LEN(x264Param1)},
	{"mpeg4",	mpeg4Param1, ARRAY_LEN(mpeg4Param1)}
};

static const ChoiceOptionParam choiceOption =
{
	SOT_CHOICE,
	OVC,
	groups,
	ARRAY_LEN(groups)
};

//static const ChoiceOptionParam 
static const OptionParam mencoderParam1[] = 
{
	{SEEK_TIME,		"-ss",		"0",		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{END_TIME,		"-endpos",	"20",		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{NO_SKIP,		"-noskip",	"",			ARG_SEP,	NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{SWS,			"-sws",		"",			ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},

	{OUT_FORMAT,	"-of",		NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT,		NULL},
	{OF_OPTS,		NULL,		NULL,		ARG_SEP,	OPTION_SEP,	OPTEM_DEFAULT,		OPTEF_CHILDREN,		/*TODO*/NULL},

	{AUDIO_FILTER,	"-af",		"volnorm",	ARG_SEP,	FILTER_SEP,	OPTEM_DEFAULT,		OPTEF_MUST | OPT_HAS_CHILDREN,	/*TODO*/NULL},
	{VIDEO_FILTER,	"-vf",		"harddup",	ARG_SEP,	FILTER_SEP,	OPTEM_DEFAULT,		OPTEF_MUST | OPTEF_CHILDREN | OPTEF_SELF,	/*TODO*/NULL},
	
	{OSRATE,		"-srate",	NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_MUST | OPTEF_CONTEXT,		NULL},
	{OFPS,			"-ofps",	NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_MUST | OPTEF_CONTEXT,		NULL},


	{OAC,			"-oac",		NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_MUST | OPTEF_CONTEXT,		NULL},
	{OAC_OPTS,		NULL,		NULL,		ARG_SEP,	OPTION_SEP,	OPTEM_DEFAULT,		OPTEF_CHILDREN,		/*TODO*/NULL},

	{OVC,			"-ovc",		NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT,		NULL},
	{FOURCC,		"-ffourcc",	"",			ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{OVC_OPTS,		NULL,		NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CHILDREN,		(void*)&choiceOption},

	{IFILE,			"",			NULL,		ARG_SEP,	NULL,		OPTEM_VALUE_ONLY,	OPTEF_MUST | OPTEF_CONTEXT,		NULL},
	{OFILE,			"-o",		NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_MUST | OPTEF_CONTEXT,		NULL}
};


bool IsParamValid(const OptionParam* pParam)
{
	if(pParam == NULL)
	{
		return false;
	}
	if(pParam->szID == NULL && pParam->szName == NULL && pParam->szValue == NULL && pParam->szNameValueSep == NULL
		&& pParam->szSubOptionSep == NULL && pParam->nEvaluateMode == 0 && pParam->nEvaluateFlag == 0
		&& pParam->pChildren == NULL)
	{
		return false;
	}
	return true;
}

bool SomeFunc(OptionContext* pContext, DefaultOptionExp* pExp, OptionParams* pParams)
{
	bool bRet;
	std::string val;
	bRet = pContext->Get(OVC, val);
	if(!bRet)
	{
		return false;
	}

	if(val.compare(0, strlen("xvid"), "xvid") == 0)
	{
		pParams->pSubOptions = xvidParam1;
		pParams->nSubOptionCount = sizeof(xvidParam1)/sizeof(xvidParam1[0]);

		pExp->SetOptionName("-xvidencopts");
	}
	else if(val.compare(0, strlen("x264"), "x264") == 0)
	{
		pParams->pSubOptions = x264Param1;
		pParams->nSubOptionCount = sizeof(x264Param1)/sizeof(x264Param1[0]);
		
		pExp->SetOptionName("-x264opts");
	}

	return false;
}

OptionExpBuilder::OptionExpBuilder()
{

}

OptionExpBuilder::~OptionExpBuilder()
{

}

OptionExp* OptionExpBuilder::Build(const OptionParam* pParams, int nCount)
{
	if(pParams == NULL || nCount <= 0)
	{
		return NULL;
	}

	DefaultOptionExp* pRoot = new DefaultOptionExp();
	pRoot->SetSubOptionSep(ARG_SEP).SetEvaluateFlag(OPTEF_MUST | OPTEF_CHILDREN).SetEvaluateMode(OPTEM_VALUE_ONLY);

	OptionExp* pExp = NULL;
	for(int i = 0; i < nCount; i++)
	{
		pExp = Build(&pParams[i]);
		pRoot->AddChild(pExp);
	}

	return pRoot;
}

OptionExp* OptionExpBuilder::Build(const OptionParam* pParam)
{
	if(pParam == NULL)
	{
		return NULL;
	}
	if(!IsParamValid(pParam))
	{
		return NULL;
	}
// 	DefaultOptionExp* pExp = new DefaultOptionExp();
// 	pExp->SetOptionID(pParam->szID).SetOptionName(pParam->szName).SetOptionValue(pParam->szValue);
// 	pExp->SetNameValueSep(pParam->szNameValueSep).SetSubOptionSep(pParam->szSubOptionSep);
// 	pExp->SetEvaluateMode(pParam->nEvaluateMode).SetEvaluateFlag(pParam->nEvaluateFlag);

	if(pParam->pChildren)
	{
		BaseSubOptionParam* pSubOption = (BaseSubOptionParam*)pParam->pChildren;
		if(pSubOption->nType == SOT_CHOICE)
		{
			ChoiceOptionExp* pExp = new ChoiceOptionExp();
			pExp->SetField(OPT_FIELD_ID, pParam->szID).SetField(OPT_FIELD_NAME, pParam->szName);
			pExp->SetField(OPT_FIELD_VALUE, pParam->szValue).SetField(OPT_FIELD_NAME_VALUE_SEP, pParam->szNameValueSep);
			pExp->SetField(OPT_FIELD_SUB_OPTION_SEP, pParam->szSubOptionSep);

			ChoiceOptionParam* pChoices = (ChoiceOptionParam*)pParam->pChildren;
			pExp->SetChoiceOptionID(pChoices->szChoiceOptionID);

			OptionExp* pSubExp;
			for(int i = 0; i < pChoices->nGroupCount; i++)
			{
				for(int j = 0; j < pChoices->pGroups[i].nOptionCount; j++)
				{
					pSubExp = Build(&pChoices->pGroups[i].pOptions[j]);
					pExp->AddChoice(pChoices->pGroups[i].szGroupName, pSubExp);
				}
			}

			return pExp;
		}
		else if(pSubOption->nType == SOT_LIST)
		{
			DefaultOptionExp* pExp = new DefaultOptionExp();
			pExp->SetOptionID(pParam->szID).SetOptionName(pParam->szName).SetOptionValue(pParam->szValue);
			pExp->SetNameValueSep(pParam->szNameValueSep).SetSubOptionSep(pParam->szSubOptionSep);
			pExp->SetEvaluateMode(pParam->nEvaluateMode).SetEvaluateFlag(pParam->nEvaluateFlag);

			OptionParamList* pOptionList = (OptionParamList*)pParam->pChildren;
			OptionExp* pSubExp;
			for(int i = 0; i < pOptionList->nOptionCount; i++)
			{
				pSubExp = Build(&pOptionList->pOptions[i]);
				if(pSubExp)
				{
					pExp->AddChild(pSubExp);
				}
			}

			return pExp;
		}
	}

	DefaultOptionExp* pExp = new DefaultOptionExp();
	pExp->SetOptionID(pParam->szID).SetOptionName(pParam->szName).SetOptionValue(pParam->szValue);
	pExp->SetNameValueSep(pParam->szNameValueSep).SetSubOptionSep(pParam->szSubOptionSep);
	pExp->SetEvaluateMode(pParam->nEvaluateMode).SetEvaluateFlag(pParam->nEvaluateFlag);
	
	return pExp;
	/*
	for(int i = 0; i < pParam->nSubOptionCount; i++)
	{
		OptionExp* pGroupOpt = Build(&pParam->pSubOptions[i]);
		if(pGroupOpt)
		{
			pExp->AddChild(pGroupOpt);
		}
	}
	*/
	//return pExp;
}
