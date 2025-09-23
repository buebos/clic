#ifndef __CLIC_MODELS_COMMAND_C__
#define __CLIC_MODELS_COMMAND_C__

#include <stdbool.h>
#include <stdlib.h>

#include "../components/error.c"
#include "../components/hashmap.c"
#include "arg.c"

typedef struct Clic_Command {
    char *id;
    char *description;

    Clic_Error (*execute)(Clic_Hashmap *args);
    Clic_Error (*validate)(Clic_Hashmap *args);

    Clic_Arg *args;
    Clic_Arg *standalone_arg;
} Clic_Command;

Clic_Hashmap clic_commands_hash(Clic_Command *commands[]) {
    Clic_Hashmap map = clic_hashmap_init();
    for (int i = 0; i < (sizeof(**commands) / sizeof(commands[0]) + 1); i++) {
        clic_hashmap_set(&map, commands[i]->id, &commands[i]);
    }
    return map;
}

void *clic_command_get_arg_value(Clic_Command *command, Clic_Hashmap *args, char *key) {
    Clic_Arg *arg_target = (Clic_Arg *)clic_hashmap_get(args, key);

    if (!arg_target) {
        return NULL;
    }

    switch (arg_target->type) {
        case CLIC_ARGTYPE_BOOLEAN:
            return &arg_target->constraint.boolean.value;
        case CLIC_ARGTYPE_INT:
            return &arg_target->constraint.integer.value;
        case CLIC_ARGTYPE_FLOAT:
            return &arg_target->constraint.floatingpoint.value;
        case CLIC_ARGTYPE_STRING:
            return arg_target->constraint.string.value;
        case CLIC_ARGTYPE_ENUM:
            return arg_target->constraint.enumeration.value;
        default:
            return NULL;
    }
};

#endif /* __CLIC_MODELS_COMMAND_C__ */