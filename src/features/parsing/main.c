#ifndef __CLIC_FEATURES_PARSING_MAIN_C__
#define __CLIC_FEATURES_PARSING_MAIN_C__

#include "../../components/error.c"
#include "../../models/arg.c"
#include "../../models/command.c"
#include "../../models/cli.c"

Clic_Error clic_args_parse(
    Clic_Arg *standalone_arg,
    Clic_Arg *args,
    Clic_Hashmap *args_hash,
    int argc,
    char *argv[]

) {
    for (int i = 0; i < argc; i++) {
        Clic_Argv argvelem = clic_argv_init(argv[i]);

        Clic_Arg *arg_target = NULL;
        char *arg_str_value = NULL;

        if (argvelem.type == CLIC_ARGVTYPE_ARG_STANDALONE) {
            arg_target = standalone_arg;
        }
        if (argvelem.type == CLIC_ARGVTYPE_ARG_ID || argvelem.type == CLIC_ARGVTYPE_ARG_ABBR) {
            arg_target = clic_hashmap_get(args_hash, argvelem.cleaned_str);
        }

        if (NULL == arg_target) {
            return (Clic_Error){.code = -1, .message = "No argument target found."};
        }

        if (argvelem.type == CLIC_ARGVTYPE_ARG_STANDALONE) {
            arg_str_value = argvelem.cleaned_str;
        } else if (arg_target->type != CLIC_ARGTYPE_BOOLEAN) {
            if (i == argc - 1 && arg_target->nullable == false) {
                return (Clic_Error){.code = -1, .message = "No argument value provided."};
            }

            Clic_Argv argvelem_next = clic_argv_init(argv[i + 1]);

            if (argvelem_next.type != CLIC_ARGVTYPE_ARG_STANDALONE) {
                return (Clic_Error){.code = -1, .message = "No argument value provided."};
            }

            arg_str_value = argvelem_next.cleaned_str;
            i++;
        }

        switch (arg_target->type) {
            case CLIC_ARGTYPE_BOOLEAN:
                arg_target->constraint.boolean.value = true;
                break;
            case CLIC_ARGTYPE_INT:
                arg_target->constraint.integer.value = atoi(arg_str_value);
                break;
            case CLIC_ARGTYPE_FLOAT:
                arg_target->constraint.floatingpoint.value = atof(arg_str_value);
                break;
            case CLIC_ARGTYPE_STRING:
                arg_target->constraint.string.value = arg_str_value;
                break;
            case CLIC_ARGTYPE_ENUM:
                arg_target->constraint.enumeration.value = arg_str_value;
                break;
            default:
                break;
        }
    }

    return (Clic_Error){0};
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

#endif /* __CLIC_FEATURES_PARSING_MAIN_C__ */
