#include "../include/main.c"

#include <stdio.h>

Clic_Error help_command_validate(Clic_Hashmap *args) {
    return (Clic_Error){0};
}
Clic_Error help_command_execute(Clic_Hashmap *args) {
    Clic_Arg *command_arg = clic_hashmap_get(args, "command");

    printf("Help command 'command' arg value: %s\n", command_arg->constraint.string.value);

    return (Clic_Error){0};
}
Clic_Command help_command = {
    .id = "help",
    .description = "Help command.",
    .execute = help_command_execute,
    .validate = help_command_validate,

    .standalone_arg = &(Clic_Arg){
        .id = "command",
        .abbr = "c",
        .type = CLIC_ARGTYPE_STRING,
        .constraint = {
            .string = {
                .value = (char[32]){0},
            },
        },
    },

    .args = NULL,
};

Clic_Cli cli = {
    .id = "app",
    .description = "This is an example app for demonstrating the cli functionality.",

    .commands = {&help_command},
};

int main(int argc, char *argv[]) {
    Clic_Command command = {0};

    CLIC_ERROR_TRY(clic_cli_parse(&cli, argc, argv, &command));
    CLIC_ERROR_TRY(command.execute(&cli._.args_hash));

    return 0;
}
