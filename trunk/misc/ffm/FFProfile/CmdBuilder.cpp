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