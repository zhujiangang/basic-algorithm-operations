#include "StdAfx.h"
/*
#include "OptionExpDef.h"
#include "cflbase/tstring.h"
#include "log4cplus_config.h"
#include <assert.h>


DECLARE_THE_LOGGER_NAME(_T("lib.OptionExp"))

static int gOptLogLevel = OPT_LL_INFO;

static const log4cplus::LogLevel l4cplus_levels[] = 
{
	log4cplus::FATAL_LOG_LEVEL,
	log4cplus::ERROR_LOG_LEVEL,
	log4cplus::WARN_LOG_LEVEL,
	log4cplus::INFO_LOG_LEVEL,
	log4cplus::DEBUG_LOG_LEVEL,
	log4cplus::TRACE_LOG_LEVEL,
	log4cplus::ALL_LOG_LEVEL,
	log4cplus::OFF_LOG_LEVEL
};

bool opt_msg_enabled(int level)
{
	assert(level >= 0 || level < OPT_ALL_COUNT);
	return IS_LOG_ENABLED(THE_LOGGER, l4cplus_levels[level]);
}

void opt_msg(int level, const char* format, ...)
{
	if(!opt_msg_enabled(level))
	{
		return;
	}
	va_list args;
	va_start(args, format);
	
	std::string str;
	cfl::vformat(str, format, args);
	
	va_end(args);
	
	LOG4CPLUS_WARN_STR(THE_LOGGER, CFL_STRING_TO_TSTRING(str))
}

void opt_msg(const char* format, ...)
{
	if(!opt_msg_enabled(OPT_LL_DEBUG))
	{
		return;
	}
	va_list args;
	va_start(args, format);
	
	std::string str;
	cfl::vformat(str, format, args);
	
	va_end(args);
	
	LOG4CPLUS_DEBUG_STR(THE_LOGGER, CFL_STRING_TO_TSTRING(str))
}
void wopt_msg(int level, const wchar_t* format, ...)
{
	if(!opt_msg_enabled(level))
	{
		return;
	}
	va_list args;
	va_start(args, format);
	
	std::wstring str;
	cfl::wvformat(str, format, args);
	
	va_end(args);
	
	LOG4CPLUS_WARN_STR(THE_LOGGER, CFL_WSTRING_TO_TSTRING(str))
}
void wopt_msg(const wchar_t* format, ...)
{
	if(!opt_msg_enabled(OPT_LL_DEBUG))
	{
		return;
	}
	va_list args;
	va_start(args, format);
	
	std::wstring str;
	cfl::wvformat(str, format, args);
	
	va_end(args);
	
	LOG4CPLUS_DEBUG_STR(THE_LOGGER, CFL_WSTRING_TO_TSTRING(str))
}

*/