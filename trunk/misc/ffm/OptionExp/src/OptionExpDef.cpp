#include "StdAfx.h"
#include "OptionExpDef.h"
#include "cflbase/tstring.h"

int gOptLogLevel = OPT_LL_INFO;

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

bool IsOptLogEnabled(int level)
{
	return level <= gOptLogLevel;
}

void opt_msg(int level, const char* format, ...)
{
	if(level > gOptLogLevel)
	{
		return;
	}
	va_list args;
	va_start(args, format);
	
	std::string str;
	cfl::vformat(str, format, args);
	
	va_end(args);
	
	AfxTrace(_T("%s"), CFL_STRING_TO_T_STR(str));
}

void opt_msg(const char* format, ...)
{
	if(OPT_LL_DEBUG > gOptLogLevel)
	{
		return;
	}
	va_list args;
	va_start(args, format);
	
	std::string str;
	cfl::vformat(str, format, args);
	
	va_end(args);
	
	AfxTrace(_T("%s"), CFL_STRING_TO_T_STR(str));
}
void wopt_msg(int level, const wchar_t* format, ...)
{
	if(level > gOptLogLevel)
	{
		return;
	}
	va_list args;
	va_start(args, format);
	
	std::wstring str;
	cfl::wvformat(str, format, args);
	
	va_end(args);
	
	AfxTrace(_T("%s"), CFL_WSTRING_TO_T_STR(str));
}
void wopt_msg(const wchar_t* format, ...)
{
	if(OPT_LL_DEBUG > gOptLogLevel)
	{
		return;
	}
	va_list args;
	va_start(args, format);
	
	std::wstring str;
	cfl::wvformat(str, format, args);
	
	va_end(args);
	
	AfxTrace(_T("%s"), CFL_WSTRING_TO_T_STR(str));
}

const char* SafeStr(const char* str)
{
	return (str == NULL) ? "NULL" : str;
}
const char* SafePStr(const std::string* pstr)
{
	return (pstr == NULL) ? "NULL" : pstr->c_str();
}
