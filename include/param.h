#ifndef __CLI_PARAM_H__

#define __CLI_PARAM_H__

#include "core.h"

CliParam* cli_param_get(CliParam* params, size_t params_len, char* id);

CliParam* cli_param_get_last(CliParam* params, size_t params_len);

#endif  // !__CLI_PARAM_H__