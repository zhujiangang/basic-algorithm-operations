#include "stdafx.h"
#include "CmdBuilder.h"
#include "log4cplus_config.h"

void CmdInfo::Clear()
{
	m_szBinFQPath.erase();
	m_szInputFQPath.erase();
	m_szOutputFQPath.erase();
	m_szCmdLine.erase();
	m_nWeight = -1;
}

StrObjPtrContext::StrObjPtrContext()
{
}
StrObjPtrContext::~StrObjPtrContext()
{
	StrObjPtrMap::iterator iter = m_map.begin();

	for(; iter != m_map.end(); iter++)
	{
		if(iter->second != NULL)
		{
			if(IS_LOG_ENABLED(THE_LIB_LOGGER, log4cplus::TRACE_LOG_LEVEL))
			{
				cfl::tstring szLog;
				cfl::tformat(szLog, _T("Delete object [%s] from StrObjPtrContext"), CFL_STRING_TO_T_STR(iter->first));
				LOG4CPLUS_TRACE_STR(THE_LIB_LOGGER, szLog)
			}
			delete iter->second;
			iter->second = NULL;
		}
	}
	m_map.clear();
}

bool StrObjPtrContext::Get(const std::string& key, cfl::Object** pval)
{
	StrObjPtrMap::iterator iter = m_map.find(key);
	if(iter != m_map.end())
	{
		if(pval != NULL)
		{
			*pval = iter->second;
		}
		return true;
	}
	return false;
}
void StrObjPtrContext::Put(const std::string& key, cfl::Object* val)
{
	StrObjPtrMap::iterator iter = m_map.find(key);
	if(iter != m_map.end())
	{
		if(iter->second != NULL)
		{
			delete iter->second;
		}
		iter->second = val;
	}
	else
	{
		m_map.insert(std::make_pair<std::string, cfl::Object*>(key, val));
	}
}

StrObjPtrContext::StrObjPtrMap* StrObjPtrContext::GetDataMap()
{
	return &m_map;
}