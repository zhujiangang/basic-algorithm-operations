#ifndef __CMD_BUILDER_H__
#define __CMD_BUILDER_H__

#pragma once

#pragma warning( disable : 4786 )

#include "cflbase/tstring.h"
#include "cflbase/Object.h"
#include <vector>
#include <map>

enum StreamTrackEnum
{
	VIDEO = 0,
	AUDIO
};

class CmdInfo
{
public:
	cfl::tstring	m_szBinFQPath;
	cfl::tstring	m_szInputFQPath;
	cfl::tstring	m_szOutputFQPath;
	cfl::tstring	m_szCmdLine;

public:
	void Clear();
};

class StrObjPtrContext
{
public:
	typedef std::map<std::string, cfl::Object*> StrObjPtrMap;

	StrObjPtrContext();
	virtual ~StrObjPtrContext();
	virtual bool Get(const std::string& key, cfl::Object** pval);
	virtual void Put(const std::string& key, cfl::Object* val);
	virtual StrObjPtrMap* GetDataMap();
protected:
	StrObjPtrMap m_map;
};

template<class T>
class OptionExpObj : public cfl::Object
{
public:
	OptionExpObj() {}
	virtual ~OptionExpObj() {}
	virtual void* GetData() {return &m_data;}
protected:
	T m_data;
};

typedef std::vector<cfl::tstring> TStrVector;
typedef OptionExpObj< TStrVector > TStrVectorObj;


class CmdBuilder
{
public:
	CmdBuilder() {}
	virtual ~CmdBuilder() {}
//	virtual bool Build(cfl::tstring& szCmdLine, cfl::tstring* pszOutputFile = NULL) { return false; }
//	virtual bool Build(std::vector<CmdInfo>& commands, std::vector<cfl::tstring>& delFiles) = 0;

	virtual bool Build(CmdInfo& cmdInfo, StrObjPtrContext& context) = 0;
};

#endif