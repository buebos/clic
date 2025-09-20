#ifndef __CLIC_FEATURE_CLI_MAIN_C__
#define __CLIC_FEATURE_CLI_MAIN_C__

#include "../components/error.c"
#include "../models/cli.c"

typedef enum Clic_ArgvType {
    CLIC_ARGVTYPE_ARG_ID = 0,
    CLIC_ARGVTYPE_ARG_ABBR,
    CLIC_ARGVTYPE_ARG_VALUE,
} Clic_ArgvType;

typedef struct Clic_Argv {
    Clic_ArgvType type;
    char *value;
} Clic_Argv;

Clic_Argv clic_argv_init(char *argv) {
    if (argv[0] == '-' && argv[1] != '-') {
        return (Clic_Argv){.type = CLIC_ARGVTYPE_ARG_ABBR, .value = argv + 1};
    }
    if (argv[0] == '-' && argv[1] == '-') {
        return (Clic_Argv){.type = CLIC_ARGVTYPE_ARG_ID, .value = argv + 2};
    }

    return (Clic_Argv){.type = CLIC_ARGVTYPE_ARG_VALUE, .value = argv};
}

Clic_Error clic_args_parse(Clic_Arg *args, Clic_Hashmap *args_hash, int argc, char *argv[]) {
    size_t standalone_args_cursor = 0;

    for (int i = 0; i < argc; i++) {
        Clic_Argv argvelem = clic_argv_init(argv[i]);

        Clic_Arg *arg_target = NULL;
        char *arg_value = NULL;

        if (argvelem.type == CLIC_ARGVTYPE_ARG_VALUE) {
            arg_target = &args[standalone_args_cursor];
            standalone_args_cursor++;
        }
        if (argvelem.type == CLIC_ARGVTYPE_ARG_ID || argvelem.type == CLIC_ARGVTYPE_ARG_ABBR) {
            arg_target = clic_hashmap_get(args_hash, argvelem.value);
        }

        if (NULL == arg_target) {
            return (Clic_Error){.code = -1, .message = "No argument target found."};
        }

        if (argvelem.type == CLIC_ARGVTYPE_ARG_VALUE) {
            arg_value = argvelem.value;
        } else {
            if (
                i == argc - 1 &&
                arg_target->type != CLIC_ARGTYPE_BOOLEAN &&
                arg_target->nullable == false

            ) {
                return (Clic_Error){.code = -1, .message = "No argument value provided."};
            }

            Clic_Argv argvelem_next = clic_argv_init(argv[i + 1]);

            if (argvelem_next.type != CLIC_ARGVTYPE_ARG_VALUE) {
                return (Clic_Error){.code = -1, .message = "No argument value provided."};
            }

            arg_value = argvelem_next.value;
            i++;
        }

        switch (arg_target->type) {
            case CLIC_ARGTYPE_INT:
                arg_target->constraint.integer.value = atoi(arg_value);
                break;
            case CLIC_ARGTYPE_FLOAT:
                arg_target->constraint.floatingpoint.value = atof(arg_value);
                break;
            case CLIC_ARGTYPE_STRING:
                arg_target->constraint.string.value = arg_value;
                break;
            case CLIC_ARGTYPE_ENUM:
                arg_target->constraint.enumeration.value = arg_value;
                break;
            default:
                break;
        }
    }


    return (Clic_Error){.code = 0};
}

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
        cli->_commands_hash = clic_commands_hash(cli->commands);

        Clic_Command *match = clic_hashmap_get(&cli->_commands_hash, argv[1]);

        /**
         * No command matched for the 2nd arg so take the root command
         * as the default.
         */
        if (NULL == match) {
            *command_target = *cli->root_command;
        } else {
            *command_target = *match;
            offset = 2;
        }
    }

    Clic_Error args_parse_err = clic_args_parse(
        command_target->args,
        &cli->_args_hash,
        argc - offset,
        argv + offset

    );

    if (args_parse_err.code != 0) {
        return args_parse_err;
    }

    return (Clic_Error){.code = 0};
}

#endif /* __CLIC_FEATURE_CLI_MAIN_C__ */
