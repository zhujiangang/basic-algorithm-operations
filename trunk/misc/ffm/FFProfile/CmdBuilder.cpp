#include "stdafx.h"
#include "CmdBuilder.h"
#include "OptionExpTree.h"

MeCmdBuilder::MeCmdBuilder()
{
	Reset();
}
void MeCmdBuilder::Reset()
{
	m_szBinFile.erase();
	m_szInput.erase();
	m_szOutput.erase();
	m_pContext = NULL;
	m_pBuilder = NULL;
}

MeCmdBuilder& MeCmdBuilder::SetBinFile(const TCHAR* szBinFile)
{
	m_szBinFile.assign(szBinFile);
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetInput(const TCHAR* szInput)
{
	m_szInput.assign(szInput);
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetOutput(const TCHAR* szOutput)
{
	m_szOutput.assign(szOutput);
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetOptionContext(OptionContext* pContext)
{
	m_pContext = pContext;
	return *this;
}
MeCmdBuilder& MeCmdBuilder::SetOptionExpBuilder(OptionExpBuilder* pOptExpBuilder)
{
	m_pBuilder = pOptExpBuilder;
	return *this;
}

bool MeCmdBuilder::Build(cfl::tstring& szCmdLine)
{
	OptionExpTree tree;
	if(!tree.Build(m_pContext, m_pBuilder))
	{
		return false;
	}
	std::string szOption;
	if(!tree.Evaluate(m_pContext, szOption))
	{
		return false;
	}

	cfl::tformat(szCmdLine, _T("%s %s %s -o %s"), m_szBinFile.c_str(), CFL_STRING_TO_T_STR(szOption), 
		m_szInput.c_str(), m_szOutput.c_str());

	return true;
}