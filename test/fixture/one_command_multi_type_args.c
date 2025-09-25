#ifndef __TEST_FIXTURE_ONE_COMMAND_MULTI_TYPE_ARGS_C__
#define __TEST_FIXTURE_ONE_COMMAND_MULTI_TYPE_ARGS_C__
#include "../../include/main.c"

Clic_Command one_command = {
    .id = "command",
    .description = "One command demonstrating multi-type arguments",
    .execute = NULL,
    .validate = NULL,
    .standalone_arg = &(Clic_Arg){
        .id = "file",
        .abbr = "f",
        .description = "Input file path",
        .nullable = false,
        .type = CLIC_ARGTYPE_STRING,
        .constraint = {
            .string = {
                .min_len = 0,
                .max_len = 0,
                .value = (char[256]){0},
            },
        },
    },
    .args = (Clic_Arg[]){
        {
            .id = "mode",
            .abbr = "m",
            .description = "Build mode (debug|release)",
            .nullable = true,
            .type = CLIC_ARGTYPE_ENUM,
            .constraint = {
                .enumeration = {
                    .values = (char *[]){
                        "debug",
                        "release",
                        NULL,
                    },
                    .value = (char[256]){0},
                },
            },
        },
        {
            .id = "release",
            .abbr = "r",
            .description = "Shortcut for --mode release",
            .nullable = true,
            .type = CLIC_ARGTYPE_BOOLEAN,
            .constraint = {
                .boolean = {
                    .value = false,
                },
            },
        },
        {
            .id = "count",
            .abbr = "c",
            .description = "Repetition count",
            .nullable = true,
            .type = CLIC_ARGTYPE_INT,
            .constraint = {
                .integer = {
                    .min = 0,
                    .max = 0,
                    .divisible = 0,
                    .value = 0,
                },
            },
        },
        {
            .id = "factor",
            .abbr = "fa",
            .description = "Scale factor",
            .nullable = true,
            .type = CLIC_ARGTYPE_FLOAT,
            .constraint = {
                .floatingpoint = {
                    .min = 0,
                    .max = 0,
                    .value = 0,
                },
            },
        },
        {
            .id = "string",
            .abbr = "str",
            .type = CLIC_ARGTYPE_STRING,
            .constraint = {
                .string = {
                    .min_len = 1,
                    .max_len = 8,
                    .value = (char[8]){0},
                },
            },
        },
    },
};

Clic_Command no_args_command = {
    .id = "command",
    .description = "One command demonstrating multi-type arguments",
    .execute = NULL,
    .validate = NULL,
    .standalone_arg = NULL,
    .args = NULL,
};

Clic_Cli cli = {
    .id = "one_command_multi_type_args",
    .description = "Example configuration for a CLI test fixture.",
    .root_command = &one_command,
    .commands = {
        &one_command,
        &no_args_command,
        NULL,
    },
};

#endif /* __TEST_FIXTURE_ONE_COMMAND_MULTI_TYPE_ARGS_C__ */
