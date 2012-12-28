// OptionContext.cpp: implementation of the OptionContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionContext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OptionContext::OptionContext()
{

}

OptionContext::~OptionContext()
{

}

bool OptionContext::ContainsKey(const char* key)
{
	ssmap::iterator iter = m_propMap.find(key);
	return iter != m_propMap.end();
}

bool OptionContext::Get(const char* key, std::string& val)
{
	ssmap::iterator iter = m_propMap.find(key);
	if(iter != m_propMap.end())
	{
		val = iter->second;
		return true;
	}
	return false;
}
void OptionContext::Put(const char* key, const char* val)
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

OptionContextW::OptionContextW()
{
	
}

OptionContextW::~OptionContextW()
{
	
}

bool OptionContextW::ContainsKey(const wchar_t* key)
{
	ssmap::iterator iter = m_propMap.find(key);
	return iter != m_propMap.end();
}

bool OptionContextW::Get(const wchar_t* key, std::wstring& val)
{
	ssmap::iterator iter = m_propMap.find(key);
	if(iter != m_propMap.end())
	{
		val = iter->second;
		return true;
	}
	return false;
}
void OptionContextW::Put(const wchar_t* key, const wchar_t* val)
{
	ssmap::iterator iter = m_propMap.find(key);
	if(iter != m_propMap.end())
	{
		iter->second.assign(val);
	}
	else
	{
		m_propMap.insert(std::make_pair<std::wstring, std::wstring>(key, val));
	}
}
