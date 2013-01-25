#ifndef __MP4_CMD_LIST_BUILDER_H__
#define __MP4_CMD_LIST_BUILDER_H__

#include "CmdBuilder.h"
#include "OptionContext.h"

class MP4CmdListBuilder : public CmdListBuilder
{
public:
	MP4CmdListBuilder();
	virtual CmdListBuilder* Clone();
	virtual bool Build(std::vector<CmdInfo>& commands, StrObjPtrContext& context);
};

#endif