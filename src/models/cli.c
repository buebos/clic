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


#endif /* __CLIC_CLI_H__ */
