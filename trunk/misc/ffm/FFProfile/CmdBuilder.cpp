#include "stdafx.h"
#include "CmdBuilder.h"

PropMap::PropMap()
{
}

PropMap::~PropMap()
{

}
bool PropMap::ContainsKey(const char* key)
{
	ssmap::iterator iter = m_propMap.find(key);
	return iter != m_propMap.end();
}
bool PropMap::GetProp(const char* key, std::string& val)
{
	ssmap::iterator iter = m_propMap.find(key);
	if(iter != m_propMap.end())
	{
		val = iter->second;
		return true;
	}
	return false;
}
void PropMap::PutProp(const char* key, const char* val)
{
	ssmap::iterator iter = m_propMap.find(key);
	if(iter != m_propMap.end())
	{
		iter->second.assign(val);
	}
	else
	{
		m_propMap.insert(std::make_pair<std::string, std::string>(key, val));
	}
}

bool BuildCmdOption(std::string& option, const CmdParam* pParam, const char* sep, PropMap* pPropMap)
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
		option.assign(pParam->szCmdOption).append(sep).append(pParam->szDefaultValue);
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
	option.assign(pParam->szCmdOption).append(sep).append(val);
	
	return true;
}

bool BuildOptions(std::string& szCodecOpts, const CmdParam* pParams, int n, const char* sepInterOpt, 
				  const char* sepOptKeyVal, PropMap* pPropMap)
{
	szCodecOpts.erase();
	
	std::string option;
	for(int i = 0; i < n; i++)
	{
		if(!::BuildCmdOption(option, &pParams[i], sepOptKeyVal, pPropMap))
		{
			return false;
		}
		if(option.empty())
		{
			continue;
		}
		
		if(!szCodecOpts.empty())
		{
			szCodecOpts.append(sepInterOpt);
		}
		szCodecOpts.append(option);
	}
	
	return true;
}

bool BuildSubOptions(std::string& option, const SubOptsParam* pSubOptParam, PropMap* pPropMap)
{
	std::string szOptionVal;
	bool bRet = ::BuildOptions(szOptionVal, pSubOptParam->pOptsParams, pSubOptParam->nOptsParamCount, 
		OPTION_SEP, PROP_SEP, pPropMap);
	if(!bRet)
	{
		return false;
	}
	
	//update -ovc option value
	//pPropMap->PutProp(OVC, vcodecOptsParams[i].szOVC);
	
	//set command line
	option.assign(pSubOptParam->szSubOptCmdName).append(ARG_SEP).append(szOptionVal);
	
	pPropMap->PutProp(pSubOptParam->szSubOptPropName, option.c_str());
	
	return true;
}

bool BuildSubOptions(std::string& option, const SubOptsParam* pSubOptParam, int n, PropMap* pPropMap)
{
	option.erase();

	std::string val;
	bool bRet;
	for(int i = 0; i < n; i++)
	{
		bRet = pPropMap->GetProp(pSubOptParam[i].szFindOptPropName, val);
		if(!bRet)
		{
			continue;
		}
		if(strcmp(pSubOptParam[i].szFindOptPropValue, val.c_str()) != 0)
		{
			continue;
		}
		
		bRet = ::BuildOptions(val, pSubOptParam[i].pOptsParams, pSubOptParam[i].nOptsParamCount, 
			OPTION_SEP, PROP_SEP, pPropMap);
		if(!bRet)
		{
			return false;
		}
		
		//set command line
		if(!val.empty())
		{
			option.assign(pSubOptParam[i].szSubOptCmdName).append(ARG_SEP).append(val);

			pPropMap->PutProp(pSubOptParam[i].szSubOptPropName, option.c_str());
		}
		return true;
	}
	return false;
}

bool ParseSize(const std::string& str, int& w, int& h)
{
	size_t index = str.find('x');
	if(index == std::string::npos || index == 0 || (index == str.size() - 1))
	{
		return false;
	}
	std::string szWidth = str.substr(0, index);
	w = atoi(szWidth.c_str());

	std::string szHeight = str.substr(index + 1);
	h = atoi(szHeight.c_str());

	return w > 0 && h > 0;
}