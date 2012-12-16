// VideoFilterBuilder.cpp: implementation of the VideoFilterBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FFProfile.h"
#include "VideoFilterBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
//-vf scale=720:-2,expand=:480:::,crop=720:480,harddup
static const FilterParam scaleParams[] = 
{
	{COF_DEFAULT,		VIDEO_WIDTH,	NULL},
	{COF_INTERNAL_SET,	NULL,			"-2"}
};

static const FilterParam expandParams[] = 
{
	{COF_INTERNAL_SET,	NULL,	""},
	{COF_DEFAULT,		VIDEO_HEIGHT,	NULL},
	{COF_INTERNAL_SET,	NULL,	""},
	{COF_INTERNAL_SET,	NULL,	""},
	{COF_INTERNAL_SET,	NULL,	""}
};

static const FilterParam cropParams[] = 
{
	{COF_DEFAULT,	VIDEO_WIDTH,	NULL},
	{COF_DEFAULT,	VIDEO_HEIGHT,	NULL}
};

static const FilterParam scaleHardParams[] = 
{
	{COF_DEFAULT,	VIDEO_WIDTH,	NULL},
	{COF_DEFAULT,	VIDEO_HEIGHT,	NULL}
};
*/

//-vf scale=720:-2,expand=:480:::,crop=720:480,harddup
static const CmdParam scaleParams[] = 
{
	{"w", COF_DEFAULT,		VIDEO_WIDTH,	NULL},
	{"h", COF_INTERNAL_SET,	NULL,			"-2"}
};

static const CmdParam expandParams[] = 
{
	{"w", COF_INTERNAL_SET,	NULL,	""},
	{"h", COF_DEFAULT,		VIDEO_HEIGHT,	NULL},
	{"x", COF_INTERNAL_SET,	NULL,	""},
	{"y", COF_INTERNAL_SET,	NULL,	""},
	{"osd", COF_INTERNAL_SET,	NULL,	""}
};

static const CmdParam cropParams[] = 
{
	{"w", COF_DEFAULT,	VIDEO_WIDTH,	NULL},
	{"h", COF_DEFAULT,	VIDEO_HEIGHT,	NULL}
};

static const CmdParam scaleHardParams[] = 
{
	{"w", COF_DEFAULT,	VIDEO_WIDTH,	NULL},
	{"h", COF_DEFAULT,	VIDEO_HEIGHT,	NULL}
};

static const FilterOptsCmdParam filterOtps[] = 
{
	{"scale",	scaleParams, ARRAY_LEN(scaleParams) },
	{"expand",	expandParams, ARRAY_LEN(expandParams) },
	{"crop",	cropParams, ARRAY_LEN(cropParams) },
//	{"scale",	scaleHardParams, ARRAY_LEN(scaleHardParams) },
	{"harddup",	NULL, 0 },
};

VideoFilterBuilder::VideoFilterBuilder()
{

}

VideoFilterBuilder::~VideoFilterBuilder()
{

}

bool VideoFilterBuilder::Build(std::string& szCmdLine, PropMap* pPropMap)
{
	return Build(szCmdLine, filterOtps, ARRAY_LEN(filterOtps), pPropMap);
}

bool VideoFilterBuilder::Build(std::string& szCodecOpts, const FilterOptsCmdParam* pParams, int n, PropMap* pPropMap)
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
bool VideoFilterBuilder::BuildCmdOption(std::string& option, const FilterOptsCmdParam* pParam, PropMap* pPropMap)
{
	option.erase();

	option.assign(pParam->szFilterName);

	if(pParam->nFilterparamsCount > 0)
	{
		option.append(PROP_SEP);
		for(int i = 0; i < pParam->nFilterparamsCount; i++)
		{
			if(i > 0)
			{
				option.append(OPTION_SEP);
			}
			if(pParam->pFilterParams[i].nOptionFlag & COF_INTERNAL_SET)
			{
				option.append(pParam->pFilterParams[i].szDefaultValue);
				continue;
			}

			//default
			std::string val;
			bool bRet = pPropMap->GetProp(pParam->pFilterParams[i].szName, val);
			if(!bRet)
			{
				//use default
				if(pParam->pFilterParams[i].szDefaultValue != NULL)
				{
					val = pParam->pFilterParams[i].szDefaultValue;
				}
				//optional param
				else if(pParam->pFilterParams[i].nOptionFlag & COF_OPTIONAL)
				{
					//not setting, no default, ignore this param
					continue;
				}
				//non-optional param, but neither setting by user or having a default value
				else
				{
					return false;
				}
			}
			option.append(val);
		}
	}
	
	return true;
}
