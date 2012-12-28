// SysUtils.h: interface for the SysUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSUTILS_H__BF63CF03_7C25_4B87_B8A1_3321C60D6CD5__INCLUDED_)
#define AFX_SYSUTILS_H__BF63CF03_7C25_4B87_B8A1_3321C60D6CD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SysUtils  
{
public:

	static bool BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileExt, 
		const char* szSrcFile, bool bHasQuote = true);
	static bool BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileName, 
		bool bHasQuote = true);

private:
	SysUtils();
	virtual ~SysUtils();
};

#endif // !defined(AFX_SYSUTILS_H__BF63CF03_7C25_4B87_B8A1_3321C60D6CD5__INCLUDED_)
