#ifndef __XC_CMDS_H__
#define __XC_CMDS_H__

#include "cmd.h"

extern cmd_descriptor_t cmds_root[];

void cmds_default(const char* str);
#endif
