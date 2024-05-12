#include "../../include/cmd.h"

CliFlag* cli_cmd_find_flag(CliCommand* root, char* flag_id) {
    while (root) {
        for (int i = 0; i < root->flags_len; i++) {
            CliFlag* flag = &(root->flags[i]);
            bool is_flag_id_match = strcmp(flag->id, flag_id) == 0;

            if (is_flag_id_match) {
                return flag;
            }

            for (int j = 0; j < flag->aliases_len && flag->aliases; j++) {
                char* flag_alias = flag->aliases[j];

                if (strcmp(flag_alias, flag_id) == 0) {
                    return flag;
                }
            }
        }

        root = root->_next;
    }

    return NULL;
}

CliCommand* cli_cmd_find_subcmd(CliCommand* cmd, char* cmd_id) {
    for (int i = 0; i < cmd->cmds_len; i++) {
        CliCommand* command = cmd->cmds[i];

        if (strcmp(command->id, cmd_id) == 0) {
            return command;
        }

        for (int j = 0; j < command->aliases_len && command->aliases; i++) {
            char* flag_alias = command->aliases[i];

            if (strcmp(flag_alias, cmd_id) == 0) {
                return command;
            }
        }
    }

    return NULL;
};
