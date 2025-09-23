#ifndef __CLIC_CLI_H__
#define __CLIC_CLI_H__

#include <stdbool.h>
#include <stdlib.h>

#include "../components/error.c"
#include "../components/hashmap.c"
#include "arg.c"
#include "command.c"

typedef struct Clic_Cli {
    char *id;
    char *description;

    /** Private */
    struct _Clic_Cli {
        Clic_Hashmap commands_hash;
        Clic_Hashmap args_hash;
    } _;

    Clic_Command *root_command;
    Clic_Command *commands[];
} Clic_Cli;

Clic_Error clic_cli_parse(Clic_Cli *cli, int argc, char *argv[], Clic_Command *command_target) {
    if (argc == 1 && !cli->root_command) {
        return (Clic_Error){
            .code = 1,
            .message = "No command provided.",
        };
    }

    char offset = 1;

    if (argc == 1) {
        *command_target = *cli->root_command;
    } else {
        cli->_.commands_hash = clic_commands_hash(cli->commands);

        Clic_Command *match = clic_hashmap_get(&cli->_.commands_hash, argv[1]);

        if (NULL == match && !cli->root_command) {
            return (Clic_Error){.code = 1, "No command provided."};
        }

        /**
         * No command matched for the 2nd arg so take the root command
         * as the default.
         */
        if (NULL == match && cli->root_command) {
            *command_target = *cli->root_command;
        } else {
            *command_target = *match;
            offset = 2;
        }
    }

    Clic_Error args_parse_err = clic_args_parse(
        command_target->standalone_arg,
        command_target->args,
        &cli->_.args_hash,
        argc - offset,
        argv + offset

    );

    if (args_parse_err.code != 0) {
        return args_parse_err;
    }

    return (Clic_Error){0};
}

#endif /* __CLIC_CLI_H__ */
