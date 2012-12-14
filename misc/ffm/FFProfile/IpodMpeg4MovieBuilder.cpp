// IpodMpeg4MovieBuilder.cpp: implementation of the CIpodMpeg4MovieBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IpodMpeg4MovieBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIpodMpeg4MovieBuilder::CIpodMpeg4MovieBuilder()
{

}

CIpodMpeg4MovieBuilder::~CIpodMpeg4MovieBuilder()
{

}

bool CIpodMpeg4MovieBuilder::Build(std::string& szCmdLine)
{
	bool done = false;

	szCmdLine.erase();

	std::string szVal;
	do 
	{
		//bin folder
		if(!GetProp(FFP_BIN_DIR, szVal))
		{
			break;
		}
		szCmdLine.append("\"");
		if(!szVal.empty())
		{
			szCmdLine.append(szVal).append(PATH_SEP);
		}

		//bin file
		if(!GetProp(FFP_BIN_NAME, szVal) || szVal.empty())
		{
			break;
		}
		szCmdLine.append(szVal).append("\"");

		//input file
		if(!GetProp(FFP_INPUT_FILE, szVal) || szVal.empty())
		{
			break;
		}
		szCmdLine.append(ARG_SEP).append(szVal);

		//general options: Overwrite output files without asking.
		szCmdLine.append(ARG_SEP).append("y");


		//audio options

		//audio codec: aac
		szCmdLine.append(ARG_SEP).append("-acodec aac");

		//audio bit rate
		if(!GetProp(FFP_AUDIO_BITRATE, szVal) || szVal.empty())
		{
			szVal = "128k";
		}
		szCmdLine.append(ARG_SEP).append("-b:a").append(ARG_SEP).append(szVal);

		//audio sample rate
		if(!GetProp(FFP_AUDIO_SAMPLE_RATE, szVal) || szVal.empty())
		{
			szVal = "44100";
		}
		szCmdLine.append(ARG_SEP).append("-ar").append(ARG_SEP).append(szVal);

		//audio channel
		if(!GetProp(FFP_AUDIO_CHANNEL, szVal) || szVal.empty())
		{
			szVal = "2";
		}
		szCmdLine.append(ARG_SEP).append("-ac").append(ARG_SEP).append(szVal);


		//video options

		//video codec: only x264 supported
		if(!GetProp(FFP_VIDEO_CODEC, szVal) || szVal.empty())
		{
			szVal = "libx264";
		}
		szCmdLine.append(ARG_SEP).append("-vcodec").append(ARG_SEP).append(szVal);

		//video codec options: only x264 supported
		if(!GetProp(FFP_VIDEO_CODEC_OPTIONS, szVal) || szVal.empty())
		{
			szVal = "-preset slow -profile:v high -level 3.1";
		}
		szCmdLine.append(ARG_SEP).append(szVal);

		//frame size
		if(!GetProp(FFP_VIDEO_FRAME_SIZE, szVal) || szVal.empty())
		{
			break;
		}
		szCmdLine.append(ARG_SEP).append("-s").append(ARG_SEP).append(szVal);

		//video bitrate
		if(!GetProp(FFP_VIDEO_BITRATE, szVal) || szVal.empty())
		{
			szVal = "1200k";
		}
		szCmdLine.append(ARG_SEP).append("-b:v").append(ARG_SEP).append(szVal);

		//video frame rate
		if(!GetProp(FFP_VIDEO_FRAME_RATE, szVal) || szVal.empty())
		{
			szVal = "25";
		}
		szCmdLine.append(ARG_SEP).append("-r").append(ARG_SEP).append(szVal);

		//general video options
		szCmdLine.append(ARG_SEP).append("-maxrate 10000k -bufsize 10000k");


		//output options

		//output format
		if(!GetProp(FFP_OUTPUT_FORMAT, szVal) || szVal.empty())
		{
			szVal = "mp4";
		}
		szCmdLine.append(ARG_SEP).append("-f").append(ARG_SEP).append(szVal);

		//output folder
		szCmdLine.append(ARG_SEP);

		if(GetProp(FFP_OUTPUT_DIR, szVal))
		{
			szCmdLine.append(szVal).append(PATH_SEP);
		}
		
		//output file
		if(!GetProp(FFP_OUTPUT_FILE_NAME, szVal) || szVal.empty())
		{
			break;
		}
		szCmdLine.append(szVal);


		//everything is ok
		done = true;

	} while (false);

	return done;
}
