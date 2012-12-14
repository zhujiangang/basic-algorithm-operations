// FFCmdBuilder.h: interface for the FFCmdBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FFCMDBUILDER_H__D3319370_8C32_47EA_B6FE_6A0EE5BEDBC2__INCLUDED_)
#define AFX_FFCMDBUILDER_H__D3319370_8C32_47EA_B6FE_6A0EE5BEDBC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )

#include <map>
#include <string>

typedef std::map< int, std::string > ismap;
typedef std::map< std::string, std::string > ssmap;

enum FFParamEnum
{
	FFP_FIRST = 0,
	FFP_BIN_DIR = 0,
	FFP_BIN_NAME,
	FFP_INPUT_FILE,

	FFP_AUDIO_CODEC,
	FFP_AUDIO_CODEC_OPTIONS,
	FFP_AUDIO_BITRATE,
	FFP_AUDIO_SAMPLE_RATE,
	FFP_AUDIO_CHANNEL,

	FFP_VIDEO_CODEC,
	FFP_VIDEO_CODEC_OPTIONS,
	FFP_VIDEO_FRAME_SIZE,
	FFP_VIDEO_BITRATE,
	FFP_VIDEO_FRAME_RATE,

	FFP_OUTPUT_FORMAT,
	FFP_OUTPUT_DIR,
	FFP_OUTPUT_FILE_NAME,
	FFP_COUNT
};

class FFCmdBuilder  
{
public:
	FFCmdBuilder();
	virtual ~FFCmdBuilder();

	virtual bool Build(std::string& szCmdLine);

	virtual bool ContainsKey(int key);
	virtual bool GetProp(int key, std::string& szVal);

protected:
	ismap	m_propMap;

	static const char* PATH_SEP;
	static const char* ARG_SEP;
};

#endif // !defined(AFX_FFCMDBUILDER_H__D3319370_8C32_47EA_B6FE_6A0EE5BEDBC2__INCLUDED_)
