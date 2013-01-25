#ifndef __ME_CMD_LIST_BUILDER_H__
#define __ME_CMD_LIST_BUILDER_H__

#include "CmdBuilder.h"
#include "OptionContext.h"

class MeCmdListBuilder : public CmdListBuilder
{
public:
	MeCmdListBuilder();
	virtual CmdListBuilder* Clone();
	virtual bool Build(std::vector<CmdInfo>& commands, StrObjPtrContext& context);
};

#endif