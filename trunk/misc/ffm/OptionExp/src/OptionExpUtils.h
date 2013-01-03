#if !defined(AFX_SYSUTILS_H__BF63CF03_7C25_4B87_B8A1_3321C60D6CD5__INCLUDED_)
#define AFX_SYSUTILS_H__BF63CF03_7C25_4B87_B8A1_3321C60D6CD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )

#include <string>

class OptionExpUtils  
{
public:
	static bool BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileExt, 
		const char* szSrcFile, bool bHasQuote = true);
	static bool BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileName, 
		bool bHasQuote = true);
};

const char* SafeStrA(const char* str);
const wchar_t* SafeStrW(const wchar_t* str);

const char* SafePStrA(const std::string* pstr);
const wchar_t* SafePStrW(const std::wstring* str);

#if defined(UNICODE) || defined(_UNICODE)
#define SafeStr SafeStrW
#define SafePStr SafePStrW
#else
#define SafeStr SafeStrA
#define SafePStr SafePStrA
#endif

#endif // !defined(AFX_SYSUTILS_H__BF63CF03_7C25_4B87_B8A1_3321C60D6CD5__INCLUDED_)
