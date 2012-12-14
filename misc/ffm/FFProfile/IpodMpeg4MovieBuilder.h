// IpodMpeg4MovieBuilder.h: interface for the CIpodMpeg4MovieBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPODMPEG4MOVIEBUILDER_H__EE03CAC8_F65C_4372_9B0E_62F17F0EF67C__INCLUDED_)
#define AFX_IPODMPEG4MOVIEBUILDER_H__EE03CAC8_F65C_4372_9B0E_62F17F0EF67C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FFCmdBuilder.h"

class CIpodMpeg4MovieBuilder : public FFCmdBuilder  
{
public:
	CIpodMpeg4MovieBuilder();
	virtual ~CIpodMpeg4MovieBuilder();
	virtual bool Build(std::string& szCmdLine);
};

#endif // !defined(AFX_IPODMPEG4MOVIEBUILDER_H__EE03CAC8_F65C_4372_9B0E_62F17F0EF67C__INCLUDED_)
