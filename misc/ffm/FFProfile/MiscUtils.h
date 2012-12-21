// MiscUtils.h: interface for the MiscUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISCUTILS_H__07923961_4C7C_4A13_B383_1B360615BEF9__INCLUDED_)
#define AFX_MISCUTILS_H__07923961_4C7C_4A13_B383_1B360615BEF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MiscUtils  
{
public:
	MiscUtils();
	virtual ~MiscUtils();

	static bool BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileExt, 
		const char* szSrcFile, bool bHasQuote = true);
	static bool BuildFullFilePath(std::string& str, const char* szOutputFolder, const char* szOutputFileName, 
		bool bHasQuote = true);
};

#endif // !defined(AFX_MISCUTILS_H__07923961_4C7C_4A13_B383_1B360615BEF9__INCLUDED_)
