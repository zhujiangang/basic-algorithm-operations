#ifndef __CMD_BUILDER_H__
#define __CMD_BUILDER_H__

#pragma once

#pragma warning( disable : 4786 )

#include <string>
#include <map>

#ifndef PATH_SEP
#define PATH_SEP	"\\"
#endif

#ifndef ARG_SEP
#define ARG_SEP		" "
#endif

//-lavcopts vcodec=mpeg4:mbd=2:trell:autoaspect:vbitrate=512:vhq
#ifndef PROP_SEP
#define PROP_SEP	"="
#endif

//-lavcopts vcodec=mpeg4:mbd=2:trell:autoaspect:vbitrate=512:vhq
#ifndef OPTION_SEP
#define OPTION_SEP	":"
#endif

//-vf-add scale=480:272,harddup
#ifndef	FILTER_SEP
#define FILTER_SEP	","
#endif

#define SEEK_TIME			"SeekTime"
#define END_TIME			"EndTime"
#define MAX_AV_SYNC			"MaxAVSync"
#define NO_SKIP				"NoSkip"
#define SWS					"SWS"
#define AUDIO_FILTER		"AudioFilter"
#define VIDEO_FILTER		"VideoFilter"
#define VIDEO_SIZE			"Video Size"
#define VIDEO_WIDTH			"VideoWidth"
#define VIDEO_HEIGHT		"VideoHeight"
#define OSRATE				"OutputSampleRate"
#define OFPS				"OFPS"
#define OAC					"OAC"
#define OAC_OPTS			"__OACOpts__"
#define OVC					"OVC"
#define OVC_OPTS			"__OVCOpts__"
#define FOURCC				"FourCC"

#define OUT_FORMAT			"OutputFormat"
#define OF_OPTS				"__OFOpts__"

#define OFILE				"OutputFile"

#define VIDEO_BITRATE		"VideoBitrate"
#define AUDIO_BITRATE		"AudioBitrate"
#define MAX_B_FRAMES		"MaxBFrames"

typedef std::map< std::string, std::string > ssmap;

/*
* A name / value pair parameter
*/
class NameValuePair
{
public:
	std::string szName;
	std::string szValue;

public:
	NameValuePair(const char* name = NULL, const char* value = NULL)
	{
		if(name)
		{
			szName.assign(name);
		}
		if(value)
		{
			szValue.assign(value);
		}
	}
	void ToString(std::string& str)
	{
		str.assign(szName);
		if(!szValue.empty())
		{
			str.append(PROP_SEP).append(szValue);
		}
	}
	std::string ToString()
	{
		std::string str;
		ToString(str);
		return str;
	}
};

enum CmdOptionFlag
{
	//Default command option
	COF_DEFAULT			= 0x0000,

	//Optional command option
	COF_OPTIONAL		= 0x0001,

	//Flag type command option.
	//e.g. "-nosound" 
	COF_FLAG_TYPE		= 0x0002,

	COF_INTERNAL_SET	= 0x0004,

	COF_SET_PROP		= 0x0008,

	COF_SUB_SET			= 0x0010
};

class CmdParam
{
public:
	const char* szCmdOption;
	int			nOptionFlag;
	const char* szName;
	const char* szDefaultValue;
};

class CodecOptsCmdParam
{
public:
	const char*		szCodecName;
	const char*		szOVC;
	const char*		szCodecOptsCmdOption;
	const CmdParam*	pOptsParams;
	int				nOptsParamCount;
};

class FilterParam
{
public:
	int			nOptionFlag;
	const char* szName;
	const char* szDefaultValue;
};

class FilterOptsCmdParam
{
public:
	const char*			szFilterName;
	const FilterParam*	pFilterParams;
	int					nFilterparamsCount;
};

class PropMap
{
public:
	PropMap();
	virtual ~PropMap();
	virtual bool ContainsKey(const char* key);
	virtual bool GetProp(const char* key, std::string& val);
	virtual void PutProp(const char* key, const char* val);

protected:
	ssmap	m_propMap;
};

class CmdBuilder
{
public:
	CmdBuilder() {}
	virtual ~CmdBuilder() {}
	virtual bool Build(std::string& szCmdLine) = 0;
};

bool ParseSize(const std::string& str, int& w, int& h);

#endif