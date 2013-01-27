#ifndef __OPTION_EXP_DEF_H__
#define __OPTION_EXP_DEF_H__

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
#define VIDEO_SIZE			"VideoSize"
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
#define PASS_COUNT			"PassCount"

#define ASPECT				"Aspect"
#define KEYINT				"KeyInt"

//PARAM Names
#define PARAM_DEL_LIST		"param.dellist"		/* Temporary File List */
#define PARAM_OF			"param.out.format"	/* Output Format or Container Type*/
#define PARAM_OUT_EXT		"param.out.ext"		/* Output File Extension. e.g. <.mp4> */
#define PARAM_OPT_CXT		"param.option.context"

#define PARAM_BIN_MENCODER	"param.bin.mencoder"
#define PARAM_BIN_MPLAYER	"param.bin.mplayer"
#define PARAM_BIN_MP4CREATOR	"param.bin.mp4creator"
#define PARAM_INPUT_FILE	"param.input.file"
#define PARAM_OUTPUT_FOLDER	"param.output.folder"

/*
//ALL, FATAL, ERROR, WARN, INFO, DEBUG, TRACE, OFF
enum OptLogLevel
{
	OPT_LL_FATAL	= 0,
	OPT_LL_ERROR	= 1,
	OPT_LL_WARN		= 2,
	OPT_LL_INFO		= 3,
	OPT_LL_DEBUG	= 4,
	OPT_LL_TRACE	= 5,

	OPT_LL_ALL		= 6,
	OPT_LL_OFF		= 7,

	OPT_ALL_COUNT
};

bool opt_msg_enabled(int level);

void opt_msg(const char* format, ...);
void opt_msg(int level, const char* format, ...);

void wopt_msg(const wchar_t* format, ...);
void wopt_msg(int level, const wchar_t* format, ...);

#if defined(_UNICODE) || defined(UNICODE)
#define topt_msg wopt_msg
#else
#define topt_msg opt_msg
#endif
*/

#endif