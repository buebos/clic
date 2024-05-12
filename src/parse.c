#include "../include/parse.h"

CliParseResult cli_param_parse_validate(CliParam* param) {
    char* value = param->_value;

    if (!value) {
        if (param->is_nullable) {
            return (CliParseResult){.status = CLI_PARSE_SUCCESS, .data = {0}};
        }

        return (CliParseResult){
            .status = CLI_PARSE_ERROR_PARAM_MISSING,
            .data = {0},
        };
    }

    switch (param->type) {
        case CLI_BOOL:
            if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
                break;
            }
            if (strcmp(value, "1") == 0 || strcmp(value, "0") == 0) {
                break;
            }

            return (CliParseResult){
                .status = CLI_PARSE_ERROR_PARAM_TYPE,
                .data = {0},
            };
        case CLI_INT:
        case CLI_FLOAT: {
            char* endptr;

            if (param->type == CLI_INT) {
                strtol(value, &endptr, 10);
            } else {
                strtod(value, &endptr);
            }

            if (*endptr != '\0') {
                return (CliParseResult){
                    .status = CLI_PARSE_ERROR_PARAM_TYPE,
                    .data = {0},
                };
            }

            break;
        }
        case CLI_PATH_DIR: {
            break;
        }
        case CLI_PATH_FILE: {
            break;
        }
        case CLI_STR:
        default:
            break;
    }

    if (param->value_constraints) {
        bool is_within_constraint = false;

        for (size_t i = 0; i < param->value_constraints_len; i++) {
            char* constraint = strlwr(param->value_constraints[i]);

            if (strcmp(strlwr(value), constraint) == 0) {
                is_within_constraint = true;
                break;
            }
        }

        if (!is_within_constraint) {
            return (CliParseResult){
                .status = CLI_PARSE_ERROR_PARAM_VALUE_CONSTRAINT,
                .data = {0},
            };
        }
    }

    return (CliParseResult){.status = CLI_PARSE_SUCCESS, .data = {0}};
}

CliParseResult cli_flag_parse_validate(CliFlag* flag) {
    if (!flag->_is_set && !flag->is_nullable) {
        return (CliParseResult){
            .status = CLI_PARSE_ERROR_FLAG_MISSING,
            .data = {0},
        };
    }

    for (size_t i = 0; i < flag->params_len; i++) {
        CliParseResult res = cli_param_parse_validate(&flag->params[i]);

        if (res.status != CLI_PARSE_SUCCESS) {
            return res;
        }
    }

    return (CliParseResult){.status = CLI_PARSE_SUCCESS, .data = {0}};
}

CliParseResult cli_cmd_parse_validate(CliCommand* cmd) {
    while (cmd) {
        for (size_t i = 0; i < cmd->params_len; i++) {
            CliParseResult res = cli_param_parse_validate(&cmd->params[i]);

            if (res.status != CLI_PARSE_SUCCESS) {
                return res;
            }
        }

        for (size_t i = 0; i < cmd->flags_len; i++) {
            CliFlag* flag = &cmd->flags[i];

            if (!flag->_is_set) {
                continue;
            }

            CliParseResult res = cli_flag_parse_validate(flag);

            if (res.status != CLI_PARSE_SUCCESS) {
                return res;
            }
        }

        cmd = cmd->_next;
    }

    return (CliParseResult){.status = CLI_PARSE_SUCCESS, .data = {0}};
}

CliParseResult cli_parse(Cli* cli, int argc, char** argv) {
    if (!cli->cmds) {
        return (CliParseResult){.status = CLI_PARSE_SUCCESS, .data = {0}};
    }

    CliCommand* cmd_current = NULL;
    CliFlag* flag_current = NULL;

    for (int i = 1; i < argc; i++) {
        CliTok tok = cli_tok(cli, cmd_current, flag_current, argv[i]);

        switch (tok.type) {
            case CLI_TOK_PARAM:
                tok.match.param->_value = tok.value;

                CliParseResult res = cli_param_parse_validate(tok.match.param);

                if (res.status != CLI_PARSE_SUCCESS) {
                    return res;
                }

                break;
            case CLI_TOK_FLAG:
                if (flag_current) {
                    CliParseResult res = cli_flag_parse_validate(flag_current);

                    if (res.status != CLI_PARSE_SUCCESS) {
                        return res;
                    }
                }

                tok.match.flag->_is_set = true;

                if (tok.match.flag->params && tok.match.flag->params_len) {
                    flag_current = tok.match.flag;
                } else {
                    flag_current = NULL;
                }

                break;
            case CLI_TOK_CMD:
                if (flag_current) {
                    CliParseResult res = cli_flag_parse_validate(flag_current);

                    if (res.status != CLI_PARSE_SUCCESS) {
                        return res;
                    }
                }

                if (cmd_current) {
                    CliParseResult res = cli_cmd_parse_validate(cmd_current);
                    if (res.status != CLI_PARSE_SUCCESS) {
                        return res;
                    }

                    cmd_current->_next = tok.match.cmd;
                } else {
                    cli->_head = tok.match.cmd;
                }

                flag_current = NULL;
                tok.match.cmd->_next = NULL;

                tok.match.cmd->_prev = cmd_current;
                cmd_current = tok.match.cmd;
                cli->_tail = cmd_current;

                break;
            default:
                return (CliParseResult){
                    .status = CLI_PARSE_ERROR_UNKNOWN_TOK,
                    .data = {
                        .unknown_token = {
                            .index = i,
                        },
                    },
                };
        }
    }

    if (!cmd_current) {
        return (CliParseResult){
            .status = CLI_PARSE_ERROR_NO_CMD,
            .data = {0},
        };
    }

    if (flag_current) {
        CliParseResult res = cli_flag_parse_validate(flag_current);
        if (res.status != CLI_PARSE_SUCCESS) {
            return res;
        }
    }

    CliParseResult res = cli_cmd_parse_validate(cmd_current);
    if (res.status != CLI_PARSE_SUCCESS) {
        return res;
    }

    return (CliParseResult){
        .status = CLI_PARSE_SUCCESS,
        .data = {0},
    };
}
