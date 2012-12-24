#ifndef __OPTION_EXP_DEF_H__
#define __OPTION_EXP_DEF_H__

#pragma warning( disable : 4786 )

#include <string>

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

#define FORCE_OF			"ForceOutputFormat"
#define OF					"OutputFormat"
#define OF_OPTS				"__OFOpts__"

#define PASS_LOG_FILE		"PassLogFile"
#define IFILE				"InputFile"
#define OFILE				"OutputFile"

#define VIDEO_BITRATE		"VideoBitrate"
#define AUDIO_BITRATE		"AudioBitrate"
#define MAX_B_FRAMES		"MaxBFrames"
#define PASS				"Pass"

#define ERROR_STR			"__ErrorStr__"

//ALL, FATAL, ERROR, WARN, INFO, DEBUG, TRACE, OFF
enum OptLogLevel
{
	OPT_LL_FATAL	= 100,
	OPT_LL_ERROR	= 200,
	OPT_LL_WARN		= 300,
	OPT_LL_INFO		= 400,
	OPT_LL_DEBUG	= 500,
	OPT_LL_TRACE	= 600,

	OPT_LL_ALL		= 0x7FFFFFFF,
	OPT_LL_OFF		= 0x80000000
};

extern int gOptLogLevel;

bool IsOptLogEnabled(int level);
void opt_msg(const char* format, ...);
void opt_msg(int level, const char* format, ...);

void wopt_msg(const wchar_t* format, ...);
void wopt_msg(int level, const wchar_t* format, ...);

#if defined(_UNICODE) || defined(UNICODE)
#define topt_msg wopt_msg
#else
#define topt_msg opt_msg
#endif

bool ParseSize(const std::string& str, int& w, int& h);
const char* SafeStr(const char* str);
const char* SafePStr(const std::string* pstr);

#endif