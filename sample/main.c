#include <stdio.h>

#include "../include/get.h"
#include "../include/parse.h"

typedef struct AppSample {
    Cli *cli;
} AppSample;

typedef int (*AppCommandRun)(AppSample *app);

CliCommand Hello = {
    .id = "hello",
    .run = NULL,

    .aliases_len = 0,
    .aliases = NULL,

    .cmds_len = 0,
    .cmds = NULL,

    .params_len = 1,
    .params = (CliParam[]){
        {
            .id = "hello.name",
            .is_list = false,
            .is_nullable = true,
            .type = CLI_STR,

            .value_constraints_len = 0,
            .value_constraints = NULL,
        },
    },

    .flags_len = 0,
    .flags = NULL,
};

Cli cli = {
    .name = "sample",
    .epilogue = "The sample clic app",

    .cmds_len = 1,
    .cmds = (CliCommand *[]){
        &Hello,
    },

    .flags_len = 0,
    .flags = NULL,
};

int main(int argc, char **argv) {
    AppSample app = {
        .cli = &cli,
    };

    CliParseResult res = cli_parse(app.cli, argc, argv);

    if (res.status != CLI_PARSE_SUCCESS) {
        printf("[ERROR]: Something went wrong parsing the cli input, error code: %d.\n", res.status);
        return res.status;
    }

    CliCommand *target = cli_get_cmd(&cli);

    if (target->run) {
        return ((AppCommandRun)target->run)(&app);
    }

    if (strcmp(target->id, "hello") == 0) {
        char *name = cli_get(app.cli, "hello.name");

        printf("[INFO]: Hello ");

        if (name) {
            printf("%s ", name);
        }

        printf("from the 'hello' command!\n");
    } else {
        printf("[WARN]: Command with no handle case.\n");
        return -1;
    }

    return 0;
}