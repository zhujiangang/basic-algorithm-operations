#include "StdAfx.h"
#include "DefaultOptionExpBuilder.h"
#include "DefaultOptionExp.h"
#include "cflbase/tstring.h"
#include "SysUtils.h"
#include "OptionExpDef.h"
#include "log4cplus_config.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static bool AddNextSibling(OptionExp* pOptExp, OptionExp* pSibing)
{
	//add sibling at next
	OptionExp* pParent = pOptExp->GetParent();
	if(pParent == NULL)
	{
		return false;
	}
	int nIndex = pParent->GetIndex(pOptExp);
	ASSERT(nIndex >= 0);
	if(nIndex < 0)
	{
		return false;
	}
	pParent->AddChild(pSibing, nIndex + 1);
	return true;
}

static OptionExp* GetRoot(OptionExp* pOptExp)
{
	OptionExp *pNode = pOptExp;
	while(pNode != NULL)
	{
		if(pNode->GetParent() == NULL)
		{
			return pNode;
		}
		pNode = pNode->GetParent();
	}
	return NULL;
}

static OptionExp* GetNonRootAncestor(OptionExp* pOptExp)
{
	OptionExp *pNode = pOptExp, *pParent = pOptExp->GetParent();
	while(pParent != NULL)
	{
		if(pParent->GetParent() == NULL)
		{
			return pNode;
		}
		pNode = pParent;
		pParent = pParent->GetParent();
	}
	return NULL;
}

static OptionExp* FindChild(OptionExp* pOptExp, const char* name)
{
	const char* pChildName;
	OptionExp* pChild;
	for(int i = pOptExp->GetChildCount() - 1; i >= 0; i--)
	{
		pChild = pOptExp->GetChild(i);
		pChildName = pChild->GetFieldStr(OPT_FIELD_NAME);
		if(pChildName != NULL && strcmp(pChildName, name) == 0)
		{
			return pChild;
		}
	}
	return NULL;
}

static OptionExp* FindSibling(OptionExp* pOptExp, const char* name)
{
	OptionExp* pParent = pOptExp->GetParent();
	if(pParent == NULL)
	{
		return NULL;
	}
	return FindChild(pParent, name);
}

static bool PassFuncSet(OptionExp* pOptExp, OptionContext* pContext, const std::string* pVal)
{
	if(IS_LOG_ENABLED(THE_LIB_LOGGER, log4cplus::TRACE_LOG_LEVEL))
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("PassFuncSet called: pval = %s, name=%s"),
			CFL_A2T(SafePStrA(pVal)), CFL_A2T(SafePStrA(pOptExp->GetField(OPT_FIELD_NAME))));
		LOG4CPLUS_TRACE_STR(THE_LIB_LOGGER, szLog)
	}
	//only work for pass 1
	if(pVal == NULL || pVal->compare(0, 1, "1") != 0)
	{
		return false;
	}

	if(FindSibling(pOptExp, "turbo") == NULL)
	{
		//add sibling "turbo" at next
		DefaultOptionExp* pSibling = new DefaultOptionExp();
		pSibling->SetOptionName("turbo").SetOptionValue("");
		pSibling->SetEvaluateMode(OPTEM_NAME_ONLY).SetEvaluateFlag(OPTEF_SELF);
		if(!AddNextSibling(pOptExp, pSibling))
		{
			delete pSibling;
			return false;
		}
	}
	
	OptionExp* pAncestor = GetNonRootAncestor(pOptExp);
	if(pAncestor != NULL && (FindSibling(pAncestor, "-nosound") == NULL))
	{
		//add sibling "-nosound" at global options
		DefaultOptionExp* pSibling = new DefaultOptionExp();
		pSibling->SetOptionName("-nosound").SetOptionValue("");
		pSibling->SetEvaluateMode(OPTEM_NAME_ONLY).SetEvaluateFlag(OPTEF_SELF);
		if(!AddNextSibling(pAncestor, pSibling))
		{
			delete pSibling;
			return false;
		}
	}
	return true;
}

static const OptionParam xvidParam1[] = 
{
	{VIDEO_BITRATE,	"bitrate",		"687",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE | OPTEF_MUST,	NULL},
	{MAX_B_FRAMES,	"max_bframes",	"0",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{PASS,			"pass",			NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"xvid",			NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),	OPTEF_NONE,			NULL},

	{OVC_OPTS,		"-xvidencopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam x264Param1[] = 
{
	{VIDEO_BITRATE,	"bitrate",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE | OPTEF_MUST,	NULL},
	{NULL,			"subq",		"6",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"ref",		"2",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"bframes",	"0",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"level_idc","30",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"nocabac",	"",			NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{NULL,			"no8x8dct",	"",			NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{NULL,			"me",		"umh",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"trellis",	"0",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"weightp",	"0",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{PASS,			"pass",		NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,	PassFuncSet},
	{FOURCC,		"-ffourcc",	"H264",		NULL,		NULL,		MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_VALUE, CONTEXT_VALUE),	OPTEF_NONE,			NULL},
	{OVC,			"x264",		NULL,		NULL,		NULL,		MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,			NULL},
	{OVC_OPTS,		"-x264encopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam mpeg4Param1[] = 
{
	{NULL,			"vcodec",	"mpeg4",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE | OPTEF_MUST,	NULL},
	{NULL,			"mbd",		"2",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"trell",	"",			NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{PASS,			"vpass",		NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,	PassFuncSet},
	{FOURCC,		NULL,		"DIVX",		NULL,		NULL,		MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_VALUE, CONTEXT_VALUE),	OPTEF_NONE,			NULL},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_mpeg2video_opts[] = 
{
	{NULL,			"vcodec",	"mpeg2video",PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE | OPTEF_MUST,	NULL},
	{NULL,			"mbd",		"2",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"trell",	"",			NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{NULL,			"keyint",	"15",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"aspect",	"4/3",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"vrc_buf_size",	"1835",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"vrc_maxrate",	"9800",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"vstrict",	"0",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_mpeg1video_opts[] = 
{
	{NULL,			"vcodec",	"mpeg1video",PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE | OPTEF_MUST,	NULL},
	{NULL,			"mbd",		"2",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"trell",	"",			NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{NULL,			"keyint",	"15",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"aspect",	"4/3",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"vrc_buf_size",	"327",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"vrc_maxrate",	"1152",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"vstrict",	"-1",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_ffvhuff_opts[] = 
{
	{NULL,			"vcodec",	"ffvhuff",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"format",	"422p",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"pred",		"2",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"context",	"1",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"vstrict",	"-1",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_ffv1_opts[] = 
{
	{NULL,			"vcodec",	"ffv1",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"format",	"422p",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"coder",	"0",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"context",	"1",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_h263p_opts[] = 
{
	{NULL,			"vcodec",	"h263p",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_msmpeg4_opts[] = 
{
	{NULL,			"vcodec",	"msmpeg4",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_msmpeg4v2_opts[] = 
{
	{NULL,			"vcodec",	"msmpeg4v2",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_mjpeg_opts[] = 
{
	{NULL,			"vcodec",	"mjpeg",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_wmv1_opts[] = 
{
	{NULL,			"vcodec",	"wmv1",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_wmv2_opts[] = 
{
	{NULL,			"vcodec",	"wmv2",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_libvpx_opts[] = 
{
	{NULL,			"vcodec",	"libvpx",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{FOURCC,		NULL,		"VP80",		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_VALUE, CONTEXT_VALUE),	OPTEF_NONE,			NULL},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const OptionParam ovc_flv_opts[] = 
{
	{NULL,			"vcodec",	"flv",	PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{VIDEO_BITRATE,	"vbitrate",	"800",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{NULL,			"mbd",		"2",		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NULL,			"trell",	"",			NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{NULL,			"autoaspect","",		NULL,		NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{PASS,			"vpass",	NULL,		PROP_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT | OPTEF_HAS_FUNC | OPTEF_FUNC_ONCE,PassFuncSet},
	{OVC,			"lavc",		NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE),		OPTEF_NONE,		NULL},
	{OVC_OPTS,		"-lavcopts",	NULL,		NULL,		NULL,		
	MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME),	OPTEF_NONE,			NULL}
};

static const NamedOptionParamList ovc_opts_groups[] = 
{
	{"xvid",	xvidParam1, ARRAY_LEN(xvidParam1)},
	{"x264",	x264Param1, ARRAY_LEN(x264Param1)},
	{"mpeg4",	mpeg4Param1, ARRAY_LEN(mpeg4Param1)},
	{"mpeg2video",	ovc_mpeg2video_opts, ARRAY_LEN(ovc_mpeg2video_opts)},
	{"mpeg1video",	ovc_mpeg1video_opts, ARRAY_LEN(ovc_mpeg1video_opts)},
	{"ffvhuff",	ovc_ffvhuff_opts, ARRAY_LEN(ovc_ffvhuff_opts)},
	{"fflv1",	ovc_ffv1_opts, ARRAY_LEN(ovc_ffv1_opts)},
	{"h263p",	ovc_h263p_opts, ARRAY_LEN(ovc_h263p_opts)},
	{"msmpeg4",	ovc_msmpeg4_opts, ARRAY_LEN(ovc_msmpeg4_opts)},
	{"msmpeg4v2",	ovc_msmpeg4v2_opts, ARRAY_LEN(ovc_msmpeg4v2_opts)},
	{"mjpeg",	ovc_mjpeg_opts, ARRAY_LEN(ovc_mjpeg_opts)},
	{"wmv1",	ovc_wmv1_opts, ARRAY_LEN(ovc_wmv1_opts)},
	{"wmv2",	ovc_wmv2_opts, ARRAY_LEN(ovc_wmv2_opts)},
	{"libvpx",	ovc_libvpx_opts, ARRAY_LEN(ovc_libvpx_opts)},
	{"flv",	ovc_flv_opts, ARRAY_LEN(ovc_flv_opts)}
};

static const ChoiceOptionParamList ovc_opts_choices =
{
	SOT_CHOICE,
	OVC,
	ovc_opts_groups,
	ARRAY_LEN(ovc_opts_groups)
};

//-oac faac -faacopts br=128:mpeg=4:object=2
static const OptionParam oac_aac_opt_param[] = 
{
	{AUDIO_BITRATE, "br",	"128", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPT_CONFIGURABLE, NULL},
	{NULL, "mpeg",	"4", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{NULL, "object","2", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{OAC,  "faac",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OAC_OPTS,	"-faacopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

//-oac mp3lame -lameopts abr:br=128
static const OptionParam mp3OptsBase[] = 
{
	{AUDIO_BITRATE, "br",	"128", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPT_CONFIGURABLE, NULL},
	{NULL, "abr",	"", 	NULL, NULL, OPTEM_NAME_ONLY, OPTEF_SELF, NULL},
	{OAC,  "mp3lame",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OAC_OPTS,	"-lameopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

//-oac lavc -lavcopts acodec=ac3:abitrate=128
static const OptionParam ac3OptsBase[] = 
{
	{NULL, "acodec",	"ac3", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{AUDIO_BITRATE, "abitrate",	"128", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPT_CONFIGURABLE, NULL},
	{OAC,  "lavc",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OAC_OPTS,	"-lavcopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

//-oac lavc -lavcopts acodec=mp2:abitrate=128 
static const OptionParam mp2OptsBase[] = 
{
	{NULL, "acodec",	"mp2", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{AUDIO_BITRATE, "abitrate",	"128", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPT_CONFIGURABLE, NULL},
	{OAC,  "lavc",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OAC_OPTS,	"-lavcopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

//-oac lavc -lavcopts acodec=wmav1:abitrate=128 
static const OptionParam wmav1OptsBase[] = 
{
	{NULL, "acodec",	"wmav1", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{AUDIO_BITRATE, "abitrate",	"128", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPT_CONFIGURABLE, NULL},
	{OAC,  "lavc",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OAC_OPTS,	"-lavcopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

//-oac lavc -lavcopts acodec=wmav1:abitrate=128 
static const OptionParam wmav2OptsBase[] = 
{
	{NULL, "acodec",	"wmav2", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{AUDIO_BITRATE, "abitrate",	"128", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPT_CONFIGURABLE, NULL},
	{OAC,  "lavc",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OAC_OPTS,	"-lavcopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

static const NamedOptionParamList oac_opts_groups[] = 
{
	{"aac",	oac_aac_opt_param, ARRAY_LEN(oac_aac_opt_param)},
	{"faac",	oac_aac_opt_param, ARRAY_LEN(oac_aac_opt_param)},
	{"mp3",	mp3OptsBase, ARRAY_LEN(mp3OptsBase)},
	{"ac3",	ac3OptsBase, ARRAY_LEN(ac3OptsBase)},
	{"mp2",	mp2OptsBase, ARRAY_LEN(mp2OptsBase)},
	{"wmav1",	wmav1OptsBase, ARRAY_LEN(wmav1OptsBase)},
	{"wmav2",	wmav2OptsBase, ARRAY_LEN(wmav2OptsBase)},
	{"pcm",	NULL, 0}
};

static const ChoiceOptionParamList oac_opts_choices =
{
	SOT_CHOICE,
	OAC,
	oac_opts_groups,
	ARRAY_LEN(oac_opts_groups)
};

//-of mpeg -mpegopts format=dvd:tsaf:telecine -ofps 24000/1001
static const OptionParam of_mpeg_dvd_film2NTSC_opts[] = 
{
	{NULL, "format","dvd", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{NULL, "tsaf",	"", 	PROP_SEP, NULL, OPTEM_NAME_ONLY, OPTEF_SELF, NULL},
	{NULL, "telecine","", 	PROP_SEP, NULL, OPTEM_NAME_ONLY, OPTEF_SELF, NULL}, /*24000/1001  fps required*/
	{OFPS, "24000/1001",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OF,  "mpeg",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OF_OPTS,	"-mpegopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

//-mpegopts format=dvd:tsaf:film2pal -ofps 24000/1001
static const OptionParam of_mpeg_dvd_film2PAL_opts[] = 
{
	{NULL, "format","dvd", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{NULL, "tsaf",	"", 	PROP_SEP, NULL, OPTEM_NAME_ONLY, OPTEF_SELF, NULL},
	{NULL, "film2pal","", 	PROP_SEP, NULL, OPTEM_NAME_ONLY, OPTEF_SELF, NULL}, /*24000/1001  fps required*/
	{OFPS, "24000/1001",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OF,  "mpeg",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OF_OPTS,	"-mpegopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

//-of lavf -lavfopts format=avi
static const OptionParam of_avi_opts[] = 
{
	{NULL, "format","avi", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{OF,  "lavf",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OAC_OPTS,	"-lavfopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

//-of lavf -lavfopts format=asf
static const OptionParam of_asf_opts[] = 
{
	{NULL, "format","asf", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{OF,  "lavf",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OAC_OPTS,	"-lavfopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

//-of lavf -lavfopts format=matroska
static const OptionParam of_matroska_opts[] = 
{
	{NULL, "format","matroska", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{OF,  "lavf",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OAC_OPTS,	"-lavfopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

//-of lavf -lavfopts format=flv
static const OptionParam of_flv_opts[] = 
{
	{NULL, "format","flv", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{OF,  "lavf",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OAC_OPTS,	"-lavfopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

//-of lavf -lavfopts format=swf
static const OptionParam of_swf_opts[] = 
{
	{NULL, "format","swf", 	PROP_SEP, NULL, OPTEM_DEFAULT, OPTEF_SELF, NULL},
	{OF,  "lavf",	NULL, 	NULL, NULL, MAKE_OPT_MODE(OPTEM_SET_CONTEXT, OPT_FIELD_NAME, CONTEXT_VALUE), OPTEF_NONE, NULL},
	{OAC_OPTS,	"-lavfopts",NULL, NULL,	NULL, MAKE_OPT_MODE(OPTEM_SET_PARENT, OPT_FIELD_NAME, OPT_FIELD_NAME), OPTEF_NONE, NULL}
};

static const NamedOptionParamList of_opts_groups[] = 
{
	{"dvd_film2NTSC",	of_mpeg_dvd_film2NTSC_opts, ARRAY_LEN(of_mpeg_dvd_film2NTSC_opts)},
	{"dvd_film2PAL",	of_mpeg_dvd_film2PAL_opts, ARRAY_LEN(of_mpeg_dvd_film2PAL_opts)},
	{"avi",	of_avi_opts, ARRAY_LEN(of_avi_opts)},
	{"asf",	of_asf_opts, ARRAY_LEN(of_asf_opts)},
	{"matroska",	of_matroska_opts, ARRAY_LEN(of_matroska_opts)},
	{"flv",	of_flv_opts, ARRAY_LEN(of_flv_opts)},
	{"swf",	of_swf_opts, ARRAY_LEN(of_swf_opts)}
};

static const ChoiceOptionParamList of_opts_choices =
{
	SOT_CHOICE,
	FORCE_OF,
	of_opts_groups,
	ARRAY_LEN(of_opts_groups)
};


//-vf scale=720:-2,expand=:480:::,crop=720:480,harddup
static const OptionParam scaleParams[] = 
{
	{VIDEO_WIDTH,	"w",	NULL,		PROP_SEP,	NULL,	OPTEM_VALUE_ONLY,		OPTEF_CONTEXT | OPTEF_MUST,			NULL},
	{NULL,			"h",	"-2",		PROP_SEP,	NULL,	OPTEM_VALUE_ONLY,		OPTEF_SELF,				NULL}
};

static const DirectOptionParamList scaleOpt = 
{
	SOT_LIST, scaleParams, ARRAY_LEN(scaleParams)
};

static const OptionParam expandParams[] = 
{
	{NULL,		"w",		"",			PROP_SEP,	NULL,		OPTEM_VALUE_ONLY,		OPTEF_SELF,			NULL},
	{VIDEO_HEIGHT,"h",		NULL,		PROP_SEP,	NULL,		OPTEM_VALUE_ONLY,		OPT_CONFIGURABLE | OPTEF_MUST,	NULL}
// 	{NULL,		"x",		"",		PROP_SEP,	NULL,		OPTEM_VALUE_ONLY,		OPTEF_SELF,			NULL},
// 	{NULL,		"y",		"",		PROP_SEP,	NULL,		OPTEM_VALUE_ONLY,		OPTEF_SELF,			NULL},
// 	{NULL,		"osd",		"",		PROP_SEP,	NULL,		OPTEM_VALUE_ONLY,		OPTEF_SELF,			NULL}
};

static const DirectOptionParamList expandOpt = 
{
	SOT_LIST, expandParams, ARRAY_LEN(expandParams)
};

static const OptionParam cropParams[] = 
{
	{VIDEO_WIDTH,		"w",		NULL,		PROP_SEP,	NULL,		OPTEM_VALUE_ONLY,		OPTEF_CONTEXT | OPTEF_MUST,			NULL},
	{VIDEO_HEIGHT,		"h",		NULL,		PROP_SEP,	NULL,		OPTEM_VALUE_ONLY,		OPTEF_CONTEXT | OPTEF_MUST,			NULL}
};

static const DirectOptionParamList cropOpt = 
{
	SOT_LIST, cropParams, ARRAY_LEN(cropParams)
};

static const OptionParam filterOtps[] = 
{
	{NULL,		"scale",		NULL,		PROP_SEP,	OPTION_SEP,		OPTEM_DEFAULT,		/*OPTEF_MUST | */OPTEF_CHILDREN,			(void*)&scaleOpt},
	{NULL,		"expand",		NULL,		PROP_SEP,	OPTION_SEP,		OPTEM_DEFAULT,		/*OPTEF_MUST | */OPTEF_CHILDREN | OPTEF_KEEP_EMPTY,			(void*)&expandOpt},
	{NULL,		"crop",			NULL,		PROP_SEP,	OPTION_SEP,		OPTEM_DEFAULT,		/*OPTEF_MUST | */OPTEF_CHILDREN,			(void*)&cropOpt},
	{NULL,		"harddup",		"",		NULL,		NULL,			OPTEM_NAME_ONLY,	OPTEF_SELF,				NULL}
};

static const DirectOptionParamList vfOpts = 
{
	SOT_LIST, filterOtps, ARRAY_LEN(filterOtps)
};

//static const ChoiceOptionParamList 
static const OptionParam profile1Data[] = 
{
	{SEEK_TIME,		"-ss",		"0",		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{END_TIME,		"-endpos",	"20",		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{MAX_AV_SYNC,	"-mc",		"0",		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_SELF,			NULL},
	{NO_SKIP,		"-noskip",	"",			ARG_SEP,	NULL,		OPTEM_NAME_ONLY,	OPTEF_SELF,			NULL},
	{SWS,			"-sws",		"",			ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},

	{OF,			"-of",		NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT,		NULL},
	{OF_OPTS,		NULL,		NULL,		ARG_SEP,	OPTION_SEP,	OPTEM_DEFAULT,		OPTEF_CHILDREN,		(void*)&of_opts_choices},

	{AUDIO_FILTER,	"-af",		"volnorm",	ARG_SEP,	FILTER_SEP,	OPTEM_DEFAULT,		OPTEF_MUST | OPTEF_CHILDREN | OPTEF_SELF,	/*TODO*/NULL},
	{VIDEO_FILTER,	"-vf",		"harddup",	ARG_SEP,	FILTER_SEP,	OPTEM_DEFAULT,		OPTEF_MUST | OPTEF_CHILDREN/* | OPTEF_SELF*/,	(void*)&vfOpts},
	
	{OSRATE,		"-srate",	NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_MUST | OPTEF_CONTEXT,		NULL},
	{OFPS,			"-ofps",	NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_MUST | OPTEF_CONTEXT,		NULL},


	{OAC,			"-oac",		NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_MUST | OPTEF_CONTEXT,		NULL},
	{OAC_OPTS,		NULL,		NULL,		ARG_SEP,	OPTION_SEP,	OPTEM_DEFAULT,		OPTEF_CHILDREN,		(void*)&oac_opts_choices},

	{OVC,			"-ovc",		NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT,		NULL},
	{FOURCC,		"-ffourcc",	"",			ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPT_CONFIGURABLE,	NULL},
	{OVC_OPTS,		NULL,		NULL,		ARG_SEP,	OPTION_SEP,	OPTEM_DEFAULT,		OPTEF_CHILDREN,		(void*)&ovc_opts_choices},

	{PASS_LOG_FILE,	"-passlogfile",	NULL,	ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_CONTEXT,		NULL},
	{IFILE,			"",			NULL,		ARG_SEP,	NULL,		OPTEM_VALUE_ONLY,	OPTEF_MUST | OPTEF_CONTEXT,		NULL},
	{OFILE,			"-o",		NULL,		ARG_SEP,	NULL,		OPTEM_DEFAULT,		OPTEF_MUST | OPTEF_CONTEXT,		NULL}
};


bool IsParamValid(const OptionParam* pParam)
{
	if(pParam == NULL)
	{
		return false;
	}
	if(pParam->szID == NULL && pParam->szName == NULL && pParam->szValue == NULL && pParam->szNameValueSep == NULL
		&& pParam->szSubOptionSep == NULL && pParam->nEvaluateMode == 0 && pParam->nEvaluateFlag == 0
		&& pParam->ptr == NULL)
	{
		return false;
	}
	return true;
}

DefaultOptionExpBuilder::DefaultOptionExpBuilder()
{

}

DefaultOptionExpBuilder::~DefaultOptionExpBuilder()
{

}

OptionExp* DefaultOptionExpBuilder::Build(OptionContext* pContext)
{
	OptionExp* pRoot = BuildRoot(profile1Data, ARRAY_LEN(profile1Data), pContext);
	return pRoot;
}

/*
bool DefaultOptionExpBuilder::BuildTree(OptionContext* pContext, OptionExpTree* pTree)
{
	OptionExp* pRoot = BuildRoot(profile1Data, ARRAY_LEN(profile1Data), pContext);
	if(pRoot == NULL)
	{
		return false;
	}
	pTree->SetRoot(pRoot);
	return true;
}
*/
OptionExp* DefaultOptionExpBuilder::BuildRoot(const OptionParam* pParams, int nCount, OptionContext* pContext)
{
	if(pParams == NULL || nCount <= 0)
	{
		return NULL;
	}

	DefaultOptionExp* pRoot = new DefaultOptionExp();
	pRoot->SetSubOptionSep(ARG_SEP).SetEvaluateFlag(OPTEF_MUST | OPTEF_CHILDREN).SetEvaluateMode(OPTEM_VALUE_ONLY);

	OptionExp* pExp = NULL;
	for(int i = 0; i < nCount; i++)
	{
		pExp = Build(&pParams[i], pContext);
		pRoot->AddChild(pExp);
	}

	return pRoot;
}

bool DefaultOptionExpBuilder::ParseOptionParamArray(void* ptr, OptionContext* pContext, OptionParamList* pArray)
{
	if(ptr == NULL)
	{
		return false;
	}
	int nType = *((int*)ptr);

	bool bFound = true;
	switch (nType)
	{
	case SOT_CHOICE:
		{
			bFound = ParseChoiceOptions((ChoiceOptionParamList*)ptr, pContext, pArray);
		}
		break;
	case SOT_LIST:
		{
			DirectOptionParamList* pList = (DirectOptionParamList*)ptr;
			pArray->pOptions = pList->pOptions;
			pArray->nCount = pList->nOptionCount;
		}
		break;
	default:
		{
			bFound = false;
		}
		break;
	}
	return bFound;
}

bool DefaultOptionExpBuilder::ParseChoiceOptions(ChoiceOptionParamList* ptr, OptionContext* pContext, OptionParamList* pArray)
{
	std::string val;
	bool bRet = pContext->Get(ptr->szChoiceOptionID, val);
	if(!bRet)
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("Failed to get choice option id : %s"), CFL_A2T(ptr->szChoiceOptionID));
		LOG4CPLUS_INFO_STR(THE_LIB_LOGGER, szLog)
		return false;
	}
	for(int i = 0; i < ptr->nGroupCount; i++)
	{
		if(val.compare(0, val.size(), ptr->pGroups[i].szName) == 0)
		{
			pArray->pOptions = ptr->pGroups[i].pOptions;
			pArray->nCount = ptr->pGroups[i].nOptionCount;
			return true;
		}
	}
	return false;
}

OptionExp* DefaultOptionExpBuilder::Build(const OptionParam* pParam, OptionContext* pContext)
{
	if(!IsParamValid(pParam))
	{
		return NULL;
	}

	DefaultOptionExp* pExp = new DefaultOptionExp();
	pExp->SetOptionID(pParam->szID).SetOptionName(pParam->szName).SetOptionValue(pParam->szValue);
	pExp->SetNameValueSep(pParam->szNameValueSep).SetSubOptionSep(pParam->szSubOptionSep);
	pExp->SetEvaluateMode(pParam->nEvaluateMode).SetEvaluateFlag(pParam->nEvaluateFlag);

	if(pParam->ptr)
	{
		if(pParam->nEvaluateFlag & OPTEF_CHILDREN)
		{
			OptionParamList subOptions;
			bool bRet = ParseOptionParamArray(pParam->ptr,pContext, &subOptions);
			if(bRet && subOptions.nCount > 0)
			{
				OptionExp* pChildExp;
				for(int i = 0; i < subOptions.nCount; i++)
				{
					pChildExp = Build(&subOptions.pOptions[i], pContext);
					if(pChildExp)
					{
						pExp->AddChild(pChildExp);
					}
				}
			}
		}
		else if(pParam->nEvaluateFlag & OPTEF_HAS_FUNC)
		{
			//pExp->SetFuncSet((FuncSet*)(&(pParam->ptr)));
			pExp->SetFuncSet((FuncSet)(pParam->ptr));
		}
	}
	return pExp;
}
