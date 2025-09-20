#ifndef __CLIC_CLI_H__
#define __CLIC_CLI_H__

#include <stdbool.h>
#include <stdlib.h>

#include "../components/error.c"
#include "../components/hashmap.c"

enum Clic_ArgType {
    CLIC_ARGTYPE_BOOLEAN = 0,
    CLIC_ARGTYPE_INT,
    CLIC_ARGTYPE_FLOAT,
    CLIC_ARGTYPE_STRING,
    CLIC_ARGTYPE_ENUM,
};

typedef struct Clic_ArgConstraintInteger {
    int min;
    int max;
    int divisible;

    int value;
} Clic_ArgConstraintInteger;
typedef struct Clic_ArgConstraintFloat {
    float min;
    float max;

    float value;
} Clic_ArgConstraintFloat;
typedef struct Clic_ArgConstraintString {
    size_t min_len;
    size_t max_len;

    char *value;
} Clic_ArgConstraintString;
typedef struct Clic_ArgConstraintEnum {
    char **values;

    char *value;
} Clic_ArgConstraintEnum;

union Clic_ArgConstraint {
    Clic_ArgConstraintInteger integer;
    Clic_ArgConstraintFloat floatingpoint;
    Clic_ArgConstraintString string;
    Clic_ArgConstraintEnum enumeration;
    bool boolean;
};

typedef struct Clic_Arg {
    char *id;
    char *abbr;
    char *description;

    char nullable;

    enum Clic_ArgType type;
    union Clic_ArgConstraint constraint;
} Clic_Arg;

typedef struct Clic_Command {
    char *id;
    char *description;

    Clic_Error (*execute)(Clic_Arg *);
    Clic_Error (*validate)(Clic_Arg *);

    Clic_Arg *args;
} Clic_Command;

typedef struct Clic_Cli {
    char *id;
    char *description;

    Clic_Hashmap _commands_hash;
    Clic_Hashmap _args_hash;

    Clic_Command *root_command;
    Clic_Command *commands[];
} Clic_Cli;

Clic_Hashmap clic_args_hash(Clic_Arg args[]) {
    Clic_Hashmap map = clic_hashmap_init();
    for (int i = 0; i < sizeof(*args) / sizeof(args[0]); i++) {
        clic_hashmap_set(&map, args[i].id, &args[i]);
        clic_hashmap_set(&map, args[i].abbr, &args[i]);
    }
    return map;
}
Clic_Hashmap clic_commands_hash(Clic_Command *commands[]) {
    Clic_Hashmap map = clic_hashmap_init();
    for (int i = 0; i < sizeof(**commands) / sizeof(commands[0]); i++) {
        clic_hashmap_set(&map, commands[i]->id, &commands[i]);
    }
    return map;
}

void *clic_args_get();

#endif /* __CLIC_CLI_H__ */
