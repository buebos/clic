#include "../include/main.c"

#include <stdio.h>

Clic_Error help_command_validate(Clic_Arg *args) {
    return (Clic_Error){.code = 0};
}
Clic_Error help_command_execute(Clic_Arg *args) {
    int *value = clic_args_get(args, "value");

    printf("Value: %d\n", value);

    return (Clic_Error){.code = 0};
}
Clic_Command help_command = {
    .id = "help",
    .description = "Help command.",
    .execute = help_command_execute,
    .validate = help_command_validate,

    .args = (Clic_Arg[]){
        {
            .id = "command",
            .abbr = "c",
            .type = CLIC_ARGTYPE_INT,
            .constraint = {
                .integer = {
                    .min = 0,
                    .max = 1000,
                    .value = 0,
                },
            },
        },
    },
};

Clic_Cli cli = {
    .id = "app",
    .description = "This is an example app for demonstrating the cli functionality.",

    .commands = (Clic_Command *[]){
        &help_command,
    },
};

int main(int argc, char *argv[]) {
    Clic_Command command = {0};

    CLIC_ERROR_TRY(clic_cli_parse(&cli, &command, argc, argv));
    CLIC_ERROR_TRY(command.execute(command.args));

    return 0;
}
