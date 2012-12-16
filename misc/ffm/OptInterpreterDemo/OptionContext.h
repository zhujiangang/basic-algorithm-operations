// OptionContext.h: interface for the OptionContext class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONCONTEXT_H__F412A6C4_C4BA_4E75_9A01_74F32E07B3B9__INCLUDED_)
#define AFX_OPTIONCONTEXT_H__F412A6C4_C4BA_4E75_9A01_74F32E07B3B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>

class OptionContext  
{
public:
	OptionContext();
	virtual ~OptionContext();

	virtual bool ContainsKey(const char* key);
	virtual bool Get(const char* key, std::string& val);
	virtual void Put(const char* key, const char* val);

protected:
	typedef std::map< std::string, std::string > ssmap;
	ssmap	m_propMap;
};

#endif // !defined(AFX_OPTIONCONTEXT_H__F412A6C4_C4BA_4E75_9A01_74F32E07B3B9__INCLUDED_)
