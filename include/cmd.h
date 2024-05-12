#ifndef __CLI_CMD_H__

#define __CLI_CMD_H__

#include "core.h"

CliFlag* cli_cmd_find_flag(CliCommand* cmd, char* tok);
CliCommand* cli_cmd_find_subcmd(CliCommand* cmd, char* cmd_id);

#endif  // !__CLI_CMD_H__