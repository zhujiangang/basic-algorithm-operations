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

VideoFilterBuilder::VideoFilterBuilder()
{

}

VideoFilterBuilder::~VideoFilterBuilder()
{

}

bool VideoFilterBuilder::Build(std::string& szCmdLine, PropMap* pPropMap)
{
	return false;
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
