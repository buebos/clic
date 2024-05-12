#ifndef __CLI_TOKEN_H__

#define __CLI_TOKEN_H__

#include <stdbool.h>

#include "cmd.h"
#include "core.h"
#include "param.h"

#define CLI_TOKEN_FLAG_PREFIX '-'

/** The '--' chars before flags */
#define CLI_FLAG_ID_PRE_LEN 2
/** The '-' char before flag short aliases */
#define CLI_FLAG_ALIAS_PRE_LEN 1

typedef enum CliTokType {
    CLI_TOK_UNKNOWN = 0,
    CLI_TOK_PARAM,
    CLI_TOK_FLAG,
    CLI_TOK_CMD,
} CliTokType;
typedef union CliTokMatch {
    CliParam* param;
    CliFlag* flag;
    CliCommand* cmd;
} CliTokMatch;
typedef struct CliTok {
    CliTokType type;
    CliTokMatch match;
    char* value;
} CliTok;

bool cli_tok_is_flag(char* tok);

CliTok cli_tok(Cli* cli, CliCommand* cmd_current, CliFlag* flag_current, char* tok_val);

#endif  // !__CLI_TOKEN_H__