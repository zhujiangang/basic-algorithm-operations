#include "stdafx.h"
#include "OptionExpUtils.h"
#include <shlwapi.h>
#include "cflbase/FileUtils.h"
#include "cflbase/tstring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool OptionExpUtils::BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileExt, 
									   const char* szSrcFile, bool bHasQuote)
{
	std::string szTemp;
	cfl::FileUtilsA::GetFileNameOnly(szTemp, szSrcFile);
	
	if(szOutputFolder != NULL && szOutputFolder[0] != '\0')
	{
		cfl::format(str, "%s\\%s%s", szOutputFolder, szTemp.c_str(), szOutputFileExt);
		int nCount = 0;
		while(::PathFileExistsA(str.c_str()))
		{
			nCount++;
			cfl::format(str, "%s\\%s_%d%s", szOutputFolder, szTemp.c_str(), nCount, szOutputFileExt);
		}
	}
	else
	{
		cfl::format(str, "%s%s", szTemp.c_str(), szOutputFileExt);
		int nCount = 0;
		while(::PathFileExistsA(str.c_str()))
		{
			nCount++;
			cfl::format(str, "%s_%d%s", szTemp.c_str(), nCount, szOutputFileExt);
		}
	}
	cfl::format(str, "\"%s\"", str.c_str());
	return true;
}

bool OptionExpUtils::BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileName, 
									   bool bHasQuote)
{
	if(szOutputFolder != NULL && szOutputFolder[0] != '\0')
	{
		const char* fmt = bHasQuote ? "\"%s\\%s\"" : "%s\\%s";
		cfl::format(str, fmt, szOutputFolder, szOutputFileName);
	}
	else
	{
		const char* fmt = bHasQuote ? "\"%s\"" : "%s";
		cfl::format(str, fmt, szOutputFileName);
	}
	return true;
}

const char* SafeStrA(const char* str)
{
	return (str == NULL) ? "" : str;
}
const wchar_t* SafeStrW(const wchar_t* str)
{
	return (str == NULL) ? L"" : str;
}

const char* SafePStrA(const std::string* pstr)
{
	return (pstr == NULL) ? "" : pstr->c_str();
}
const wchar_t* SafePStrW(const std::wstring* pstr)
{
	return (pstr == NULL) ? L"" : pstr->c_str();
}


