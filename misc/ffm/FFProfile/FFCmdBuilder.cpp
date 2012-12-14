// FFCmdBuilder.cpp: implementation of the FFCmdBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FFCmdBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char* FFCmdBuilder::PATH_SEP = "\\";
const char* FFCmdBuilder::ARG_SEP = " ";

FFCmdBuilder::FFCmdBuilder()
{

}

FFCmdBuilder::~FFCmdBuilder()
{

}

bool FFCmdBuilder::Build(std::string& szCmdLine)
{
	return false;
}

bool FFCmdBuilder::ContainsKey(int key)
{
	ismap::iterator iter = m_propMap.find(key);
	return iter != m_propMap.end();
}

bool FFCmdBuilder::GetProp(int key, std::string& szVal)
{
	ismap::iterator iter = m_propMap.find(key);
	if(iter != m_propMap.end())
	{
		szVal = iter->second;
		return true;
	}
	return false;
}