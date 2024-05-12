#ifndef __CLI_GET_H__

#define __CLI_GET_H__

#include "cmd.h"
#include "core.h"
#include "param.h"

void* cli_get(Cli* cli, char* id);
CliCommand* cli_get_cmd(Cli* cli);

#endif  // !__CLI_GET_H__