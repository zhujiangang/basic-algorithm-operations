#ifndef __CMD_BUILDER_H__
#define __CMD_BUILDER_H__

#pragma once

#include "cflbase/tstring.h"
#include "OptionContext.h"
#include "OptionExp.h"

class CmdBuilder
{
public:
	CmdBuilder() {}
	virtual ~CmdBuilder() {}
	virtual bool Build(cfl::tstring& szCmdLine) = 0;
};

class MeCmdBuilder : public CmdBuilder
{
public:
	MeCmdBuilder();
	virtual bool Build(cfl::tstring& szCmdLine);

	virtual MeCmdBuilder& SetBinFile(const TCHAR* szBinFile);
	virtual MeCmdBuilder& SetInput(const TCHAR* szInput);
	virtual MeCmdBuilder& SetOutput(const TCHAR* szOutput);
	virtual MeCmdBuilder& SetOptionContext(OptionContext* pContext);
	virtual MeCmdBuilder& SetOptionExpBuilder(OptionExpBuilder* pOptExpBuilder);

protected:
	void Reset();

	cfl::tstring m_szBinFile;
	cfl::tstring m_szInput;
	cfl::tstring m_szOutput;
	OptionContext* m_pContext;
	OptionExpBuilder* m_pBuilder;
};

#endif