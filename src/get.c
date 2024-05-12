#include "../include/get.h"

void* cli_get(Cli* cli, char* id) {
    CliCommand* cmdcur = cli->_tail;

    while (cmdcur) {
        CliFlag* flag = cli_cmd_find_flag(cmdcur, id);

        if (flag) {
            return (void*)(&flag->_is_set);
        }

        CliParam* param = NULL;

        param = cli_param_get(cmdcur->params, cmdcur->params_len, id);

        if (param) {
            return (void*)param->_value;
        }

        for (int i = 0; i < cmdcur->flags_len; i++) {
            CliFlag* flag = &cmdcur->flags[i];

            param = cli_param_get(flag->params, flag->params_len, id);

            if (param) {
                return (void*)param->_value;
            }
        }

        cmdcur = cmdcur->_prev;
    }

    return NULL;
}

CliCommand* cli_get_cmd(Cli* cli) {
    return cli->_tail;
}
