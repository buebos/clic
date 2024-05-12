#include "../../include/token.h"

bool cli_tok_is_flag(char* tok) {
    if (tok == NULL) {
        return false;
    }

    return tok[0] == CLI_TOKEN_FLAG_PREFIX;
}

char* cli_tok_flag_clean(char* tok) {
    if (strlen(tok) < CLI_FLAG_ALIAS_PRE_LEN) {
        return NULL;
    }

    short offset = 0;

    while (tok[offset] == CLI_TOKEN_FLAG_PREFIX) {
        offset += 1;
    }

    return tok + offset;
}

CliTok cli_tok(Cli* cli, CliCommand* cmd_current, CliFlag* flag_current, char* tok_val) {
    CliTok tok = {
        .type = CLI_TOK_UNKNOWN,
        .match = {0},
        .value = tok_val,
    };

    if (cli_tok_is_flag(tok_val)) {
        char* tok_flag_id = cli_tok_flag_clean(tok_val);

        if (tok_flag_id == NULL) {
            return tok;
        }

        CliFlag* flag_match = cli_cmd_find_flag(cli->_head, tok_flag_id);

        if (!flag_match) {
            return tok;
        }

        tok.type = CLI_TOK_FLAG;
        tok.match.flag = flag_match;

        return tok;
    }

    CliCommand* cmd_match = cli_cmd_find_subcmd(
        cmd_current
            ? cmd_current
            : (&(CliCommand){
                  .cmds = cli->cmds,
                  .cmds_len = cli->cmds_len,
                  ._next = NULL,
                  ._prev = NULL,
              }),
        tok_val

    );

    if (cmd_match) {
        tok.type = CLI_TOK_CMD;
        tok.match.cmd = cmd_match;
        return tok;
    }

    CliParam* param_match = NULL;

    if (flag_current) {
        param_match = cli_param_get_last(flag_current->params, flag_current->params_len);
    } else if (cmd_current) {
        param_match = cli_param_get_last(cmd_current->params, cmd_current->params_len);
    }

    if (param_match) {
        tok.type = CLI_TOK_PARAM;
        tok.match.param = param_match;
    }

    return tok;
}