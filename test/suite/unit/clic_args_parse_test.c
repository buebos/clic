#include <assert.h>
#include <stdio.h>

#include "../../../include/main.c"

int main(int argc, char *argv[]) {
    Clic_Arg *args_schema = (Clic_Arg[]){
        {
            .id = "file",
            .abbr = "f",
            .type = CLIC_ARGTYPE_STRING,
            .constraint = {
                .string = {
                    .value = (char[256]){0},
                },
            },
        },
        {
            .id = "mode",
            .abbr = "m",
            .type = CLIC_ARGTYPE_ENUM,
            .constraint = {
                .enumeration = {
                    .values = (char *[]){
                        "debug",
                        "release",
                    },
                    .value = (char[256]){0},
                },
            },
        },
    };

    char *args[] = {"--file", "test.txt", "--mode", "debug"};

    Clic_Hashmap args_hash = clic_args_hash(args_schema);

    CLIC_ERROR_TRY(clic_args_parse(args_schema, &args_hash, sizeof(args) / sizeof(args[0]), args));

    assert(strcmp(args_schema[0].constraint.string.value, "test.txt") == 0);
    assert(strcmp(args_schema[1].constraint.enumeration.value, "debug") == 0);

    char *args_2[] = {"test.txt", "--mode", "debug"};

    CLIC_ERROR_TRY(clic_args_parse(args_schema, &args_hash, sizeof(args_2) / sizeof(args_2[0]), args_2));

    assert(strcmp(args_schema[0].constraint.string.value, "test.txt") == 0);
    assert(strcmp(args_schema[1].constraint.enumeration.value, "debug") == 0);

    printf("[SUCCESS]: clic_args_parse_test\n");

    return 0;
}