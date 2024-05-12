#ifndef __CLI_PARSE_H__

#define __CLI_PARSE_H__

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "token.h"

typedef enum CliParseResStatus {
    CLI_PARSE_SUCCESS = 0,
    CLI_PARSE_ERROR_PARAM_TYPE,
    CLI_PARSE_ERROR_UNKNOWN_TOK,
    CLI_PARSE_ERROR_PARAM_MISSING,
    CLI_PARSE_ERROR_PARAM_VALUE_CONSTRAINT,

    CLI_PARSE_ERROR_FLAG_MISSING,

    CLI_PARSE_ERROR_NO_CMD,
} CliParseResStatus;

typedef struct CliParseUnknownTok {
    size_t good_until_index;
} CliParseUnknownTok;

typedef union CliParseResData {
    CliParseUnknownTok unknown_token;
} CliParseResData;

typedef struct CliParseResult {
    CliParseResStatus status;
    CliParseResData data;
} CliParseResult;

void* cli_get(Cli* cli, char* arg_id);

CliParseResult cli_parse(Cli* cli, int argc, char** argv);

#endif  // !__CLI_PARSE_H__
