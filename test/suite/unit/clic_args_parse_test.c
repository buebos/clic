#include <assert.h>
#include <stdio.h>

#include "../../../include/main.c"
#include "../../fixture/one_command_multi_type_args.c"

/**
 * Valid argument syntax given the command definitions:
 *
 * 1. $ cli command text.txt --mode release
 * 2. $ cli command --file text.txt --mode
 * 3. $ cli command --mode release text.txt
 * 4. $ cli command text.txt --release
 * 5. $ cli command --release text.txt
 */
int main(int argc, char *argv[]) {
    Clic_Error error = {0};
    Clic_Command *command = cli.commands[0];
    Clic_Arg *args_schema = command->args;

    /**
     * Case 1: All of arg values with ID prefixes.
     *
     * $ cli --file test.txt --mode debug
     */
    char *args[] = {"--file", "test.txt", "--mode", "debug"};
    Clic_Hashmap args_hash = clic_args_hash(command->standalone_arg, args_schema);
    error = clic_args_parse(command->standalone_arg, args_schema, &args_hash, sizeof(args) / sizeof(args[0]), args);
    assert(error.code == 0 && "Cli parsing should be successful");
    assert(
        strcmp(
            clic_command_get_arg_value(command, &args_hash, "file"),
            "test.txt") == 0

    );
    assert(
        strcmp(
            clic_command_get_arg_value(command, &args_hash, "mode"),
            "debug") == 0

    );

    /**
     * Case 2: Mixed standalone values with ID prefixes.
     *
     * $ cli test.txt --mode debug
     */
    char *args_2[] = {"test.txt", "--mode", "debug"};
    error = clic_args_parse(command->standalone_arg, args_schema, &args_hash, sizeof(args_2) / sizeof(args_2[0]), args_2);
    assert(
        strcmp(
            clic_command_get_arg_value(command, &args_hash, "file"),
            "test.txt") == 0

    );
    assert(
        strcmp(
            clic_command_get_arg_value(command, &args_hash, "mode"),
            "debug") == 0

    );

    /**
     * Case 3: Unordered mixed standalone values with ID prefixes.
     */

    printf("[SUCCESS]: clic_args_parse_test\n");

    return 0;
}
